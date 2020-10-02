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
# @date 2 Oct 2020
# @brief File containing Makefile rules to build 16 bit OS kernel
# @note The tool chains are 16 bit DOS
# @note c0t.obj must be linked as the first module

CC=tcc
CFLAGS=-1 -K- -k -mt -N- -w -w-amp -g1 -j1 -v -y -c -I..\include -L..\lib
AS=tasm
ASFLAGS=/zi
LD=tlink
LDFLAGS=/v /k
STRIP=tdstrip
STRIPFLAGS=-c
REN=ren

objects=c0t.obj main.obj
build=..\build

kernel.bin: clean $(objects)
    $(LD) $(LDFLAGS) $(build)\c0t.obj \
                     $(build)\N_SCOPY.obj \
                     $(build)\main.obj
    $(STRIP) $(STRIPFLAGS) $(build)\c0t.exe $(build)\$@
    @$(REN) $(build)\c0t.com $(build)\c0t.bin

c0t.obj: c0t.asm
    $(AS) $(ASFLAGS) c0t.asm $(build)\$@

main.obj: main.c
    $(CC) $(CFLAGS) -o$(build)\$@ main.c 

clean:
    erase $(build)\c0t.obj
    erase $(build)\main.obj