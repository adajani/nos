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

#ifndef __FAT12_H
    #define __FAT12_H
    /* @link: https://www.eit.lth.se/fileadmin/eit/courses/eitn50/Literature/fat12_description.pdf */

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

    struct FileInformation {
        unsigned char fileName[8];
        unsigned char extension[3];
        unsigned char attributes;
        unsigned char reserved[2];
        unsigned int creationTime;
        unsigned int creationDate;
        unsigned int lastAccessDate;
        unsigned char ignore[2]; /* ignore in FAT12 */
        unsigned int lastWriteTime;
        unsigned int lastWriteDate;
        unsigned int firstLogicalCluster;
        unsigned long fileSize; /* in bytes */
    };

    void initializeFileSystem(unsigned char bootDrive);
    void readBootSectorInformation(void);
    void readFATtable(void);
    void readRootEntriesTable(void);
#endif