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
# @date 26 Jan 2021
# @brief File containing Makefile rules to build image writer tool
# @note The tool uses Turbo C library based on DOS (small memory model)

TCPATH=c:\tc
CC=tcc
CFLAGS=-ms -g1 -j1 -c -I$(TCPATH)\include
LD=tlink
LDFLAGS=/L$(TCPATH)\lib
objects=imgwrt.obj
build=..\..\build

imgwrt.exe: clean $(objects)
    $(LD) $(LDFLAGS) c0s.obj \
                     $(build)\imgwrt.obj, \
                     $(build)\$@,, \
                     cs.lib maths.lib emu.lib

imgwrt.obj: imgwrt.c
    $(CC) $(CFLAGS) -o$(build)\$@ imgwrt.c

clean:
    erase $(build)\imgwrt.exe
    erase $(build)\imgwrt.obj