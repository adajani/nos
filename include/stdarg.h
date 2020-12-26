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
* @brief Standard argument header file
*/

#ifndef __STDARG_H
    #define __STDARG_H

    #ifndef NULL
        #define NULL 0
    #endif

    typedef void *va_list;

    #define __size(x)           ((sizeof(x)+sizeof(int)-1) & ~(sizeof(int)-1))
    #define va_start(ap, parmN) ((void)((ap) = (va_list)((char *)(&parmN)+__size(parmN))))
    #define va_arg(ap, type)    (*(type *)(((*(char **)&(ap))+=__size(type))-(__size(type))))
    #define va_end(ap)          ((void)NULL)
#endif