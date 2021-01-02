/************************************************************************
* Copyright (C) 2020 by Ahmad Dajani                                    *
*                                                                       *
* This file is part of NOS.                                             *
*                                                                       *
* NOS is free software: you can redistribute it and/or modify it        *
* under the terms of the GNU Lesser General Public License as published *
* by the Free Software Foundation, either version 3 of the License, or  *
* (at your option) any later version.                                   *
*                                                                       *
* NOS is distributed in the hope that it will be useful,                *
* but WITHOUT ANY WARRANTY* without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU Lesser General Public License for more details.                   *
*                                                                       *
* You should have received a copy of the GNU Lesser General Public      *
* License along with NOS.  If not, see <http://www.gnu.org/licenses/>.  *
************************************************************************/

/*@file main.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 31 Oct 2020
* @brief Kernel memory source file
* @see c0t.asm
*/

#include <kernel/memory.h>
#include <bios.h> /* CALL_MEMORY_BIOS */
#include <math.h> /* abs */
#include <conio.h> /* printFormat */
#include <string.h> /* memset */

unsigned long startAddress = NULL;
unsigned long lastValidAddress = NULL;
unsigned long initializedAddress = NULL;

unsigned long getLastValidAddress(void) {
    CALL_MEMORY_BIOS();
    /* Operation     Notes
        (_AX <<  (6   paragraph
                +4))  linear address (*16)
                -1    the address above video memory (0xa0000)
    */
    return ((unsigned long)_AX << 10) - 1;
}

void far *convertLinearAddressToFarPointer(unsigned long address) {
    /* normalized address (20 bits) = segment (16 bits) : offset (4 bits) */
    unsigned int segment;
    unsigned int offset;
    offset = (unsigned int)(address & 0xfL);
    segment = (unsigned int)(address >> 4L);
    return (void far *)MK_FP(segment, offset);
}

void initializeMemory(unsigned int heapStart) {
    /*
    on computer restart, the memory will still have 
    data. This function clear the conventional memory by setting NULL value
    */
    register unsigned int nextChunk;
    unsigned int chunkSize;
    unsigned int remainChunkSize;
    unsigned long currentAddress;
    unsigned long totalMemory;

    startAddress = (unsigned long)((_CS << 4) + heapStart);
    lastValidAddress = getLastValidAddress();
    initializedAddress = startAddress; /* used in kmalloc to track the new regions*/

    totalMemory = (lastValidAddress - startAddress) + 1;

    chunkSize = (unsigned int)(totalMemory / 0xffffL);
    remainChunkSize = (unsigned int)(totalMemory % 0xffffL);

    #ifdef KMEM_DEBUG
    printFormat("initialize memory:\n");
    printFormat("\tKernel heap start @ %x:%x\n", _CS, heapStart);
    printFormat("\tprobe %d chuncks with size %x. remainChunkSize is %x\n", chunkSize, 0xffff, remainChunkSize);
    #endif

    currentAddress = startAddress;
    for(nextChunk=0; nextChunk<chunkSize; nextChunk++) {
        memset(convertLinearAddressToFarPointer(currentAddress), NULL, 0xffff);
        currentAddress += 0xffffL;
    }
    if(remainChunkSize) {
        memset(convertLinearAddressToFarPointer(currentAddress), NULL, remainChunkSize);
    }
}

/*
    - all available memory can be allocated
    - blocks larger than 64k can be allocated
*/
void far *kmalloc(unsigned long size) {
    unsigned long currentAddress = NULL;
    struct MemoryControlBlock far *currentMemoryControlBlock = NULL;
    void far *newAddress = NULL;

    size += sizeof(struct MemoryControlBlock);

    /* Try to allocate new MCB struct */
    if(abs((long)lastValidAddress - (long)initializedAddress) >= size) {
        currentMemoryControlBlock = (struct MemoryControlBlock far *)initializedAddress;
        currentMemoryControlBlock->isInitialized = 1;
        currentMemoryControlBlock->isAvailable = 0;
        currentMemoryControlBlock->size = size;
        currentMemoryControlBlock->magic = KMALLOC_PRIME_MAGIC;

        #ifdef KMEM_DEBUG
        printFormat("kmalloc: create new struct\n");
        #endif

        newAddress = convertLinearAddressToFarPointer(initializedAddress + sizeof(struct MemoryControlBlock));

        initializedAddress += size;
        return newAddress;
    }

    /* If MCB already created then try to utilize existing struct */
    currentAddress = startAddress;
    while(currentAddress != lastValidAddress) {
        currentMemoryControlBlock = (struct MemoryControlBlock far *)currentAddress;

        if(currentMemoryControlBlock->isAvailable &&
           currentMemoryControlBlock->size >= size) {
            currentMemoryControlBlock->isAvailable = 0;
            currentMemoryControlBlock->size = size;

            #ifdef KMEM_DEBUG
            printFormat("kmalloc: use struct\n");
            #endif

            currentAddress += sizeof(struct MemoryControlBlock);
            return convertLinearAddressToFarPointer(currentAddress);
        }
        currentAddress += currentMemoryControlBlock->size;
    }

    /* TODO: can't find continuous empty slot, so try to merge slots into bigger one */

    /* Sorry: no more memory for you :( */
    return NULL;
}

void kfree(void far *address) {
    struct MemoryControlBlock far *currentMemoryControlBlock = NULL;
    unsigned long linearAddress = (unsigned long)address;

    if(!address) {
        return;
    }

    linearAddress -= sizeof(struct MemoryControlBlock);

    currentMemoryControlBlock = (struct MemoryControlBlock far *)linearAddress;

    if(currentMemoryControlBlock->magic != KMALLOC_PRIME_MAGIC) {
        #ifdef KMEM_DEBUG
        printFormat("kfree: invalid MCB header\n");
        #endif
        return;
    }

    currentMemoryControlBlock->isAvailable = 1;
}