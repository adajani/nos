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

/*@file conio.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 25 Dec 2020
* @brief Console input/output source file
*/

#include <bios.h>
#include <stdarg.h>
#include <conio.h>

static unsigned char consoleTextColor = 7;
static unsigned char consoleActivePage = 0;

void setTextcolor(unsigned char newcolor) {
    //color attribute background | forground
    consoleTextColor = newcolor;
}

void setActivePage(unsigned char page) {
    consoleActivePage = page;
}

void setCursorPosition(unsigned char row, unsigned char column) {
    _AH = 2;
    _BH = consoleActivePage;
    _DH = row;
    _DL = column;
    CALL_VIDEO_BIOS();
}

unsigned char getCursorPosition(unsigned char type) {
    _AH = 3;
    _BH = consoleActivePage;
    CALL_VIDEO_BIOS();
    if(type == ROW) {
        return _DH;
    }
    return _DL;
}

void clearScreen(void) {
    _AX = 0x0600;
    _BH = consoleTextColor;
    _CX = 0x0000; /*ch=row of upper left corner */
                  /*cl=column of upper left corner */
    _DX = ((SCREEN_HEIGHT - 1) << 8) | (SCREEN_WIDTH - 1);
    CALL_VIDEO_BIOS();
    setCursorPosition(0, 0);
}

/* @note: Console tab is treated as space! */
void printCharacter(enum PRINT_STREAM stream, unsigned char character) {
    switch(stream) {
        case STDOUT:
            _AH = 0xe;
            _AL = character;
            _BH = consoleActivePage;
            CALL_VIDEO_BIOS();
            if(character == CONSOLE_LINE_FEED) {
                /* convert \n into \n\r */
                _AL = CONSOLE_CARRIAGE_RETURN;
                CALL_VIDEO_BIOS();
            }
            break;

        case LOGGER:
            outPortByte(0xe9, character);
            break;
    }
}

void printString(enum PRINT_STREAM stream, char *string) {
    while(*string) {
        printCharacter(stream, *string++);
    }
}

unsigned char readCharacter(void) {
    _AH = 0;
    CALL_KEYBOARD_BIOS();
    return _AL;
}

int convertStringToInteger(char *string) {
    int result = 0;
    int sign = 1;

    if(*string=='-') {
        sign = -1;
        string++;
    }

    while((*string) && (*string >= '0') && (*string <= '9')) {
        result = (result * 10) + ((*string) - '0');
        string++;
    }
    return sign * result;
}

unsigned int convertHexStringToInteger(unsigned char *hexNumber) {
    unsigned int result = 0;
    while(*hexNumber) {
        unsigned char byte = *hexNumber++;
        if(byte >= '0' && byte <= '9') {
            byte = byte - '0';
        }
        else if((byte >= 'a' || byte >= 'A') && (byte <='f' || byte <='F')){
            byte = byte - 'a' + 10;
        }
        result = (result << 4) | (byte & 0xF);
    }
    return result;
}

char *convertIntegerToString(unsigned int num, int base) {
    /* e.g itoa */
    #define MAX_CONVERT_BUFFER 5 /* max integer is 65535, ffff */
    static char lookup[] = {"0123456789abcdef"};
    static char buffer[MAX_CONVERT_BUFFER + 1] = {NULL}; /* plus null*/
    char *ptr;

    ptr = &buffer[sizeof(buffer) - 1];
    *ptr = NULL;

    do {
        *--ptr = lookup[num % base];
        num /= base;
    } while(num != NULL);

    return ptr;
}

void printFormat(enum PRINT_STREAM stream, char* format, ...) {
    register char *character;
    register char *string;
    register int integer;

    va_list arg;
    va_start(arg, format);

    for(character = format; *character != NULL; character++) {
        if(*character != '%') {
            printCharacter(stream, *character);
        }
        else {
            character++; /* skip % */
            switch(*character) {
                case 'c' :  integer = va_arg(arg, char);
                            printCharacter(stream, (char)integer);
                            break;

                case 's':   string = va_arg(arg, char *);
                            printString(stream, string);
                            break;

                case 'd' :  integer = va_arg(arg, int);
                            if(integer < 0) {
                                integer = -integer;
                                printCharacter(stream, '-');
                            }
                            printString(stream, convertIntegerToString(integer, 10));
                            break;

                case 'o':   integer = va_arg(arg, unsigned int);
                            printString(stream, convertIntegerToString(integer, 8));
                            break;


                case 'x':   integer = va_arg(arg, unsigned int);
                            printString(stream, convertIntegerToString(integer, 16));
                            break;
            }
        }
    }

    va_end(arg);
}

unsigned char *readString(unsigned char *string) {
    unsigned int actualLength = 0;
    unsigned int maximumLength;
    unsigned char *stringPointer;
    unsigned char character=0;
    int currentColumn;
    int currentRow;
    if (!string) {
        return NULL;
    }

    maximumLength = (unsigned char)string[0];
    if(maximumLength < 1) {
        return NULL;
    }

    stringPointer = &string[2];

    while(1) {
        character = readCharacter();

        if (character == CONSOLE_BACKSPACE) {
            if (actualLength == 0) {
                continue; /* nothing to erase */
            }

            currentColumn = getCursorPosition(COLUMN);
            currentRow = getCursorPosition(ROW);

            currentColumn--;
            if(currentColumn < 0) {
                currentColumn = SCREEN_WIDTH - 1;
                currentRow--;
            }

            setCursorPosition(currentRow, currentColumn);
            printCharacter(STDOUT, ' ');
            setCursorPosition(currentRow, currentColumn);

            actualLength--;
            stringPointer[actualLength] = NULL;
        }
        else if (character == CONSOLE_CARRIAGE_RETURN) {
            stringPointer[actualLength] = NULL;
            break;
        }
        else if(character >=' ' && character <= '~') {
            if(actualLength < maximumLength) {
                printCharacter(STDOUT, character);
                stringPointer[actualLength] = character;
                actualLength++;
            }
        }
    }

    string[1] = (unsigned char)actualLength;
    return stringPointer;
}

unsigned char inPortByte(unsigned int portNumber) {
    _DX = portNumber;
    _AX = 0;
    asm in al, dx
    return _AL;
}

unsigned int inPortWord(unsigned int portNumber) {
    _DX = portNumber;
    _AX = 0;
    asm in ax, dx
    return _AX;
}

void outPortByte(unsigned int portNumber, unsigned char value) {
    _DX = portNumber;
    _AL = value;
    asm out dx, al
}

void outPortWord(unsigned int portNumber, unsigned int value) {
    _DX = portNumber;
    _AX = value;
    asm out dx, ax
}