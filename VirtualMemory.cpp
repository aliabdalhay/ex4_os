#include "VirtualMemory.h"
#include "PhysicalMemory.h"

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
// maybe give it root ?
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
void virtualToPhysical(uint64_t virtualAddress, uint64_t array[TABLES_DEPTH]) {
	word_t current_address = 0;
	word_t new_address;
	int64_t current_index;
	uint64_t left_number = virtualAddress;
	for(unsigned i = 0; i < TABLES_DEPTH; ++i) {
		current_index = left_number >> (uint64_t )OFFSET_WIDTH * (TABLES_DEPTH-i);
		left_number %= (uint64_t )pow(2, OFFSET_WIDTH * ((uint64_t )TABLES_DEPTH-i));
		PMread(current_index + current_address * PAGE_SIZE, &new_address);

		if(!new_address){
			for(unsigned i = 1; i <NUM_FRAMES; ++i){
				word_t val;
				for(unsigned j = 0; j < PAGE_SIZE; ++j){
					PMread(i * PAGE_SIZE + j, &val)
					if(val != 0 && current_address != i) {
						next_frame = i;
					}
				}
			}
			new_frame = -1
		}

	}
}



void VMinitialize() {
    clearTable(0);
}


int VMread(uint64_t virtualAddress, word_t* value) {
	//Todo

	return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
	//Todo

	return 1;
}


