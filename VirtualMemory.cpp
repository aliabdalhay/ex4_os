#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <cmath>
#include <bitset>



#define SUCCESS 1
#define FAILURE 0
using namespace std;


/////////////////// Declaration of Functions /////////////////////////////////////////////////////


void clearTable(uint64_t frameIndex);

bitset<VIRTUAL_ADDRESS_WIDTH> toBinary(const uint64_t& virtualAddress);

bitset<OFFSET_WIDTH> toBinary2(const uint64_t& virtualAddress);

std::string addressToBinary(const uint64_t& virtualAddress);

std::string addressToBinary2(const uint64_t& virtualAddress);

/*
 * <page, offset>
 * this pair is binary of an address page and offset
 */
typedef std::pair<std::string , std::string>  pageOffsetPair;


uint64_t min(uint64_t first , uint64_t second);

uint64_t maximalCyclicDistance(uint64_t pageToFill, uint64_t curr_page);

string sub_string(const string &str, unsigned long start, unsigned long length);

uint64_t convertBinaryToDecimal(uint64_t n);

uint64_t strToint(const std::string &page);

pageOffsetPair translateToPage(const uint64_t &virtualAddress);

void read(unsigned int i, uint64_t frame, word_t &val);

void write(unsigned int i, uint64_t frame);

void evict(uint64_t value, uint64_t max_int);

void restore(uint64_t current_frame, uint64_t pageToFill);

void Dfs_travel(uint64_t frame, std::string &root, uint64_t pageToFill, double max, std::string &maxString);

void DFS(int depth, uint64_t frame, std::string root, std::string &maxString, uint64_t pageToFill);

word_t nextUnusedFrame(uint64_t currentFrame);

/**
 * Remove the link from the parent of the given frameIndx to it.
// */
void remove_link(int depth, uint64_t frame, uint64_t value);

word_t read_evict_write(unsigned int idx, uint64_t nextFrame, std::string &maxString);

uint64_t index_substr(uint64_t i, std::string &maxString);

uint64_t helpToEvict(unsigned int i, string &max_str, uint64_t nextFrame);

word_t evict(std::string max_str, uint64_t page);

word_t helpToRemove(word_t current_frame, std::string &page, int depth, uint64_t index);

uint64_t translate(string &page, string offset, word_t current_frame);

uint64_t toPhysical(uint64_t virtualAddress);

void VMinitialize();

int VMread(uint64_t virtualAddress, word_t* value);

int VMwrite(uint64_t virtualAddress, word_t value);




//////////////////////////////// Implementation of Functions /////////////////////////////////////////////////



void clearTable(uint64_t frameIndex)
{
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMwrite(frameIndex * PAGE_SIZE + i, 0);
	}
}


bitset<VIRTUAL_ADDRESS_WIDTH> toBinary(const uint64_t& virtualAddress)
{
    bitset<VIRTUAL_ADDRESS_WIDTH> binary = std::bitset<VIRTUAL_ADDRESS_WIDTH>(virtualAddress);
    return binary;
}


bitset<OFFSET_WIDTH> toBinary2(const uint64_t& virtualAddress)
{
    bitset<OFFSET_WIDTH> binary = std::bitset<OFFSET_WIDTH>(virtualAddress);
    return binary;
}



std::string addressToBinary(const uint64_t& virtualAddress)
{
    std::string binary = toBinary(virtualAddress).to_string();//to binary
    return binary;
}

std::string addressToBinary2(const uint64_t& virtualAddress)
{
    std::string binary = toBinary2(virtualAddress).to_string();//to binary
    return binary;
}


uint64_t min(uint64_t first , uint64_t second)
{
    return first <= second ? first : second;
}


uint64_t maximalCyclicDistance(uint64_t pageToFill, uint64_t curr_page) {

    auto x = (uint64_t)abs((int)pageToFill - (int)curr_page);
    auto y = (uint64_t)abs((int)pageToFill - (int)curr_page);
    uint64_t get_min = min(NUM_PAGES - x, y);
    return get_min;
}


string sub_string(const string &str, unsigned long start, unsigned long length)
{
    if (start  < 0 ) start+=str.length();
    if (length < 0 ) length=str.length()+length-start;
    if (length < 0 ) return "";
    return str.substr(start,length);
}



uint64_t convertBinaryToDecimal(uint64_t n)
{
    uint64_t decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}


uint64_t strToint(const std::string &page)
{
    std::string::size_type sz = 0;
    uint64_t ull = std::stoull (page,&sz,0);
    uint64_t pageToFill = convertBinaryToDecimal(ull);
    return pageToFill;
}



pageOffsetPair translateToPage(const uint64_t &virtualAddress)
{
    std::string binary = addressToBinary(virtualAddress);
    std::string offset = sub_string(binary, VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH, OFFSET_WIDTH);
    std::string page = sub_string(binary, 0, VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH);
    pageOffsetPair pair = {page, offset};
    return pair ;
}




void read(unsigned int i, uint64_t frame, word_t &val)
{
    PMread(frame * PAGE_SIZE + i, &val);
}

void write(unsigned int i, uint64_t frame)
{
    PMwrite(frame * PAGE_SIZE + i, 0);
}

