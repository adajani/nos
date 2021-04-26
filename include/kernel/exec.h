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

/*@file exec.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 20 Apr 2021
* @brief Execute binary files header
*/

#ifndef __EXEC_H
    #define __EXEC_H
    #define EXEC_DEBUG

    #define EXE_SIGNATURE 0x5a4d

    /*@note: all registers should be stored in data segment before
             far jump, because we are changing the stack.
    */
    #define FAR_JUMP(_ss_, _sp_, _cs_, _ip_) do { \
        asm { mov ss, word ptr _ss_ }\
        asm { mov sp, word ptr _sp_ }\
        asm { push    word ptr _cs_ }\
        asm { push    word ptr _ip_ }\
        asm { DB 0xCB               }\
        } while(0);

    struct ExecutableFile {
        unsigned int signature; /* MZ */
        unsigned int imageLength; /* mod 512 */
        unsigned int fileSize; /* in 512 byte pages */
        unsigned int relocationItems;
        unsigned int headerSize; /* in 16 byte paragraphs */
        unsigned int minParagraphs;
        unsigned int maxParagraphs;
        unsigned int stackSegment; /* in paras */
        unsigned int stackPointer;
        unsigned int checksum; /* negative of pgm */
        unsigned int instructionPointer;
        unsigned int codeSegment;
        unsigned int relocationItemOffset;
        unsigned int overlayNumber; /* 0 for root program */
    };

    struct RelocationTable {
        unsigned int offset;
        unsigned int segment;
    };

    int executeBinary(char *path);
#endif