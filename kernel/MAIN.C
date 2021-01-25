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
* @date 2 Oct 2020
* @brief Main kernel
* @note maximum file size is 32 sectors (16KB)
* @see c0t.asm
*/
#include <kernel/memory.h> /* kmalloc, kfree */
#include <kernel/service.h> /* NOS_INTR, initializeInterrupt */
#include <kernel/fat12.h> /* initializeFileSystem */
#include <kernel/disk.h> /* initializeDisk */
#include <kernel/version.h> /* MAJOR_VERSION, MINOR_VERSION, COPY_RIGHT */
#include <conio.h> /* printFormat */
#include <string.h> /* memset, size_t */

extern unsigned int _heapStart; /* file: c0t.asm */
extern unsigned char bootDrive; /* file: c0t.asm */

void main() {
#if 0
    #define SIZE 100
    unsigned char *p;
    unsigned char far *address = NULL;
    unsigned int segment = 0;
    unsigned int offset = 0;
    char value = 'a';
    size_t size = 0;
    int i;
    /*                   size  actual  buffer  null */
    unsigned char buffer[1+    1+      SIZE+      1 ]={SIZE};
#endif
    unsigned char far *buffer = NULL;
    int status;
    unsigned int lba;
    unsigned char numberOfSectors = 1;

    printFormat("Starting NOS v%d.%d, %s\n", MAJOR_VERSION, MINOR_VERSION, COPY_RIGHT);
    initializeMemory(_heapStart);
    initializeInterrupt();
    initializeDisk(bootDrive);
    initializeFileSystem();

    buffer = (unsigned char far *) kmalloc(numberOfSectors * SECTOR_SIZE);
    printFormat("Buffer @ %x:%x\n", FP_SEG(buffer), FP_OFF(buffer));

    /* 1. read */
    lba = 0;
    printFormat("Reading %d sector(s) at lba %d (boot sector)\n", numberOfSectors, lba);
    status = DiskOperationLBA(READ, numberOfSectors, lba, bootDrive, buffer);
    if(status == FAILURE) {
        printFormat("Read Error\n");
    } else {
        printFormat("Read success\n");
        printFormat("Check read buffer: ");
        if(buffer[0]==0xeb && buffer[1]==0x3c &&
           buffer[510]==0x55 && buffer[511]==0xaa
        ) {
            printFormat("pass\n");
        } else {
            printFormat("fail\n");
        }
    }

    /* 2. write & read */
    lba = 100;
    memset(buffer,'+', SECTOR_SIZE);
    buffer[511] = '-';

    printFormat("Writing %s sector(s) at lba %d\n", numberOfSectors, lba);
    status = DiskOperationLBA(WRITE, numberOfSectors, lba, bootDrive, buffer);
    if(status == FAILURE) {
        printFormat("Write Error\n");
    } else {
        printFormat("Write success\n");
        memset(buffer, NULL, 512);
        printFormat("Read back lba %d\n", lba);
        status = DiskOperationLBA(READ, numberOfSectors, lba, bootDrive, buffer);
        if(status == FAILURE) {
            printFormat("Read back Error\n");
        } else {
            printFormat("Read back success\n");
            printFormat("Check read buffer: ");
            if(buffer[0] == '+' && buffer[200] == '+' && buffer[511] == '-') {
                printFormat("pass\n");
            } else {
                printFormat("fail\n");
            }
        }
    }

#if 0
    /* create MCB */
    printFormat("Memory test\n");
    for(i=0; i<9; i++) {
        address = (unsigned char far *) kmalloc(0xffffL);
        if(!address) {
            printFormat("Error: no memory\n");
            asm hlt;
        }
        printFormat("Allocate 0xffff bytes @ %x:%x\n", FP_SEG(address), FP_OFF(address));
        memset(address, 'a' + i, 0xffff);
    }

    address = (unsigned char far *) kmalloc(0x98d4);
    if(!address) {
        printFormat("\nError: no memory");
        asm hlt;
    }
    printFormat("Allocate 0x98d4 @ %x:%x\n", FP_SEG(address), FP_OFF(address));
    memset(address, '$', 0x98d4);

    /* Free */
    printFormat("Free address %x:%x\n", FP_SEG(address), FP_OFF(address));
    kfree(address);

    address = (unsigned char far *) kmalloc(0x200);
    if(!address) {
        printFormat("\nError: no memory");
        asm hlt;
    }
    printFormat("Allocate 0x200 @ %x:%x\n", FP_SEG(address), FP_OFF(address));
    memset(address, '!', 0x200);

    //interrupt
    printFormat("Testing interrupt:\n");
    _AX = 0xaabb;
    asm {
        int KERNEL_INTERRUPT
    }
    printFormat("Interrupt return value %x\n", _CX);


    /* Main memory tester */
    while(1) {
        printFormat("\nEnter segment in hex:");
        p = readString(buffer);
        segment = convertHexStringToInteger(p);

        printFormat("\nEnter offset in hex:");
        p = readString(buffer);
        offset = convertHexStringToInteger(p);

        printFormat("\nEnter value in hex:");
        p = readString(buffer);
        value = convertHexStringToInteger(p);

        printFormat("\nEnter size in hex:");
        p = readString(buffer);
        size = convertHexStringToInteger(p);

        address = MK_FP(segment, offset);
        memset(address, value, size);
        printFormat("\n-------> DONE");

        if(p[0]=='x') break;
    }
    printFormat("\nBye:)\n");
#endif
    asm hlt
}
