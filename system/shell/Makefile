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
# @date 20 Apr 2021
# @brief File containing Makefile rules to build shell
# @note The tool uses Turbo C library based on DOS (small memory model)

TCPATH=c:\tc
CC=tcc
CFLAGS=-ms -g1 -j1 -c -I$(TCPATH)\include
LD=tlink
LDFLAGS=/L$(TCPATH)\lib
objects=shell.obj
build=..\..\build

shell.exe: clean $(objects)
    $(LD) $(LDFLAGS) c0s.obj \
                     $(build)\shell.obj, \
                     $(build)\$@,, \
                     cs.lib
                     #maths.lib emu.lib

shell.obj: shell.c
    $(CC) $(CFLAGS) -o$(build)\$@ shell.c

clean:
    erase $(build)\shell.exe
    erase $(build)\shell.obj