#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <map>
#include <vector>


#define TWO 2
#define ZERO 0

void clearTable(uint64_t frameIndex) {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}
typedef std::pair<std::string , std::string>  pageOffsetPair;
/*
 * <page, offset>
 * this pair is binary of an address page and offset
 */
 int log2(int number)
 {
 	return static_cast<int>(log(number) / log(TWO));
 }


uint64_t offSetSize(uint64_t pageSize);
/*
 * log(pagesie)
 * return the number of digits needed for the ofset
 */

uint64_t pageSize(uint64_t virtualSize, uint64_t ofsetsize);
/*
 * log virtualsize - ofsetsize
 * return the number of digits for the oage size
 */

std::string addressToBinary(const uint64_t& virtualAddress);
/*
 * this would git an integer address and return the binary equivalent
 * number of digits in the binary should be equal to log (VM Size)
 */


pageOffsetPair translateToPage(const uint64_t& virtualAddress);
/*
 * gets a virtual addressa dn return a pair which is a binary represntation of teh adrees
 * devided to the page and offset
 */

void DFS(std::string page, uint64_t frame, int currentDepth);
/*
 *
 */


uint64_t nextUnusedFram(int currentFrame);
/*void DFS(std::string page, uint64_t frame, int currentDepth)
 * traversing over the tress, DFS  and returns the umber of fram that was not used
 * retrunz zero if the physical memory was full aka all frames were used
 * a positave number of the frame if that frame was never used
 * a negative number of the frame if that frame was used yet empty ( have to delete its parent
 * pointing at it)
 */

//Todo

//probably create a map that contains all the pages that were read or written ....
uint64_t  maximalCyclicDistance(uint64_t pageToFill,  uint64_t allUsedPages[]);
/*
 * map < key = page umbers, value =  physical address>  every thing we write or read to/ from
 * return the page number with the inimal cyclic distance from the pageToFill
 */


//todo
uint64_t toPhysical(pageOffsetPair pair);
/*
 * maps page for pair to frame then connects it to offset and returns that value which is the
 * physical address
 */


int evict(uint64_t newAddress, uint64_t* allUsedPages[]);
/*
 * if physical memory full get frame with max cyclic distance then deletes that frame delete link
 * to it and creates it with empty valued to where i need to add it
 */

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
	std::string offset = binary.substr(VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH, OFFSET_WIDTH);
	std::string page = binary.substr(0, VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH);
	pageOffsetPair pair = {page, offset};
	return pair ;
}

uint64_t nextUnusedFram(uint64_t currentFrame)
{
	for( uint64_t f = 0; f < NUM_FRAMES ; f++)
	{
		uint64_t counter = 0 ;
		for( uint64_t p = 0 ; p < PAGE_SIZE; p++)
		{
			word_t  read;
			PMread(f * PAGE_SIZE +p, &read); // check if page in teh PM has a value
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

void DFS(pageOffsetPair pair, uint64_t frame, int currentDepth, uint64_t* allUsedPages)
{
	if ( currentDepth == TABLES_DEPTH)
	{
		//todo
		return;
	}
	word_t value;
	uint64_t counter = 0;

	for(uint64_t p = 0; p < PAGE_SIZE; p++)
	{
		PMread(frame * PAGE_SIZE + currentDepth, &value);
		if( value != 0 )
		{
			
		}
	}

//	auto pageNumber = static_cast<uint64_t>(std::stoi(pair.first, nullptr, 2));
//	if(currentDepth == TABLES_DEPTH)
//	{
//		//todo write whats in vm to PM by the offset
//		//we got to the page now we fill it
//		for( int i = 0; i< PAGE_SIZE ; i++)
//		{
//			uint64_t unused = nextUnusedFram(frame);
//			PMrestore(unused, pageNumber);
//			std::string newAddressBin = addressToBinary(unused) + pair.second;
//			auto newAddress = static_cast<uint64_t>(std::stoi(newAddressBin, nullptr, 2));
//
//
//
//			std::string vmaddress = (pair.first + pair.second);
//			int vmAdd = std::stoi(vmaddress, nullptr, 2);
////			word_t word;
////			VMread(static_cast<uint64_t>(vmAddress), &word );
////			PMwrite(fram* PAGE_SIZE + i + pair.second, &word);
//
//		}
//	}
//	else
//	{
//		char pageStep =  pair.first[currentDepth];
//		int index = (int)(pageStep);
//		int word;
//		PMread(frame * PAGE_SIZE + index, &word);
//		if (word!=0)
//		{ // then there is aframe in the  next lyer in tree where i need to go
//			DFS(pair, (uint64_t) word, currentDepth+1 , allUsedPages);
//		}
//		else
//		{ // no frame in the tree where ineed to go
//			uint64_t unused = nextUnusedFram(frame);
//			if (unused != 0)
//			{// then we have an unused frame
//				createFrame(unused, pageNumber, allUsedPages );
//				PMwrite( frame* PAGE_SIZE + index,(word_t) unused);
//				DFS(pair, unused, currentDepth + 1, allUsedPages );
//
//			}
//			else{
//				//evict();
//			}
//		}
//
//	}

}

uint64_t min(uint64_t first , uint64_t second)
{
	return first < second? first: second;
}
//todo check size and stuff about array
uint64_t maximalCyclicDistance(int pageToFill, const int allUsedPages[] )
{
	int maxDistancePage = 0;
	uint64_t max = 0;
	for (int i = 0 ; i < NUM_PAGES; i++)
	{
		if (allUsedPages[i] != ZERO)
		{
			uint64_t minmal = min(static_cast<uint64_t>(abs(pageToFill - i)),
								  static_cast<uint64_t>(abs(NUM_PAGES -
															(pageToFill- i))));

			if ( minmal > max)
			{
				max = minmal;
				maxDistancePage = i;
			}
		}

	}
	return static_cast<uint64_t>(maxDistancePage);
}

int VMread(uint64_t virtualAddress, word_t* value) {
	//Todo

	return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
	//Todo

	return 1;
}


