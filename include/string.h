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

/*@file string.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 28 Dec 2020
* @brief String header file
*/

#ifndef __STRING_H
    #define __STRING_H

    #ifndef NULL
        #define NULL 0
    #endif

    #ifndef _SIZE_T
        #define _SIZE_T
        typedef unsigned size_t;
    #endif

    #define MK_FP( seg,ofs )( (void _seg * )( seg ) +( void near * )( ofs ))
    #define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
    #define FP_OFF( fp )( (unsigned )( fp ))

    #define peekw( a,b )  ( *( (int  far* )MK_FP( (a ),( b )) ))
    #define peekb( a,b )  ( *( (char far* )MK_FP( (a ),( b )) ))
    #define pokew( a,b,c )( *( (int  far* )MK_FP( (a ),( b )) ) =( int )( c ))
    #define pokeb( a,b,c )( *( (char far* )MK_FP( (a ),( b )) ) =( char )( c ))

    void memset(void far *address, char value, size_t size);
    void movedata(unsigned SourceSegment, unsigned SourceOffset,
                  unsigned DestinationSegment, unsigned DestinationOffset, size_t size);
    unsigned char convertCharacterToLowerCase(unsigned char character);
#endif