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

rem # @file run.bat
rem # @author Ahmad Dajani <eng.adajani@gmail.com>
rem # @date 24 Dec 2020
rem # @brief Run NOS using Bochs emulator
rem # @note BXSHARE environment variable that point to emulator directory

set BXSHARE=d:\Bochs-2.6.11

%BXSHARE%\bochs.exe -q -f bochsrc.bxrc
