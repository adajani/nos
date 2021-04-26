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
#include <kernel/splash.h> /* showSplashScreen */
#include <kernel/disk.h> /* initializeDisk */
#include <kernel/fat12.h> /* initializeFAT12 */
#include <kernel/filesys.h> /* initializeFileSystem */
#include <kernel/exec.h> /* executeBinary */
#include <conio.h> /* printFormat */
#include <string.h> /* memset, size_t */

extern unsigned int _heapStart; /* @see c0t.asm */
extern unsigned char bootDrive; /* @see c0t.asm */

void main() {
    int returnValue;
    showSplashScreen();
    initializeMemory(_heapStart);
    initializeDisk(bootDrive);
    initializeFAT12(bootDrive);
    initializeFileSystem(bootDrive);
    initializeInterrupt();

    returnValue = executeBinary("/system     /shell   exe");
    printFormat(STDOUT, "\nfinish, returned value=%d", returnValue);

    while(1);
}
