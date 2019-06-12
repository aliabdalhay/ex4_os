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
#include <cmath>
#include <algorithm>

#include "VirtualMemory.h"
#include "PhysicalMemory.h"


#define TWO 2
#define ZERO 0


#define MAX_CYCLIC_DISTANCE_VAL 0
#define MAX_CYCLIC_DISTANCE_FRAME 1
#define MAX_FRAME 2
#define EMPTY_PAGE_ADDRESS 3
#define EMPTY_FRAME_ADDRESS 4
#define EMPTY_FRAME_ADDRESS_FATHER 5
#define MAX_CYCLIC_DISTANCE_PAGE 6
#define MAX_CYCLIC_DISTANCE_PAGE_FATHER 7

void clearTable(uint64_t frameIndex) {
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMwrite(frameIndex * PAGE_SIZE + i, 0);
	}
}

/*
 * <page, offset>
 * this pair is binary of an address page and offset
 */
typedef std::pair<std::string , std::string>  pageOffsetPair;


int log2(int number)
{
	return static_cast<int>(log(number) / log(TWO));
}


/*
 * log(pagesize)
 * return the number of digits needed for the offset
 */
uint64_t offSetSize(uint64_t pageSize);

/*
 * log virtualsize - ofsetsize
 * return the number of digits for the oage size
 */
uint64_t pageSize(uint64_t virtualSize, uint64_t ofsetsize);


/*
 * this would git an integer address and return the binary equivalent
 * number of digits in the binary should be equal to log (VM Size)
 */

std::string addressToBinary(const uint64_t& virtualAddress);


pageOffsetPair translateToPage(const uint64_t& virtualAddress);
/*
 * gets a virtual addressa dn return a pair which is a binary represntation of teh adrees
 * devided to the page and offset
 */


void DFS(std::string page, uint64_t frame, int currentDepth);


/*void DFS(std::string page, uint64_t frame, int currentDepth)
 * traversing over the tress, DFS  and returns the umber of fram that was not used
 * retrunz zero if the physical memory was full aka all frames were used
 * a positave number of the frame if that frame was never used
 * a negative number of the frame if that frame was used yet empty ( have to delete its parent
 * pointing at it)
 */

//Todo

uint64_t nextUnusedFrame(int currentFrame);


//probably create a map that contains all the pages that were read or written ....
uint64_t  maximalCyclicDistance(uint64_t pageToFill,  uint64_t allUsedPages[]);
/*
 * map < key = page umbers, value =  physical address>  every thing we write or read to/ from
 * return the page number with the inimal cyclic distance from the pageToFill
 */


std::string checks_divisibility(std::string binary){
	uint64_t binary_offset = binary.size() % OFFSET_WIDTH;
	while(binary_offset != 0)
	{
		binary = '0' + binary;
	}
	return binary;
}



int number_of_pages(){
	int offset_table = OFFSET_WIDTH * TABLES_DEPTH;
	int pages = pow(2, offset_table);
	return pages;
}


/*
 * maps page for pair to frame then connects it to offset and returns that value which is the
 * physical address
 */

uint64_t toPhysical(uint64_t virtualAddress){
	pageOffsetPair pair = translateToPage(virtualAddress);
	int current_frame = 0;
	int numberOfPages = number_of_pages();
	unsigned int i = 0;
	std::string page = pair.first;
	std::string offset = pair.second;
	while (i != page.size())
	{
		std::basic_string<char, std::char_traits<char>, std::allocator<char>> sub_page = page.substr(i, OFFSET_WIDTH);
		uint64_t index = (uint64_t) stoi(sub_page, nullptr, 2);
		word_t val;
        PMread(index + current_frame * PAGE_SIZE, &val);

        if(val == true)
        {
            current_frame = val;
            continue;
        }
        else
        {
            uint64_t next_frame = nextUnusedFrame(current_frame);
            if(next_frame == 0)
            {
                //DFS()
            }
            //delete_link()
            PMwrite(current_frame * PAGE_SIZE + index, next_frame);
            current_frame = next_frame;

        }

	}

}







/*
 * if physical memory full get frame with max cyclic distance then deletes that frame delete link
 * to it and creates it with empty valued to where i need to add it
 */

int evict(uint64_t newAddress, uint64_t* allUsedPages[]);



void deleteFrame(int frameToDelete, uint64_t numberOfPage, uint64_t allUsedPages[]);


void createFrame(uint64_t frameToCreate, uint64_t numberOfPage, uint64_t allUsedPages[]);
/*
 * initlized teh frame ( puts zero in the physical memory) should be added to all usedpages
 */


