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
 * return the number of digits needed fro the ofset
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

//todo
// maybe give it root ?  maybe create a DFS function alone
int64_t nextUnusedFram();
/*
 * traversing over the tress, DFS  and returns the umber of fram that was not used
 * retrunz zero if the physical memory was full aka all frames were used
 * a positave number of the fram if that fram was never used
 * a negative number of the fram if that fram was used yet empty ( have to delete its parent
 * pointing at it)
 */

//Todo

//probably create a map that contains all the pages that were read or written ....
uint64_t  maximalCyclicDistance(uint64_t pageToFill,  uint64_t allUsedPages[VIRTUAL_MEMORY_SIZE]);
/*
 * map < key = page umbers, value =  physical address>  every thing we write or read to/ from
 * return the page number with the inimal cyclic distance from the pageToFill
 */


//todo
uint64_t toPhysical(pageOffsetPair pair);
/*
 * maps page fro pair to frame then connects it to ofset adn returns that value which is teh
 * physical address
 */


int evict(uint64_t newAddress);
/*
 * if physical memory full get frame with max cyclic distance then deletes that frame delete link
 * to it and creates it with empty valued to where i need to add it
 */

int deleteFrame();

int createFrame();

void VMinitialize() {
    clearTable(0);
}

std::string addressToBinary(const uint64_t& virtualAddress)
{
//	int virtualSize = log2((int)VIRTUAL_MEMORY_SIZE);
	std::string binary = std::bitset<VIRTUAL_ADDRESS_WIDTH>(virtualAddress).to_string();//to binary
    return binary;
}

uint64_t offSetSize(uint64_t pageSize)
{
	return (uint64_t) log2((int)pageSize);
}

uint64_t pageSize(uint64_t virtualSize, uint64_t ofsetsize)
{
	return (uint64_t) log2((int)virtualSize) - ofsetsize;
}

pageOffsetPair translateToPage(const uint64_t &virtualAddress)
{
	std::string binary = addressToBinary(virtualAddress);
	std::string offset = binary.substr(VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH, OFFSET_WIDTH);
	std::string page = binary.substr(0, VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH);
	pageOffsetPair pair = {page, offset};
	return pair ;
}

 uint64_t min(uint64_t first , uint64_t second)
{
	return first < second? first: second;
}
//todo check size and stuff about array
uint64_t maximalCyclicDistance(int pageToFill, int allUsedPages[VIRTUAL_MEMORY_SIZE] )
{
	int maxIndex = 0;
	uint64_t max = 0;
	for (int i =0 ; i < VIRTUAL_MEMORY_SIZE; i++)
	{
		uint64_t minmal = min(static_cast<uint64_t>(abs(pageToFill - allUsedPages[i])),
							  static_cast<uint64_t>(abs(VIRTUAL_MEMORY_SIZE -
														(pageToFill- allUsedPages[i]))));

		if ( minmal > max)
		{
			max = minmal;
			maxIndex = i;
		}
	}
	return static_cast<uint64_t>(maxIndex);
}

int VMread(uint64_t virtualAddress, word_t* value) {
	//Todo

	return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
	//Todo

	return 1;
}


