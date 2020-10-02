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
ASFLAGS=/zi

objects=N_SCOPY.obj N_LXMUL.obj
build=..\build

all: clean $(objects)
    @

N_SCOPY.obj: N_SCOPY.asm
    $(AS) $(ASFLAGS) N_SCOPY.asm $(build)\N_SCOPY.OBJ

N_LXMUL.obj: N_LXMUL.asm
    $(AS) $(ASFLAGS) N_LXMUL.asm $(build)

clean:
    erase $(build)\N_SCOPY.obj
    erase $(build)\N_LXMUL.obj