void evict(uint64_t value, uint64_t max_int){
    PMevict(value, max_int);
}

void restore(uint64_t current_frame, uint64_t pageToFill)
{
    PMrestore(current_frame, pageToFill);
}



void Dfs_travel(uint64_t frame, std::string &root, uint64_t pageToFill, double max, std::string &maxString){
    unsigned int i = 0;
    int value;
    while(i < PAGE_SIZE)
    {
       read(i, frame, value);
        if (value == 0) {
            i++;
            continue;
        }else{
            std::string path = addressToBinary2(i);
            uint64_t index = strToint(root + path);

            double result = maximalCyclicDistance(pageToFill, index);
            double maximum = std::max(result, max);
            if (maximum == result) {
                maxString = root + path;
                max = result;
            }
        }
        i++;
    }
}


void DFS(int depth, uint64_t frame, std::string root, std::string &maxString, uint64_t pageToFill)
{
    int max = -1;
    int val;
    if (depth == TABLES_DEPTH) {
        Dfs_travel(frame, root, pageToFill, max, maxString);
    }
    else {
        for (unsigned int i = 0; i < PAGE_SIZE; i++) {
            read(i, frame, val);
            std::string path = addressToBinary2(i);
            if (val != 0) {
                DFS(depth + 1, (uint64_t) val, root + path, maxString, pageToFill);
            }
        }
    }
}



word_t nextUnusedFrame(uint64_t currentFrame)
{
    for(uint64_t f = 0; f < NUM_FRAMES ; f++)
    {
        uint64_t counter = 0 ;
        for(uint64_t p = 0 ; p < PAGE_SIZE; p++)
        {
            word_t word;
            read(p, f, word);  // check if page in teh PM has a value
            if(word != 0)
            {
                break; // has value this frame is used
            }
            else {
                counter ++;
            }
        }
        if (counter == PAGE_SIZE && f != currentFrame)
        {
            return f;
        }
    }
    return FAILURE; // physical memory is full  need to evict
}



void remove_link(int depth, uint64_t frame, uint64_t value)
{
    if (depth > TABLES_DEPTH) {
        return;
    }
    word_t word;
    for (unsigned int i = 0; i < PAGE_SIZE; i++) {
        read(i, value, word);
        if ((uint64_t) word == frame) {
            write(i, value);
            return;
        }
        if(word != 0)
        {
            remove_link(depth + 1, frame, word);
        }
    }
}


word_t read_evict_write(unsigned int idx, uint64_t nextFrame, std::string &maxString)
{
    word_t value;
    read(idx, nextFrame, value);
    uint64_t max_int = stoull(maxString, nullptr, 2);
    evict(value, max_int);
    clearTable(value);
    write(idx, nextFrame);
    return value;

}


uint64_t index_substr(uint64_t i, std::string &maxString) {
    std::string str = sub_string(maxString, i, OFFSET_WIDTH);
    uint64_t number = std::atoi (str.c_str());
    uint64_t x = convertBinaryToDecimal(number);
    return x ;
}


uint64_t helpToEvict(unsigned int i, string &max_str, uint64_t nextFrame)
{
    uint64_t index = index_substr(i, max_str);
    word_t val;
    read(index, nextFrame, val);
    nextFrame = val;
    return nextFrame;
}


word_t evict(std::string max_str, uint64_t page)
{
    uint64_t nextFrame = 0;
    unsigned int i = 0;
    while(i < page)
    {
        nextFrame = helpToEvict(i, max_str, nextFrame);
        i += OFFSET_WIDTH;
    }
    uint64_t idx = index_substr(page, max_str);
    word_t value = read_evict_write(idx, nextFrame, max_str);
    return value;
}



word_t helpToRemove(word_t current_frame, std::string &page, int depth, uint64_t index){
    std::string max_str;
    word_t nextFrame = nextUnusedFrame(current_frame);
    std::string::size_type sz = 0;
    long long ull = std::stoull (page,&sz,0);
    uint64_t pageToFill = convertBinaryToDecimal(ull);
    if((int)nextFrame == 0)
    {
        DFS(depth, 0, "", max_str, pageToFill);
        uint64_t page_size = max_str.size() - OFFSET_WIDTH;
        nextFrame = evict(max_str, page_size);
    }
    remove_link(1, nextFrame, 0);
    PMwrite(current_frame * PAGE_SIZE + index, nextFrame);
    current_frame = nextFrame;
    return current_frame;
}

uint64_t translate(string &page, string offset, word_t current_frame)
{
    uint64_t pageToFill = stoull(page, nullptr, 2);
    uint64_t offsetToFill = stoull(offset, nullptr, 2);
    restore(current_frame, pageToFill);
    uint64_t result= current_frame * PAGE_SIZE + offsetToFill;
    return result;
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
        read(index, current_frame, val);
        if (val != 0) {
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
    uint64_t result = translate(page, offset, current_frame);
    return result;
}



void VMinitialize() {
	clearTable(0);
}



int VMread(uint64_t virtualAddress, word_t* value) {
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE) {
        return FAILURE;
    }
    PMread(toPhysical(virtualAddress), value);
    return SUCCESS;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE) {
        return FAILURE;
    }
    PMwrite(toPhysical(virtualAddress), value);
    return SUCCESS;
}




