/************************************************************************
* Copyright (C) 2020 by Ahmad Dajani                                    *
*                                                                       *
* This file is part of NOS.                                             *
*                                                                       *
* NOS is free software: you can redistribute it and/or modify it        *
* under the terms of the GNU Lesser General Public License as published *
* by the Free Software Foundation, either version 3 of the License, or  *
* (at your option) any later version.                                   *
*                                                                       *
* NOS is distributed in the hope that it will be useful,                *
* but WITHOUT ANY WARRANTY* without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU Lesser General Public License for more details.                   *
*                                                                       *
* You should have received a copy of the GNU Lesser General Public      *
* License along with NOS.  If not, see <http://www.gnu.org/licenses/>.  *
************************************************************************/

/*@file splash.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 30 Jan 2020
* @brief Kernel splash screen source file
* @see include\version.h
*/

#include <kernel/splash.h>
#include <conio.h> /* printFormat */
#include <kernel/version.h> /* MAJOR_VERSION, MINOR_VERSION, COPY_RIGHT */

static void NOSLogo(void) {
    /*
        @link: https://en.wikipedia.org/wiki/Hypsilophodon
        @note: 1. Hypsilophodon is an ornithischian dinosaur genus from the Early
                  Cretaceous period of England. 
               2. Mass: 20 kg
               3. Lived: 145 million years ago 
               4. Length: 1.8 m
    */
    printFormat(STDOUT, "\
                                  ___......__             _\n\
                              _.-'           ~-_       _.=a~~-_\n\
        --=====-.-.-_----------~   .--.       _   -.__.-~ ( ___==>\n\
                    '''--...__  (    \\ \\\\\\ { )       _.-~\n\
                              =_ ~_  \\\\-~~~//~~~~-=-~\n\
                               |-=-~_ \\\\   \\\\\n\
                               |_/   =. )   ~}\n\
                               |}      ||\n\
                              //       ||\n\
                            _//        {{\n\
                         '='~'          \\\\_\n\
                                         ~~'\n");
}

void showSplashScreen(void) {
    printFormat(STDOUT, "Starting NOS " CODE_NAME " v%d.%d, build " __DATE__ "\n%s\n",
                        MAJOR_VERSION, MINOR_VERSION, COPY_RIGHT);
    NOSLogo();
}