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
#include <string.h> /* movedata, FP_SEG, FP_OFF, convertCharacterToLowerCase */

#ifdef FAT12_DEBUG
    #include <kernel/debug.h>
#endif

static unsigned char far *buffer = NULL; /* multi purpose buffer */
static unsigned char far *fatTable = NULL;
static unsigned char far *rootEntriesTable = NULL;
static struct BootSector bootSector;
static unsigned char drive;
static unsigned dataStartAddress = 0; /* linear address */

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
        printFormat(LOGGER, "\tEntries size in bytes: %d\n", entriesSize);
        printFormat(LOGGER, "\tSectors to read: %d\n", sectorsToRead);
        printFormat(LOGGER, "\tstarting lba: %d\n", startLogicalBlockAddressing);
    #endif
}

static void initializeFATDataAddress(void) {
    dataStartAddress = bootSector.biosParameterBlock.bytesPerSector *
                      (bootSector.biosParameterBlock.reservedSectors +
                       bootSector.biosParameterBlock.sectorsPerFAT *
                       bootSector.biosParameterBlock.numberOfFATs +
                       (bootSector.biosParameterBlock.rootEntries *
                       sizeof(struct FileInformation) +
                       bootSector.biosParameterBlock.bytesPerSector - 1)
                       / bootSector.biosParameterBlock.bytesPerSector);
}

static void printFileName(enum PRINT_STREAM stream, unsigned char far *name, unsigned int size) {
    register unsigned int index;
    for(index = 0; (name[index] != ' ') && (index<size); index++) {
        printCharacter(stream, name[index]);
    }
}

static unsigned int isFileNamesEqual(unsigned char far *fileName1, unsigned char far *fileName2) {
    /* @note 1. File name is 11 bytes (8 + 3)
             2. The first byte of a name must not be space (0x20)
             3. Short names or extensions are padded with spaces.
             4. Special ASCII characters  are not allowed 0x22 ("), 0x2a (*), 0x2b (+), 0x2c (,), 0x2e (.), 0x2f (/),
                0x3a (:), 0x3b (;), 0x3c (<), 0x3d (=), 0x3e (>), 0x3f (?), 0x5b ([), 0x5c (\), 0x5d (]), 0x7c (|)
    */
    register unsigned int index;
    for (index = 0; index<(FILE_NAME_SIZE + FILE_EXTENSION_SIZE); index++) {
        if( convertCharacterToLowerCase(fileName1[index]) !=
            convertCharacterToLowerCase(fileName2[index])) {
            return 0;
        }
    }
    return 1;
}

static struct FileInformation far *getFileInformation(unsigned char far *rootTable, unsigned char far *fileName) {
    struct FileInformation far *currentFile;
    unsigned int currentOffset = 0;
    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "getFileInformation: [%s],", fileName);
    #endif

    while(currentOffset < bootSector.biosParameterBlock.rootEntries &&
          rootTable[currentOffset] != NULL) {

        currentFile = (struct FileInformation far *)MK_FP(FP_SEG(rootTable), FP_OFF(rootTable) + currentOffset);
        currentOffset += sizeof(struct FileInformation);

        /* don't show */
        if((currentFile->name[0] == DELETED_FILE) || (currentFile->attributes & VOLUME)) {
            continue;
        }

        /* @note: taking advantage of buffer overflow, we can check the extentions also */
        if(isFileNamesEqual(currentFile->name, fileName)) {
            #ifdef FAT12_DEBUG
                printFormat(LOGGER, "found at lba=%d\n", currentFile->firstLogicalCluster);
            #endif
            return currentFile;
        }
    }

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "not found\n");
    #endif
    return NULL; /* file not found */
}

