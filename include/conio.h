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

/*@file conio.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 25 Dec 2020
* @brief Console input/output header file
*/

#ifndef __CONIO_H
    #define __CONIO_H

    #define BLINK_COLOR 128
    #define MAKE_COLOR(BACKGROUND, FORGROUND) (BACKGROUND << 4 | FORGROUND)
    #define CONSOLE_LINE_FEED 10
    #define CONSOLE_CARRIAGE_RETURN 13
    #define CONSOLE_BACKSPACE 8
    
    #define SCREEN_WIDTH 80
    #define SCREEN_HEIGHT 25

    enum COLORS {
        /* dark colors */
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,

        /* light colors */
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE
    };

    enum CursorPosition {
        ROW,
        COLUMN
    };

    void setTextcolor(unsigned char newcolor);
    void setActivePage(unsigned char page);
    void setCursorPosition(unsigned char row, unsigned char column);
    unsigned char getCursorPosition(unsigned char type);
    void clearScreen(void);
    void printCharacter(unsigned char character);
    void printString(char *string);
    char *convertIntegerToString(unsigned int num, int base);
    void printFormat(char* format, ...);
    unsigned char readCharacter(void);
    unsigned char *readString(unsigned char *string);
#endif