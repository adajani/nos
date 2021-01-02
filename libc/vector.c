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

/*@file vector.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 1 Jan 2021
* @brief Interrupt vector source file
*/

#include <vector.h>
#include <string.h> /* MK_FP, FP_SEG, FP_OFF */

void setInterruptVector(int interruptNumber, void interrupt (*interruptFunctionPointer)()) {
    unsigned long far *address;
    (void)interruptFunctionPointer;
    if((interruptNumber < 0) || (interruptNumber >= 0x100)) {
        return;
    }

    asm {
        pushf
        cli
    }

    address = (unsigned long far *)MK_FP(0, interruptNumber << 2);
    *address = (unsigned long)MK_FP(FP_SEG(interruptFunctionPointer), FP_OFF(interruptFunctionPointer));

    asm {
        sti
        popf
    }
}

void interrupt (*getInterruptVector (int interruptNumber))(void) {
    unsigned long address;

    if((interruptNumber < 0) || (interruptNumber >= 0x100)) {
        return NULL;
    }

    asm {
        pushf
        cli
    }

    address = *(unsigned long far *)MK_FP(0, interruptNumber << 2);

    asm {
        sti
        popf
    }

  return (void interrupt(*)())address;
}