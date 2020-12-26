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

/*@file stdarg.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 25 Dec 2020
* @brief x86 Bios definitions header file
*/

#ifndef __BIOS_H
    #define __BIOS_H

    #define CALL_VIDEO_BIOS() asm int 0x10
    #define CALL_MEMORY_BIOS() asm int 0x12
    #define CALL_DISKETTE_BIOS() asm int 0x13
    #define CALL_KEYBOARD_BIOS() asm int 0x16
#endif