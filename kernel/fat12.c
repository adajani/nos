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

/*@file fat12.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 4 Jan 2020
* @brief Fat 12 file system source file
*/

#include <kernel/fat12.h>
#include <kernel/disk.h> /* DiskOperationLBA */
#include <kernel/memory.h> /* kmalloc */
#include <conio.h> /* printFormat, printCharacter */
#include <string.h> /* movedata, FP_SEG, FP_OFF */

#ifdef FAT12_DEBUG
    #include <kernel/debug.h>
#endif

static unsigned char far *buffer = NULL; /* multi purpose buffer */
static unsigned char far *fatTable = NULL;
static unsigned char far *rootEntriesTable = NULL;
static struct BootSector bootSector;
static unsigned char drive;

static void readBootSectorInformation(void) {
    unsigned char sectorsToRead = 1;
    unsigned int startLogicalBlockAddressing = 0;

    (void)DiskOperationLBA(READ, sectorsToRead, startLogicalBlockAddressing, drive, buffer);
    /* Skip jump instruction while copying sector buffer into structure (+3) */
    movedata(FP_SEG(buffer), FP_OFF(buffer) + 3, _DS, (unsigned)&bootSector, sizeof(struct BootSector));

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Read boot sector information\n");
        printFormat(LOGGER, "\tOemName: %s\n", bootSector.oemName);
        printFormat(LOGGER, "\tBytesPerSector: %d\n", bootSector.biosParameterBlock.bytesPerSector);
        printFormat(LOGGER, "\tSectorsPerCluster: %d\n", bootSector.biosParameterBlock.sectorsPerCluster);
        printFormat(LOGGER, "\tReservedSectors: %d\n", bootSector.biosParameterBlock.reservedSectors);
        printFormat(LOGGER, "\tNumberOfFATs: %d\n", bootSector.biosParameterBlock.numberOfFATs);
        printFormat(LOGGER, "\tRootEntries: %d\n", bootSector.biosParameterBlock.rootEntries);
        printFormat(LOGGER, "\tTotalSectors: %d\n", bootSector.biosParameterBlock.totalSectors);
        printFormat(LOGGER, "\tMedia: 0x%x\n", bootSector.biosParameterBlock.media);
        printFormat(LOGGER, "\tSectorsPerFAT: %d\n", bootSector.biosParameterBlock.sectorsPerFAT);
        printFormat(LOGGER, "\tSectorsPerTrack: %d\n", bootSector.biosParameterBlock.sectorsPerTrack);
        printFormat(LOGGER, "\tHeadsPerCylinder: %d\n", bootSector.biosParameterBlock.headsPerCylinder);
        printFormat(LOGGER, "\tHiddenSectors: %d\n", bootSector.biosParameterBlock.hiddenSectors);
    #endif
}

static void readFATtable(void) {
    size_t fatSize;
    unsigned char sectorsToRead;
    unsigned int startLogicalBlockAddressing;

    fatSize = bootSector.biosParameterBlock.bytesPerSector
              * bootSector.biosParameterBlock.sectorsPerFAT;
    sectorsToRead = (unsigned char)bootSector.biosParameterBlock.sectorsPerFAT;
    startLogicalBlockAddressing = bootSector.biosParameterBlock.reservedSectors;

    fatTable = (unsigned char far *) kmalloc(fatSize);
    memset(fatTable, NULL, fatSize);

    (void)DiskOperationLBA(READ, sectorsToRead, startLogicalBlockAddressing,
                           drive, fatTable);

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Read FAT table\n");
        printFormat(LOGGER, "\tFat size: %d\n", fatSize);
        printFormat(LOGGER, "\tSectors to read: %d\n", sectorsToRead);
        printFormat(LOGGER, "\tstarting lba: %d\n", startLogicalBlockAddressing);
    #endif
}

static void readRootEntriesTable(void) {
    size_t entriesSize;
    unsigned char sectorsToRead;
    unsigned int startLogicalBlockAddressing;

    entriesSize = bootSector.biosParameterBlock.rootEntries * sizeof(struct FileInformation);
    sectorsToRead = entriesSize / bootSector.biosParameterBlock.bytesPerSector;
    startLogicalBlockAddressing = bootSector.biosParameterBlock.numberOfFATs *
                                  bootSector.biosParameterBlock.sectorsPerFAT +
                                  bootSector.biosParameterBlock.reservedSectors;

    rootEntriesTable = (unsigned char far *) kmalloc(entriesSize);
    memset(fatTable, NULL, entriesSize);

    (void)DiskOperationLBA(READ, sectorsToRead, startLogicalBlockAddressing,
                           drive, rootEntriesTable);

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Read root entries table\n");
        printFormat(LOGGER, "\tEntries size: %d\n", entriesSize);
        printFormat(LOGGER, "\tSectors to read: %d\n", sectorsToRead);
        printFormat(LOGGER, "\tstarting lba: %d\n", startLogicalBlockAddressing);
    #endif
}

static void printFileName(enum PRINT_STREAM stream, unsigned char *name, unsigned int size) {
    register unsigned int index;
    for(index = 0; (name[index] != ' ') && (index<size); index++) {
        printCharacter(stream, name[index]);
    }
}

static void showDirectory(unsigned char far *rootTable) {
    struct FileInformation file;
    unsigned int currentOffset = 0;
    unsigned int filesCount = 0;
    unsigned int directoriesCount = 0;

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "showDirectory\n");
    #endif

    while(currentOffset < SECTOR_SIZE && rootTable[currentOffset]) {
        movedata(FP_SEG(rootTable), FP_OFF(rootTable) + currentOffset, _SS, (unsigned)&file, sizeof(struct FileInformation));
        currentOffset += sizeof(struct FileInformation);

        /* don't show */
        if((file.name[0] == DELETED_FILE) || (file.attributes & VOLUME)) {
            continue;
        }

        #ifdef FAT12_DEBUG
            printFileName(LOGGER, file.name, FILE_NAME_SIZE);
        #endif

        printFileName(STDOUT, file.name, FILE_NAME_SIZE);
        if(file.attributes & DIRECTORY) {
            printFormat(STDOUT, " <dir>"); /* size is zero */
            directoriesCount += 1;
        } else {
            printCharacter(STDOUT, '.');
            printFileName(STDOUT, file.extension, FILE_EXTENSION_SIZE);
            printFormat(STDOUT, " <file> %d", file.size);
            filesCount += 1;
            #ifdef FAT12_DEBUG
                printCharacter(LOGGER, '.');
                printFileName(LOGGER, file.extension, FILE_EXTENSION_SIZE);
                printFormat(LOGGER, ", at root entry offset:%d\n", currentOffset);
            #endif
        }

        printFormat(STDOUT, " %d:%d:%d", file.lastWriteTime.hour,
                                         file.lastWriteTime.minutes,
                                         file.lastWriteTime.doubleSeconds * 2);

        printFormat(STDOUT, " %d-%d-%d\n", file.lastWriteDate.day,
                                           file.lastWriteDate.month,
                                           file.lastWriteDate.year + 1980);
    }
    printFormat(STDOUT, "files=%d, directories=%d\n", filesCount, directoriesCount);
}

void initializeFileSystem(unsigned char bootDrive) {
    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Initialize file system\n");
    #endif

    buffer = (unsigned char far *) kmalloc(SECTOR_SIZE);
    drive = bootDrive;

    initializeDisk(drive);
    readBootSectorInformation();
    readFATtable();
    readRootEntriesTable();

    // test pwd
    showDirectory(rootEntriesTable);

    while(1); //TODO
}