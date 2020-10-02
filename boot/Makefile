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
# @brief File containing Makefile rules to build stage 0 boot loader
# @note The tool chains are 16 bit DOS
# @note Create floppy image using *nix command: mkfs.msdos -h 32 -R 32 ./floppya.img
# @note Copy boot.bin into first sector of floppy image

AS=tasm
ASFLAGS=/zi
LD=tlink
LDFLAGS=/v /k
STRIP=tdstrip
STRIPFLAGS=-c
REN=ren

objects=boot.obj
build=..\build

boot.bin: clean $(objects)
    @$(LD) $(LDFLAGS) $(build)\$(objects)
    @$(STRIP) $(STRIPFLAGS) $(build)\boot.exe
    @$(REN) $(build)\boot.com $(build)\boot.bin

boot.obj: boot.asm
    $(AS) $(ASFLAGS) boot.asm $(build)\boot.obj

clean:
    erase $(build)\$(objects)