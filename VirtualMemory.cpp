#include "VirtualMemory.h"
#include "PhysicalMemory.h"

void clearTable(uint64_t frameIndex) {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}


void VMinitialize() {
    clearTable(0);
}


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









int VMread(uint64_t virtualAddress, word_t* value) {
    return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value) {
    return 1;
}


