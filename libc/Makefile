#########################################################################
# Copyright (C) 2020 by Ahmad Dajani                                    #
#                                                                       #
# This file is part of NOS.                                             #
#                                                                       #
# NOS is free software: you can redistribute it and/or modify it        #
# under the terms of the GNU Lesser General Public License as published #
# by the Free Software Foundation, either version 3 of the License, or  #
# (at your option) any later version.                                   #
#                                                                       #
# NOS is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY# without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU Lesser General Public License for more details.                   #
#                                                                       #
# You should have received a copy of the GNU Lesser General Public      #
# License along with NOS.  If not, see <http://www.gnu.org/licenses/>.  #
#########################################################################

# @file Makefile
# @author Ahmad Dajani <eng.adajani@gmail.com>
# @date 26 Dec 2020
# @brief File containing Makefile rules to build libc
# @note The tool chains are 16 bit DOS

CC=tcc
CFLAGS=-1 -K- -k -mt -N- -w -w-amp -g1 -j1 -v -y -c -I..\include -L..\lib
LIB=TLIB
LIBFLAGS=/C
LIBNAME=libc

objects=conio.obj string.obj vector.obj
build=..\build

all: clean $(objects)
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\conio.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\string.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\vector.obj

conio.obj: conio.c
    $(CC) $(CFLAGS) -o$(build)\$@ conio.c

string.obj: string.c
    $(CC) $(CFLAGS) -o$(build)\$@ string.c

vector.obj: vector.c
    $(CC) $(CFLAGS) -o$(build)\$@ vector.c

clean:
    erase $(build)\$(LIBNAME).lib
    erase $(build)\conio.obj
    erase $(build)\string.obj
    erase $(build)\vector.obj