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
* @note As version 0.0.1 this file only prints a message VRAM (WIP)
* @see c0t.asm
*/

extern int _heap_start;

void main() {
    unsigned char far *v= (unsigned char far *)0xb8000000L;
    char msg[]={"Welcome to NOS :)"};
    int i, ptr=0;

    _AX=3;
    asm int 0x10

    for(i=0; i<sizeof(msg); i++) {
        v[ptr] = msg[i];
        v[ptr+1] = 0x1e;
        ptr+=2;
    }

    _AX=0;
    asm int 0x16
}
