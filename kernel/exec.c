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

/*@file exec.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 20 Apr 2021
* @brief Execute binary files source file
*/

#include <kernel/exec.h> /*  */
#include <kernel/filesys.h> /* fopen, fclose, loadFile */
#include <string.h> /* NULL */
#include <kernel/memory.h> /* kmalloc */
#ifdef EXEC_DEBUG
    #include <kernel/debug.h>
#endif

/* single task process execution 
   @return <0 error message
           >=0 process return value
*/
int executeBinary(char *path) {
    static unsigned int _cs_, _ip_, _ss_, _sp_;
    static unsigned char far *buffer = NULL;
    struct ExecutableFile far *header = NULL;
    int value = 0;
    struct File far *file;
    unsigned int imageBase;
    unsigned long align;
    unsigned int far *addressFixup;
    struct RelocationTable far *relocationTable;

    #ifdef EXEC_DEBUG
    printFormat(LOGGER, "executeBinary\n");
    #endif

    file = fopen(path);
    if(!file) {
        #ifdef EXEC_DEBUG
        printFormat(LOGGER, "\tFile not found\n");
        #endif
        return -1;
    }

    /* size is sector aligned */
    align = SECTOR_SIZE - (file->size % SECTOR_SIZE);
    buffer = (unsigned char far *)kmalloc_align(file->size + align);

    /* load the file into allocated buffer */
    loadFile(file, buffer);

    #ifdef EXEC_DEBUG
    printFormat(LOGGER, "\tEXE located @ %x:%x\n", FP_SEG(buffer), FP_OFF(buffer));
    #endif

    header = (struct ExecutableFile far *)buffer;
    if(header->signature != EXE_SIGNATURE) {
        printFormat(STDOUT, "\tNot valid exe header\n");
        fclose(file);
        kfree(buffer);
        return -2;
    }
    //fclose(file);

    #ifdef EXEC_DEBUG
    printFormat(LOGGER, "\tEXE: relocation\n");
    #endif
    /* exe relocation */
    imageBase = FP_SEG(buffer) + header->headerSize;
    while(header->relocationItems--) {
        relocationTable = (struct RelocationTable far *)MK_FP(FP_SEG(buffer), header->relocationItemOffset);
        relocationTable->segment += imageBase;

        addressFixup = (unsigned int far *)MK_FP(relocationTable->segment, relocationTable->offset);
        #ifdef EXEC_DEBUG
        printFormat(LOGGER, "\tItem #%d @ %x:%x=%x,",
                    header->relocationItems, relocationTable->segment,relocationTable->offset, *addressFixup);
        #endif
        *addressFixup += imageBase;
        #ifdef EXEC_DEBUG
        printFormat(LOGGER, "address fixup=%x\n", *addressFixup);
        #endif
        /* Advanced to the next item */
        header->relocationItemOffset += sizeof(struct RelocationTable);
    }
    header->stackSegment += imageBase;
    header->codeSegment += imageBase;

    _cs_ = header->codeSegment;
    _ip_ = header->instructionPointer;
    _ss_ = header->stackSegment;
    _sp_ = header->stackPointer;

    #ifdef EXEC_DEBUG
    printFormat(LOGGER, "\tExe segments: ss:%x, sp:%x, cs:%x, ip:%x\n",
                _ss_, _sp_, _cs_, _ip_);
    DebugBreak();
    #endif

    /* TODO: store returned address */
    FAR_JUMP(_ss_, _sp_, _cs_, _ip_);

    /* kfree(buffer); */
    return value;
}