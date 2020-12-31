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

/*@file kmem.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 31 Dec 2020
* @brief Kernel memory header file
*/

#ifndef __KMEM_H
    #define __KMEM_H

    /* #define KMEM_DEBUG */

    #ifndef NULL
        #define NULL 0
    #endif

    #define KMALLOC_PRIME_MAGIC 59473U

    struct MemoryControlBlock {
        unsigned int isInitialized : 1;
        unsigned int isAvailable : 1;
        unsigned int magic;
        unsigned long size;
    };

    unsigned long getLastValidAddress(void);
    void far *convertLinearAddressToFarPointer(unsigned long address);
    void initializeMemory(unsigned int heapStart);
    void far *kmalloc(unsigned long size);
    void kfree(void far *address);
#endif