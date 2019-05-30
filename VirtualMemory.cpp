#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>




#define TWO 2;

void clearTable(uint64_t frameIndex) {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}
//Todo
typedef <uint64_t page, uint64_t offSet> struct pageOffsetPair;
/*
 * this pair is binary of an address page and pffset
 */
 uint64_t log2(uint64_t number)
 {
 	return ( log(number) / log(TWO) );
 }

//todo
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
//Todo

uint64_t addressToBinary(const uint64_t& virtualAddress);
/*
 * this would git an integer address and return the binary equivalent
 * number of digits in the binary should be equal to log (VM Size)
 */

//Todo

pageOffset translateToPage(const uint64_t& virtualAddress);
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
uint64_t minimalCyclicDistance(uint64_t pageToFill);
/*
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
 * if physical memory full get fram with max cyclic distance then deletes that fram delete link
 * to it and creates it with empty valued to where i need to add it
 */

int deleteFrame();

int createFrame();

void VMinitialize() {
    clearTable(0);
}

uint64_t addressToBinary(const uint64_t& virtualAddress)
{
    std::string binary = std::bitset<VIRTUAL_ADDRESS_WIDTH>(virtualAddress).to_string(); //to binary
    return binary;
}

int VMread(uint64_t virtualAddress, word_t* value) {
	//Todo

	return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
	//Todo

	return 1;
}


