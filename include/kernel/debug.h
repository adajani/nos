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

/*@file debug.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 28 Jan 2021
* @brief Debugging macros for Bochs emulator
*/

#ifndef __KERNEL_DEBUG_H
    #define __KERNEL_DEBUG_H
    #include <conio.h> /* outPortWord */

    /* Stops simulation and breaks into the debug console */
    #define DebugBreak() outPortWord(0x8A00, 0x8A00); outPortWord(0x8A00, 0x08AE0);
#endif