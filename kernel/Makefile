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
STRIP=exe2bin
#0x60 is the kernel load segment (relocate)
STRIPFLAGS=-l=0x60
REN=ren
LIB=TLIB
LIBFLAGS=/C
LIBNAME=kernel

objects=c0t.obj memory.obj service.obj disk.obj fat12.obj main.obj
helper=helper.lib
libc=libc.lib
kernelLib=kernel.lib
build=..\build

kernel.bin: clean $(objects)
    #note: I added kernel into lib to avoid dos limitation (argument too long!)
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\memory.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\service.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\disk.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\fat12.obj

    $(LD) $(LDFLAGS) $(build)\c0t.obj \
                     $(build)\$(helper) \
                     $(build)\$(libc) \
                     $(build)\$(kernelLib) \
                     $(build)\main.obj
    $(STRIP) $(STRIPFLAGS) $(build)\c0t.exe $(build)\$@
    @$(REN) $(build)\c0t.com $(build)\c0t.bin

c0t.obj: c0t.asm
    $(AS) $(ASFLAGS) c0t.asm $(build)\$@

disk.obj: disk.c
    $(CC) $(CFLAGS) -o$(build)\$@ disk.c

fat12.obj: fat12.c
    $(CC) $(CFLAGS) -o$(build)\$@ fat12.c

service.obj: service.c
    $(CC) $(CFLAGS) -o$(build)\$@ service.c

memory.obj: memory.c
    $(CC) $(CFLAGS) -o$(build)\$@ memory.c

main.obj: main.c
    $(CC) $(CFLAGS) -o$(build)\$@ main.c

clean:
    erase $(build)\kernel.bin
    erase $(build)\c0t.obj
    erase $(build)\main.obj
    erase $(build)\memory.obj
    erase $(build)\service.obj
    erase $(build)\disk.obj
    erase $(build)\fat12.obj
    erase $(build)\$(kernelLib)