static void showFile(struct FileInformation far *file) {
    unsigned int startByte;
    unsigned int startLogicalBlockAddressing;
    register unsigned long index;
    register unsigned long sectorsToRead;
    register unsigned long bytesCount;
    register unsigned long remainBytes;

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "showFile: ");
    #endif

    if(!file) {
        printFormat(LOGGER, " exit early (NULL)\n");
        return;
    }

    if(file->attributes & DIRECTORY) {
        #ifdef FAT12_DEBUG
            printFormat(LOGGER, " exit early (directory)\n");
        #endif
        return;
    }

    startByte = dataStartAddress + (file->firstLogicalCluster - 2) *
                bootSector.biosParameterBlock.bytesPerSector;
    startLogicalBlockAddressing = startByte / SECTOR_SIZE;

    sectorsToRead = file->size / SECTOR_SIZE;
    remainBytes = file->size % SECTOR_SIZE;

    #ifdef FAT12_DEBUG
        printFileName(LOGGER, file->name, 11);
        printFormat(LOGGER, ", @ lba=%d\n", startLogicalBlockAddressing);
    #endif

    /* Edge case when file size is less that the sector */
    if( (file->size < SECTOR_SIZE) && (sectorsToRead == 0) ) {
        sectorsToRead = 1;
        bytesCount = file->size;
    } else {
        bytesCount = SECTOR_SIZE;
    }

    while(sectorsToRead) {
        (void)DiskOperationLBA(READ, 1 /* one sector */, startLogicalBlockAddressing, drive, buffer);
        #ifdef FAT12_DEBUG
            printFormat(LOGGER, "  reading %d bytes\n", bytesCount);
        #endif

        for(index=0; index<bytesCount; index++) {
            printCharacter(STDOUT, buffer[(unsigned)index]);
        }

        sectorsToRead -= 1;
        startLogicalBlockAddressing += 1;
    }

    if(remainBytes) {
        (void)DiskOperationLBA(READ, 1 /* one sector */, startLogicalBlockAddressing, drive, buffer);
        #ifdef FAT12_DEBUG
            printFormat(LOGGER, "  reading remain %d bytes\n", remainBytes);
        #endif

        for(index=0; index<remainBytes; index++) {
            printCharacter(STDOUT, buffer[(unsigned)index]);
        }
    }
}

static void showDirectory(unsigned char far *rootTable) {
    struct FileInformation far *file;
    unsigned int currentOffset = 0;
    unsigned int filesCount = 0;
    unsigned int directoriesCount = 0;

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "showDirectory\n");
    #endif

    while(currentOffset < bootSector.biosParameterBlock.rootEntries &&
          rootTable[currentOffset] != NULL) {

        file = (struct FileInformation far *)MK_FP(FP_SEG(rootTable), FP_OFF(rootTable) + currentOffset);
        currentOffset += sizeof(struct FileInformation);

        /* don't show */
        if((file->name[0] == DELETED_FILE) || (file->attributes & VOLUME)) {
            continue;
        }

        #ifdef FAT12_DEBUG
            printFileName(LOGGER, file->name, FILE_NAME_SIZE);
        #endif

        printFileName(STDOUT, file->name, FILE_NAME_SIZE);
        if(file->attributes & DIRECTORY) {
            printFormat(STDOUT, " <dir>"); /* size is zero */
            directoriesCount += 1;
        } else {
            printCharacter(STDOUT, '.');
            printFileName(STDOUT, file->extension, FILE_EXTENSION_SIZE);
            printFormat(STDOUT, " <file> %d", file->size);
            filesCount += 1;
            #ifdef FAT12_DEBUG
                printCharacter(LOGGER, '.');
                printFileName(LOGGER, file->extension, FILE_EXTENSION_SIZE);
                printFormat(LOGGER, ", file size:%d", file->size);
            #endif
        }
        #ifdef FAT12_DEBUG
            printFormat(LOGGER, ", at root entry offset:%d, file lba:%d\n",
                        currentOffset - sizeof(struct FileInformation),
                        file->firstLogicalCluster);
        #endif

        printFormat(STDOUT, " %d:%d:%d", file->lastWriteTime.hour,
                                         file->lastWriteTime.minutes,
                                         file->lastWriteTime.doubleSeconds * 2);

        printFormat(STDOUT, " %d-%d-%d\n", file->lastWriteDate.day,
                                           file->lastWriteDate.month,
                                           file->lastWriteDate.year + 1980);
    }
    printFormat(STDOUT, "files=%d, directories=%d\n", filesCount, directoriesCount);
}

void initializeFileSystem(unsigned char bootDrive) {
    #ifdef FAT12_DEBUG
        struct FileInformation far *file;
        char fileName[]={"FILE7   TXT"};
        printFormat(LOGGER, "Initialize file system\n");
    #endif

    buffer = (unsigned char far *) kmalloc(SECTOR_SIZE);
    drive = bootDrive;

    initializeDisk(drive);
    readBootSectorInformation();
    readFATtable();
    readRootEntriesTable();
    initializeFATDataAddress();

    /* Testing */
    showDirectory(rootEntriesTable);

    file = getFileInformation(rootEntriesTable, (unsigned char far*)MK_FP(FP_SEG(fileName),FP_OFF(fileName)));
    if(!file) {
        printFormat(STDOUT, "Error: file not found\n");
        while(1); //TODO
    }
    printFormat(STDOUT, "show file contents:\n");
    showFile(file);

    while(1); //TODO
}