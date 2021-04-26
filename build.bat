@echo off
rem #########################################################################
rem # Copyright (C) 2020 by Ahmad Dajani                                    #
rem #                                                                       #
rem # This file is part of NOS.                                             #
rem #                                                                       #
rem # NOS is free software: you can redistribute it and/or modify it        #
rem # under the terms of the GNU Lesser General Public License as published #
rem # by the Free Software Foundation, either version 3 of the License, or  #
rem # (at your option) any later version.                                   #
rem #                                                                       #
rem # NOS is distributed in the hope that it will be useful,                #
rem # but WITHOUT ANY WARRANTY# without even the implied warranty of        #
rem # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
rem # GNU Lesser General Public License for more details.                   #
rem #                                                                       #
rem # You should have received a copy of the GNU Lesser General Public      #
rem # License along with NOS.  If not, see <http://www.gnu.org/licenses/>.  #
rem #########################################################################

rem @file build.bat
rem @author Ahmad Dajani <eng.adajani@gmail.com>
rem @date 2 Oct 2020
rem @brief Build the entire source code for NOS project
rem @note This batch will call Makefile in each directory

echo Building NOS

echo **** Tools ****
cd tools
    cd imgwrt
        make
    cd ..
cd ..

echo **** Boot ****
cd boot
    make
cd ..

echo **** Helper ****
cd helper
    make
cd ..

echo **** Libc ****
cd libc
    make
cd ..

echo **** Kernel ****
cd kernel
    make
cd ..

echo **** System ****
cd system
    cd shell
        make
    cd ..
cd ..

echo **** DONE ****