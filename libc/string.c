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

/*@file string.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 28 Dec 2020
* @brief String source file
*/

#include <string.h>


void memset(void far *address, char value, size_t size) {
    asm push es
    _ES = FP_SEG(address);
    _DI = FP_OFF(address);
    _CX = size;
    _AL = value;
    
    asm {
        cld
        rep stosb
        pop es
    }
}

void movedata(unsigned SourceSegment, unsigned SourceOffset,
              unsigned DestinationSegment, unsigned DestinationOffset, size_t size) {
    asm {
        push ds
        push es
    }
    _CX = size;
    _DS = SourceSegment;
    _SI = SourceOffset;
    _ES = DestinationSegment;
    _DI = DestinationOffset;

    asm {
        cld
        rep movsb
        pop es
        pop ds
    }
}

unsigned char convertCharacterToLowerCase(unsigned char character) {
    if( (character >= 'A') && (character <= 'Z')) {
        return character + 32;
    }
    return character;
}