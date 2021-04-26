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

/*@file imgwrt.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 20 Apr 2021
* @brief Shell
* @description Command line interface
*/
#if 0
#define KERNEL_INTERRUPT 87

/* near pointer print */
void print(char *message) {
    _ES = _DS;
    _BX = (int)message;
    asm int KERNEL_INTERRUPT
}
#endif
int main(int argc, char* argv[]) {
    unsigned char far *screen = (unsigned char far *)0xb80000000L;
    unsigned char *message = "Direct Screen message";
    const unsigned char color = 0x1e;
    unsigned int index = 0;
    int exit=0;
    (void)argc;
    (void)argv;

    _AX=3; asm int 0x10
    while(*message) {
        screen[ index * 2     ] = *message++;
        screen[(index++ * 2) + 1] = color;
    }

    while(!exit);

    return 2021;
}