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

static unsigned char far *fatTable = NULL;
static unsigned char far *rootEntriesTable = NULL;
static struct BootSector far *bootSector = NULL;
static unsigned char far *buffer = NULL; /* multi purpose buffer with sector size */
static unsigned char drive;
static unsigned dataStartAddress = 0; /* linear address to FAT data area */

unsigned char far *getFatTable(void) {
    return fatTable;
}

unsigned char far *getRootEntriesTable(void) {
    return rootEntriesTable;
}

static void readBootSectorInformation(void) {
    unsigned char sectorsToRead = 1;
    unsigned int startLogicalBlockAddressing = 0;
    unsigned int oem;

    bootSector = (struct BootSector far *)kmalloc(SECTOR_SIZE);
    memset(bootSector, NULL, SECTOR_SIZE);
    (void)DiskOperationLBA(READ, sectorsToRead, startLogicalBlockAddressing, drive, bootSector);

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Read boot sector information\n");
        printFormat(LOGGER, "\tOemName: ");
        for(oem=0; oem<8; oem++) {
            printFormat(LOGGER, "%c", bootSector->oemName[oem]);
        }
        printFormat(LOGGER, "\n\tBytesPerSector: %d\n", bootSector->biosParameterBlock.bytesPerSector);
        printFormat(LOGGER, "\tSectorsPerCluster: %d\n", bootSector->biosParameterBlock.sectorsPerCluster);
        printFormat(LOGGER, "\tReservedSectors: %d\n", bootSector->biosParameterBlock.reservedSectors);
        printFormat(LOGGER, "\tNumberOfFATs: %d\n", bootSector->biosParameterBlock.numberOfFATs);
        printFormat(LOGGER, "\tRootEntries: %d\n", bootSector->biosParameterBlock.rootEntries);
        printFormat(LOGGER, "\tTotalSectors: %d\n", bootSector->biosParameterBlock.totalSectors);
        printFormat(LOGGER, "\tMedia: 0x%x\n", bootSector->biosParameterBlock.media);
        printFormat(LOGGER, "\tSectorsPerFAT: %d\n", bootSector->biosParameterBlock.sectorsPerFAT);
        printFormat(LOGGER, "\tSectorsPerTrack: %d\n", bootSector->biosParameterBlock.sectorsPerTrack);
        printFormat(LOGGER, "\tHeadsPerCylinder: %d\n", bootSector->biosParameterBlock.headsPerCylinder);
        printFormat(LOGGER, "\tHiddenSectors: %d\n", bootSector->biosParameterBlock.hiddenSectors);
    #endif
}

static void readFATtable(void) {
    size_t fatSize;
    unsigned char sectorsToRead;
    unsigned int startLogicalBlockAddressing;

    fatSize = bootSector->biosParameterBlock.bytesPerSector
              * bootSector->biosParameterBlock.sectorsPerFAT;
    sectorsToRead = (unsigned char)bootSector->biosParameterBlock.sectorsPerFAT;
    startLogicalBlockAddressing = bootSector->biosParameterBlock.reservedSectors;

    fatTable = (unsigned char far *)kmalloc(fatSize);
    memset(fatTable, NULL, fatSize);
    
    (void)DiskOperationLBA(READ, sectorsToRead, startLogicalBlockAddressing,
                           drive, fatTable);

    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Read FAT table\n");
        printFormat(LOGGER, "\tFat size: %d\n", fatSize);
        printFormat(LOGGER, "\tSectors to read: %d\n", sectorsToRead);
        printFormat(LOGGER, "\tstarting lba: %d\n", startLogicalBlockAddressing);
        printFormat(LOGGER, "\tcopy at address %x:%x\n", FP_SEG(fatTable), FP_OFF(fatTable));
    #endif
}

static void readRootEntriesTable(void) {
    size_t entriesSize;
    unsigned char sectorsToRead;
    unsigned int startLogicalBlockAddressing;

    entriesSize = bootSector->biosParameterBlock.rootEntries * sizeof(struct FileInformation);
    sectorsToRead = entriesSize / bootSector->biosParameterBlock.bytesPerSector;
    startLogicalBlockAddressing = bootSector->biosParameterBlock.numberOfFATs *
                                  bootSector->biosParameterBlock.sectorsPerFAT +
                                  bootSector->biosParameterBlock.reservedSectors;

    rootEntriesTable = (unsigned char far *)kmalloc(entriesSize);
    memset(rootEntriesTable, NULL, entriesSize);

    (void)DiskOperationLBA(READ, sectorsToRead, startLogicalBlockAddressing,
                           drive, rootEntriesTable);
    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Read root entries table\n");
        printFormat(LOGGER, "\tEntries size in bytes: %d\n", entriesSize);
        printFormat(LOGGER, "\tSectors to read: %d\n", sectorsToRead);
        printFormat(LOGGER, "\tstarting lba: %d\n", startLogicalBlockAddressing);
        printFormat(LOGGER, "\tcopy at address %x:%x\n", FP_SEG(entriesSize), FP_OFF(entriesSize));
    #endif
}

static void initializeFATDataAddress(void) {
    dataStartAddress = bootSector->biosParameterBlock.reservedSectors +
                      (bootSector->biosParameterBlock.sectorsPerFAT *
                       bootSector->biosParameterBlock.numberOfFATs) +
                       ((bootSector->biosParameterBlock.rootEntries *
                       sizeof(struct FileInformation))
                       / bootSector->biosParameterBlock.bytesPerSector);
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

struct FileInformation far *getFileInformation(unsigned char far *rootTable, unsigned char far *fileName) {
    /* Directories (such as the root directory) exist like files on the disk */
    struct FileInformation far *currentFile;
    unsigned int currentOffset = 0;
    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "getFileInformation: [%s],", fileName);
    #endif

    while(currentOffset < bootSector->biosParameterBlock.rootEntries &&
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

unsigned int getFileStartLogicalBlockAddressingInData(unsigned int cluster) {
    /* calculate the file/directory lba in data area on disk */
    unsigned int startLogicalBlockAddressing;
    startLogicalBlockAddressing = dataStartAddress + ((cluster - 2) *
                bootSector->biosParameterBlock.sectorsPerCluster);
    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "\t\tgetFileStartLogicalBlockAddressingInData ");
        printFormat(LOGGER, "@ data area lba=%d\n", startLogicalBlockAddressing);
    #endif
    return startLogicalBlockAddressing;
}

void initializeFAT12(unsigned char bootDrive) {
    #ifdef FAT12_DEBUG
        printFormat(LOGGER, "Initialize FAT12 system\n");
    #endif

    drive = bootDrive;
    buffer = (unsigned char far *)kmalloc(SECTOR_SIZE);

    readBootSectorInformation();
    readFATtable();
    readRootEntriesTable();
    initializeFATDataAddress();
}