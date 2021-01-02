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

/*@file service.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 2 Jan 2020
* @brief Kernel interrupt service source file
* @see c0t.asm
*/
#include <kernel/service.h>
#include <conio.h> /* printFormat */
#include <vector.h> /* setInterruptVector */

void initializeInterrupt(void) {
    setInterruptVector(KERNEL_INTERRUPT, kernelInterruptHandler);
}

#pragma argsused
static void interrupt kernelInterruptHandler(unsigned int BP, unsigned int DI, unsigned int SI, unsigned int DS,
                                             unsigned int ES, unsigned int DX, unsigned int CX, unsigned int BX,
                                             unsigned int AX, unsigned int IP, unsigned int CS, unsigned int FLAGS) {
    printFormat("Custome interrupt ax=%x\n", AX);
    CX = 0x1234;
}