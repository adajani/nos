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

/*@file fat12.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 4 Jan 2020
* @brief Fat 12 file system header file
*/

/*
The amount of data clusters is less than 4087 clusters.
a FAT12 file system contains 1.5 bytes per cluster within the file allocation table.
*/

#ifndef __FAT12_H
    #define __FAT12_H
    /* @link: https://www.eit.lth.se/fileadmin/eit/courses/eitn50/Literature/fat12_description.pdf */
    #include <conio.h> / * PRINT_STREAM */

    #define FAT12_DEBUG

    /* @see boot\bpb.inc */
    struct BiosParameterBlock {
        unsigned int bytesPerSector;
        unsigned char sectorsPerCluster;
        unsigned int reservedSectors;
        unsigned char numberOfFATs;
        unsigned int rootEntries;
        unsigned int totalSectors;
        unsigned char media;
        unsigned int sectorsPerFAT;
        unsigned int sectorsPerTrack;
        unsigned int headsPerCylinder;
        unsigned int hiddenSectors;
    };

    /* @see boot\bpb.inc */
    struct BootSector {
        unsigned char oemName[8];
        struct BiosParameterBlock biosParameterBlock;
    };

    enum FileAttribute {
        READ_ONLY = 1,
        HIDDEN = 2,
        SYSTEM = 4,
        VOLUME = 8,
        DIRECTORY = 16,
        ARCHIVE = 32,
    };

    /* The DEL command will replace the first byte of the name
       by 0xe5 to signify "deleted" */
    #define DELETED_FILE 0xE5
    #define FILE_NAME_SIZE 8
    #define FILE_EXTENSION_SIZE 3

    struct FileTime {
        unsigned int doubleSeconds : 5; /* value << 1 */
        unsigned int minutes: 6;
        unsigned int hour : 5;
    };

    struct FileDate {
        unsigned int day : 5;
        unsigned int month: 4;
        unsigned int year : 7; /* since 1980 */
    };

    struct FileInformation {
        unsigned char name[FILE_NAME_SIZE];
        unsigned char extension[FILE_EXTENSION_SIZE];
        unsigned char attributes;
        unsigned char reserved[2];
        struct FileTime creationTime;
        struct FileDate creationDate;
        struct FileDate lastAccessDate;
        unsigned char ignore[2]; /* ignore in FAT12 */
        struct FileTime lastWriteTime;
        struct FileDate lastWriteDate;
        unsigned int firstLogicalCluster;
        unsigned long size; /* in bytes */
    };

    void initializeFileSystem(unsigned char bootDrive);
    void readBootSectorInformation(void);
    void readFATtable(void);
    void readRootEntriesTable(void);
    void printFileName(enum PRINT_STREAM stream, unsigned char far *name, unsigned int size);
    void showDirectory(unsigned char far *rootTable);
    void initializeFATDataAddress(void);
    unsigned int isFileNamesEqual(unsigned char far *fileName1, unsigned char far *fileName2);
    struct FileInformation far *getFileInformation(unsigned char far *rootTable, unsigned char far *fileName);
    void showFile(struct FileInformation far *file);
#endif