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

rem @file set_path.bat
rem @author Ahmad Dajani <eng.adajani@gmail.com>
rem @date 2 Oct 2020
rem @brief Add Turbo C compiler path into DOS path environment 
rem @note call this batch file only once

@echo off
echo Set NOS Path
set path=%path%;c:\tc\bin
set build_dir=build