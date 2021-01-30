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
#include <kernel/splash.h> /* showSplashScreen */
#include <conio.h> /* printFormat */
#include <string.h> /* memset, size_t */

extern unsigned int _heapStart; /* @see c0t.asm */
extern unsigned char bootDrive; /* @see c0t.asm */

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

    showSplashScreen();
    initializeMemory(_heapStart);
    initializeFileSystem(bootDrive);
    initializeInterrupt();

    //TODO: load MZ files and execute shell

#if 0
    /* create MCB */
    printFormat(STDOUT, "Memory test\n");
    for(i=0; i<9; i++) {
        address = (unsigned char far *) kmalloc(0xffffL);
        if(!address) {
            printFormat(LOGGER, "Error: no memory\n");
            asm hlt;
        }
        printFormat(STDOUT, "Allocate 0xffff bytes @ %x:%x\n", FP_SEG(address), FP_OFF(address));
        memset(address, 'a' + i, 0xffff);
    }

    address = (unsigned char far *) kmalloc(0x98d4);
    if(!address) {
        printFormat(LOGGER, "\nError: no memory");
        asm hlt;
    }
    printFormat(STDOUT, "Allocate 0x98d4 @ %x:%x\n", FP_SEG(address), FP_OFF(address));
    memset(address, '$', 0x98d4);

    /* Free */
    printFormat(STDOUT, "Free address %x:%x\n", FP_SEG(address), FP_OFF(address));
    kfree(address);

    address = (unsigned char far *) kmalloc(0x200);
    if(!address) {
        printFormat(LOGGER, "\nError: no memory");
        asm hlt;
    }
    printFormat(STDOUT, "Allocate 0x200 @ %x:%x\n", FP_SEG(address), FP_OFF(address));
    memset(address, '!', 0x200);

    //interrupt
    printFormat(STDOUT, "Testing interrupt:\n");
    _AX = 0xaabb;
    asm {
        int KERNEL_INTERRUPT
    }
    printFormat(STDOUT, "Interrupt return value %x\n", _CX);


    /* Main memory tester */
    while(1) {
        printFormat(STDOUT, "\nEnter segment in hex:");
        p = readString(buffer);
        segment = convertHexStringToInteger(p);

        printFormat(STDOUT, "\nEnter offset in hex:");
        p = readString(buffer);
        offset = convertHexStringToInteger(p);

        printFormat(STDOUT, "\nEnter value in hex:");
        p = readString(buffer);
        value = convertHexStringToInteger(p);

        printFormat(STDOUT, "\nEnter size in hex:");
        p = readString(buffer);
        size = convertHexStringToInteger(p);

        address = MK_FP(segment, offset);
        memset(address, value, size);
        printFormat(STDOUT, "\n-------> DONE");

        if(p[0]=='x') break;
    }
    printFormat(STDOUT, "\nBye:)\n");
#endif
    asm hlt
}