void VMinitialize() {
	clearTable(0);
}


std::string addressToBinary(const uint64_t& virtualAddress)
{
//	int virtualSize = log2((int)VIRTUAL_MEMORY_SIZE);
	std::string binary = std::bitset<VIRTUAL_ADDRESS_WIDTH>(virtualAddress).to_string();//to binary
	return binary;
}

//uint64_t offSetSize(uint64_t pageSize)
//{
//	return (uint64_t) log2((int)pageSize);
//}
//
//uint64_t pageSize(uint64_t virtualSize, uint64_t ofsetsize)
//{
//	return (uint64_t) log2((int)virtualSize) - ofsetsize;
//}




pageOffsetPair translateToPage(const uint64_t &virtualAddress)
{
	std::string binary = addressToBinary(virtualAddress);
	binary = checks_divisibility(binary);
	std::string offset = binary.substr(VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH, OFFSET_WIDTH);
	std::string page = binary.substr(0, VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH);
	pageOffsetPair pair = {page, offset};
	return pair ;
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



void createFrame(uint64_t frameToCreate,uint64_t numberOfPage, uint64_t *allUsedPages)
{
	allUsedPages[numberOfPage] = frameToCreate;
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		PMwrite(frameToCreate * PAGE_SIZE + i, ZERO);
	}
}

void deleteFrame(uint64_t frameToDelete, uint64_t numberOfPage, uint64_t *allUsedPages)
{
	allUsedPages[numberOfPage] = ZERO;
	for(int i = 0; i < PAGE_SIZE; i++)
	{
		/*
		 * todo check what exactly teh evect does does it have all the frame,
		 * either with the for an delete each one of teh physica memory or no for at all
		 */
		PMevict(frameToDelete, numberOfPage);
	}
}


void maximalCyclicDistance(uint64_t pageToFill, uint64_t curr_page, uint64_t frame, uint64_t addr[8]) {
    uint64_t get_min = min(NUM_PAGES - std::abs(pageToFill - curr_page), std::abs(pageToFill - curr_page));
    if(addr[MAX_CYCLIC_DISTANCE_VAL] <= get_min){
        addr[MAX_CYCLIC_DISTANCE_FRAME] = frame;
        addr[MAX_CYCLIC_DISTANCE_PAGE] = curr_page;
        addr[MAX_CYCLIC_DISTANCE_VAL] = get_min;
    }

}




void DFS(uint64_t pageNumber, uint64_t frame, int currentDepth, uint64_t pageToFill, uint64_t addr[8])
{
    word_t val;
    int count = 0;
    for( int i = 0; i< PAGE_SIZE ; i++) {
        PMread(frame * PAGE_SIZE + i, &val);
//        uint64_t unused = nextUnusedFrame(frame);
//        PMrestore(unused, pageNumber);
//        std::string newAddressBin = addressToBinary(unused) + pair.second;
//        auto newAddress = static_cast<uint64_t>(std::stoi(newAddressBin, nullptr, 2));

        if (currentDepth == TABLES_DEPTH) {
            maximalCyclicDistance(pageToFill, pageNumber, frame, addr);
            return;
        }

        if (addr[MAX_FRAME] < (uint64_t) val) {
            addr[MAX_FRAME] = (uint64_t) val;
        }

        if (val != 0) {
            DFS((uint64_t) val, currentDepth + 1, (pageNumber << OFFSET_WIDTH) + i, pageToFill, addr);
            if (addr[EMPTY_FRAME_ADDRESS] == (uint64_t) val) {
                addr[EMPTY_FRAME_ADDRESS_FATHER] = frame;
            }
            if (addr[MAX_CYCLIC_DISTANCE_FRAME] == (uint64_t) val) {
                addr[MAX_CYCLIC_DISTANCE_PAGE_FATHER] = frame;
            }
        } else {
            count++;
        }
    }
    if(count == PAGE_SIZE){
        if ((pageToFill >> (OFFSET_WIDTH * ( TABLES_DEPTH - currentDepth) )) != pageNumber) {
            addr[EMPTY_PAGE_ADDRESS] = pageNumber;
            addr[EMPTY_FRAME_ADDRESS] = frame;
        }
    }

}

uint64_t min(uint64_t first , uint64_t second)
{
	return first <= second ? first : second;
}


















int VMread(uint64_t virtualAddress, word_t* value) {
	//Todo

	return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
	//Todo

	return 1;
}