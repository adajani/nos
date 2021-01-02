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
# @brief File containing Makefile rules to build Trubo C helper functions
# @note This build is dedicated for Turbo C

AS=tasm
ASFLAGS=/zi /d__TINY__
LIB=TLIB
LIBFLAGS=/C
LIBNAME=helper

objects=F_LXMUL.obj F_SCOPY.obj FTOL.obj H_LDIV.obj \
        H_LLSH.obj H_LRSH.obj H_LURSH.obj H_SPUSH.obj \
        N_LXMUL.obj N_SCOPY.obj 
build=..\build

all: clean $(objects)
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\F_LXMUL.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\F_SCOPY.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\FTOL.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\H_LDIV.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\H_LLSH.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\H_LRSH.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\H_LURSH.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\H_SPUSH.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\N_LXMUL.obj
    $(LIB) $(LIBFLAGS) $(build)\$(LIBNAME) +$(build)\N_SCOPY.obj

F_LXMUL.obj: F_LXMUL.asm
    $(AS) $(ASFLAGS) F_LXMUL.asm $(build)

F_SCOPY.obj: F_SCOPY.asm
    $(AS) $(ASFLAGS) F_SCOPY.asm $(build)

FTOL.obj: FTOL.asm
    $(AS) $(ASFLAGS) FTOL.asm $(build)

H_LDIV.obj: H_LDIV.asm
    $(AS) $(ASFLAGS) H_LDIV.asm $(build)

H_LLSH.obj: H_LLSH.asm
    $(AS) $(ASFLAGS) H_LLSH.asm $(build)

H_LRSH.obj: H_LRSH.asm
    $(AS) $(ASFLAGS) H_LRSH.asm $(build)

H_LURSH.obj: H_LURSH.asm
    $(AS) $(ASFLAGS) H_LURSH.asm $(build)

H_SPUSH.obj: H_SPUSH.asm
    $(AS) $(ASFLAGS) H_SPUSH.asm $(build)

N_LXMUL.obj: N_LXMUL.asm
    $(AS) $(ASFLAGS) N_LXMUL.asm $(build)

N_SCOPY.obj: N_SCOPY.asm
    $(AS) $(ASFLAGS) N_SCOPY.asm $(build)\N_SCOPY.OBJ

clean:
    erase $(build)\$(LIBNAME).lib
    erase $(build)\F_LXMUL.obj
    erase $(build)\F_SCOPY.obj
    erase $(build)\FTOL.obj
    erase $(build)\H_LDIV.obj
    erase $(build)\H_LLSH.obj
    erase $(build)\H_LRSH.obj
    erase $(build)\H_LURSH.obj
    erase $(build)\H_SPUSH.obj
    erase $(build)\N_LXMUL.obj
    erase $(build)\N_SCOPY.obj