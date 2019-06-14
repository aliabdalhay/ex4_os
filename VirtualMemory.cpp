#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <map>
#include <vector>
#include <cstdlib>
#include <string>

//#definene TWO 2
#define ZERO 0

#define SUCCESS 1
#define FAILURE 0


void clearTable(uint64_t frameIndex)
{
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMwrite(frameIndex * PAGE_SIZE + i, 0);
	}
}


std::string addressToBinary(const uint64_t& virtualAddress)
{
    std::string binary = std::bitset<VIRTUAL_ADDRESS_WIDTH>(virtualAddress).to_string();//to binary
    return binary;
}

/*
 * <page, offset>
 * this pair is binary of an address page and offset
 */
typedef std::pair<std::string , std::string>  pageOffsetPair;




uint64_t min(uint64_t first , uint64_t second)
{
    return first <= second ? first : second;
}



uint64_t maximalCyclicDistance(uint64_t pageToFill, uint64_t curr_page) {

    uint64_t x = (uint64_t)abs((int)pageToFill - (int)curr_page);
    uint64_t y = (uint64_t)abs((int)pageToFill - (int)curr_page);
    uint64_t get_min = min(NUM_PAGES - x, y);
    return get_min;
}


pageOffsetPair translateToPage(const uint64_t &virtualAddress)
{
    std::string binary = addressToBinary(virtualAddress);
    std::string offset = binary.substr(VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH, OFFSET_WIDTH);
    std::string page = binary.substr(0, VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH);
    pageOffsetPair pair = {page, offset};
    return pair ;
}



int convertBinaryToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}



int pageTo(std::string page)
{
    std::string::size_type sz = 0;
    long long ull = std::stoull (page,&sz,0);
    int pageToFill = convertBinaryToDecimal(ull);
    return pageToFill;
}


void Dfs_travel(uint64_t frame, std::string root, int pageToFill, int max, std::string &maxString){
    unsigned int i = 0;
    int value;
    while(i < PAGE_SIZE)
    {
        PMread(frame * PAGE_SIZE + i, &value);
        if (value != 0) {
            std::string branch = std::bitset<OFFSET_WIDTH>(i).to_string();

            int index = pageTo(root + branch);

            int res = maximalCyclicDistance(pageToFill, index);
            if (res > max) {
                maxString = root + branch;
                max = res;
            }
        }
        i++;
    }
}


void DFS(int depth, uint64_t frame, std::string root, std::string &maxString, int pageToFill)
{
    int max = -1;
    if (depth == TABLES_DEPTH) {
        Dfs_travel(frame, root, pageToFill, max, maxString);
    }
    else {

        for (unsigned int i = 0; i < PAGE_SIZE; i++) {
            int val;
            PMread(frame * PAGE_SIZE + i, &val);

            std::string branch = std::bitset<OFFSET_WIDTH>(i).to_string();
            if (val != 0) {
                DFS(depth + 1, (uint64_t) val, root + branch, maxString, pageToFill);
            }
        }
    }

}




uint64_t nextUnusedFrame(uint64_t currentFrame)
{
    for( uint64_t f = 0; f < NUM_FRAMES ; f++)
    {
        uint64_t counter = 0 ;
        for( uint64_t p = 0 ; p < PAGE_SIZE; p++)
        {
            word_t  read;
            PMread(f * PAGE_SIZE + p, &read); // check if page in teh PM has a value
            if( read != 0)
            {
                break; // has value this frame is used
            }
            else {
                counter ++;
            }
        }
        if ( counter == PAGE_SIZE && f != currentFrame)
        {
            return f;
        }
    }
    return 0; // physical memory is full  need to evict
}




/**
 * Remove the link from the parent of the given frameIndx to it.
// */
void remove_link(int depth, uint64_t frame, uint64_t value)
{

    if (depth > TABLES_DEPTH) {
        return;
    }
    word_t word;
    for (unsigned int i = 0; i < PAGE_SIZE; i++) {
        PMread(value * PAGE_SIZE + i, &word);
        if ((uint64_t) word == frame) {
            PMwrite(value * PAGE_SIZE + i, 0);
            return;
        }

        if(word != 0)
        {
            remove_link(depth + 1, frame, word);
        }
    }
    return;
}



word_t read_evict_write(uint64_t idx, int nextFrame, std::string maxString)
{
    word_t value;
    PMread(idx + nextFrame * PAGE_SIZE, &value);
    int max_int = stoi(maxString, nullptr, 2);
    PMevict(value, max_int);
    clearTable(value);
    PMwrite(idx + nextFrame * PAGE_SIZE, 0);
    return value;
}


uint64_t index_substr(unsigned int i, std::string maxString)
{
    std::string str = maxString.substr(i, OFFSET_WIDTH);
    return (uint64_t) stoi(str, nullptr, 2);
}



int evict(std::string maxString, uint64_t page) {
    int nextFrame = 0;
    unsigned int i = 0;
    while(i < page)
    {
        uint64_t index = index_substr(i, maxString);
        word_t val;
        PMread(index + nextFrame * PAGE_SIZE, &val);
        nextFrame = val;
        i += OFFSET_WIDTH;
    }
    uint64_t idx = index_substr(page, maxString);
    word_t value = read_evict_write(idx, nextFrame, maxString);
    return value;
}



word_t helpToRemove(word_t current_frame, std::string page, int depth, uint64_t index){
    std::string maxString = "";
    uint64_t nextFrame = nextUnusedFrame(current_frame);
    std::string::size_type sz = 0;
    long long ull = std::stoull (page,&sz,0);
    int pageToFill = convertBinaryToDecimal(ull);
    if((int)nextFrame == 0)
    {
        DFS(depth, 0, "", maxString, pageToFill);

        uint64_t page = maxString.size() - OFFSET_WIDTH;

        nextFrame = evict(maxString, page);
    }
    remove_link(1, nextFrame, 0);
    PMwrite(current_frame * PAGE_SIZE + index, nextFrame);
    current_frame = nextFrame;
    return current_frame;
}


uint64_t toPhysical(uint64_t virtualAddress){

    pageOffsetPair pair = translateToPage(virtualAddress);
    word_t current_frame = 0;
    unsigned int i = 0;
    std::string page = pair.first;
    std::string offset = pair.second;
    int depth = 1;

    while (i < page.size())
    {
        uint64_t index = index_substr(i, page);
        word_t val;
        PMread(index + current_frame * PAGE_SIZE, &val);

        if (val != false) {

            current_frame = val;
            i += OFFSET_WIDTH;
            continue;
        }
        else
        {
            current_frame = helpToRemove(current_frame, page, depth, index);

        }
        i += OFFSET_WIDTH;
    }

    int pageToFill = stoi(page, nullptr, 2);

    int offsetToFill = stoi(offset, nullptr, 2);

    PMrestore(current_frame, pageToFill);

    uint64_t result= current_frame * PAGE_SIZE + offsetToFill;

    return result;
}


void VMinitialize() {
	clearTable(0);
}



int VMread(uint64_t virtualAddress, word_t* value) {
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE) {
        return 0;
    }
    PMread(toPhysical(virtualAddress), value);
    return SUCCESS;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE) {
        return 0;
    }
    PMwrite(toPhysical(virtualAddress), value);
    return SUCCESS;
}
