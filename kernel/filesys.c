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

/*@file filesys.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 20 Apr 2021
* @brief File system interface source file
*/

#include <kernel/filesys.h> /*  */
#include <string.h> /* NULL */
#include <kernel/memory.h> /* kmalloc */
#ifdef FILESYS_DEBUG
    #include <kernel/debug.h>
#endif

static unsigned char drive;
static unsigned char far *buffer = NULL; /* multi purpose buffer with sector size */
static unsigned char far *fatTable = NULL;
static unsigned char far *rootEntriesTable = NULL;

void loadFile(struct File far *file, unsigned char far *outBuffer) {
    /* if buffer is NULL, the output will be on stdout*/
    unsigned int index = 0;
    struct ClusterChain far *chunk = file->clusterChain;

    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "\tloadFile: ");
    #endif
    while(chunk != NULL) {
        #ifdef FILESYS_DEBUG
        printFormat(LOGGER, "lba=%d @ size=%d,", chunk->cluster, chunk->size);
        #endif
        (void)DiskOperationLBA(READ, 1 /* one sector */, chunk->cluster, drive, buffer);
        
        if(outBuffer) {
            movedata(FP_SEG(buffer), FP_OFF(buffer),
                     FP_SEG(outBuffer), FP_OFF(outBuffer) + index,
                     SECTOR_SIZE);
            index+=SECTOR_SIZE;
        }
        else {
            for(index=0; index<chunk->size; index++) {
                printCharacter(STDOUT, buffer[(unsigned)index]);
            }
        }

        chunk = chunk->next;
    }
    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "\n");
    #endif
}

void fclose(struct File far *file) {
    struct ClusterChain far *currentCluster;
    struct ClusterChain far *nextCluster;

    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "fclose: delete clusters=");
    #endif

    /* delete cluster chain linked list */
    currentCluster = file->clusterChain;
    while(currentCluster != NULL) {
        #ifdef FILESYS_DEBUG
        printFormat(LOGGER, "%d,", currentCluster->cluster);
        #endif
        nextCluster = currentCluster->next;
        kfree(currentCluster);
        currentCluster = nextCluster;
    }
    kfree(file);
    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "Done");
    #endif
}

static struct ClusterChain far *buildFileClusterChain(struct FileInformation far *fileInformation) {
    unsigned int cluster = fileInformation->firstLogicalCluster;
    unsigned int t = cluster;
    unsigned int fat_offset;
    struct ClusterChain far *clusterChainHead = NULL;
    struct ClusterChain far *clusterChainLast = NULL;
    struct ClusterChain far *clusterChainNew = NULL;
    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "\tbuildFileClusterChain:\n");
    #endif
    while(1) {
        /* Construct the linked list */
        clusterChainNew = (struct ClusterChain far *)kmalloc(sizeof(struct ClusterChain));
        clusterChainNew->cluster = getFileStartLogicalBlockAddressingInData(cluster);
        clusterChainNew->size = SECTOR_SIZE; /* TODO: calculate the exact file, instead of sector align */
        clusterChainNew->next = NULL;

        if(clusterChainHead == NULL) {
            clusterChainHead = clusterChainNew;
            clusterChainLast = clusterChainHead;
        }
        clusterChainLast->next = clusterChainNew;
        clusterChainLast = clusterChainNew;

        /* Read FAT table*/
        fat_offset = (cluster * 3) / 2;
        cluster = *(unsigned int far *)MK_FP(FP_SEG(fatTable), FP_OFF(fatTable) + fat_offset);

        if(t & 1) {
            cluster >>= 4;
        }
        else {
            cluster &= 0x0fff;
        }

        if( (cluster == FAT12_BADSECTOR) || (cluster == FAT12_AVAILABLE) || 
            (cluster == FAT12_INVALIDENTRY)) {
                break;
        }

        if ((cluster >= FAT12_RESERVEDs) && (cluster <= FAT12_RESERVEDe)) {
            break;
        }

        if ((cluster >= FAT12_LASTCLUSTERs) && (cluster <= FAT12_LASTCLUSTERe)) {
            break;
        }
        t = cluster;
    }
    return clusterChainHead;
}

static struct FileInformation far *readDirectoryContent(unsigned int cluster, char *fileNameNext) {
    unsigned int start;
    unsigned int t = cluster;
    unsigned int fat_offset;
    struct FileInformation far *fileInformation;

    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "\t readDirectoryContent\n");
    #endif
    while(1) {
        start = getFileStartLogicalBlockAddressingInData(cluster);
        /* read data */
        (void)DiskOperationLBA(READ, 1 /* one sector */, start, drive, buffer);

        // is the first buffer contains the entry that hold the file information\n
        fileInformation = getFileInformation(buffer, (unsigned char far*)MK_FP(FP_SEG(fileNameNext), FP_OFF(fileNameNext)));
        if(fileInformation) {
            return fileInformation;
        }

        /* read FAT */
        fat_offset = (cluster * 3) / 2;
        cluster = *(unsigned int far *)MK_FP(FP_SEG(fatTable), FP_OFF(fatTable) + fat_offset);

        if(t & 1) {
            cluster >>= 4;
        }
        else {
            cluster &= 0x0fff;
        }

        if( (cluster == FAT12_BADSECTOR) || (cluster == FAT12_AVAILABLE) || 
            (cluster == FAT12_INVALIDENTRY)) {
                break;
        }
        if ((cluster >= FAT12_RESERVEDs) && (cluster <= FAT12_RESERVEDe)) {
            break;
        }
        if ((cluster >= FAT12_LASTCLUSTERs) && (cluster <= FAT12_LASTCLUSTERe)) {
            break;
        }

        t = cluster;
    }

    return NULL;
}

struct FileInformation far *openPath(char *path) {
    struct FileInformation far *fileInformation = NULL;
    unsigned int index;
    unsigned int fileNameIndex;
    char fileName[8+3+1];
    char fileNameNext[8+3+1];
    int root = 1;

    if(path[0] != '/') {
        #ifdef FILESYS_DEBUG
        printFormat(LOGGER, "\tCan't handle relative path for now\n");
        #endif
        return NULL;
    }

    /* parse path based on NOS path separetor / */
    for(index=1; path[index] != '\0'; index++) {
        fileNameIndex = 0;
        while(path[index] != '/' && path[index] != '\0') {
            fileName[fileNameIndex++] = path[index++];
        }
        fileName[fileNameIndex]='\0';

        fileNameIndex = 0;
        if(path[index] != '\0') {
            index++;
            while(path[index] != '/' && path[index] != '\0') {
                fileNameNext[fileNameIndex++] = path[index++];
            }
        }
        fileNameNext[fileNameIndex]='\0';

        #ifdef FILESYS_DEBUG
        printFormat(LOGGER, "\t fileName:[%s]\n", fileName);
        printFormat(LOGGER, "\t fileNameNext:[%s]\n",fileNameNext);
        #endif

        if(root) { /* start from root, only one time */
            #ifdef FILESYS_DEBUG
            printFormat(LOGGER, "\tStarting from root\n");
            #endif
            fileInformation = getFileInformation(rootEntriesTable, (unsigned char far*)MK_FP(FP_SEG(fileName), FP_OFF(fileName)));
            if(!fileInformation) {
                #ifdef FILESYS_DEBUG
                printFormat(LOGGER, "\t[%s] is not found\n", fileName);
                #endif
                return NULL;
            }
            root = 0;
        }

        if(fileNameNext[0]=='\0') {
            #ifdef FILESYS_DEBUG
            printFormat(LOGGER, "\tReach end of path:[%s]\n", fileName);
            #endif
            fileInformation = readDirectoryContent(fileInformation->firstLogicalCluster, fileName);
            if(!fileInformation) {
                #ifdef FILESYS_DEBUG
                printFormat(LOGGER, "\t[%s] is not found\n", fileName);
                #endif
                return NULL;
            }
        }
        else {
            if(fileInformation->attributes == DIRECTORY) {
                fileInformation = readDirectoryContent(fileInformation->firstLogicalCluster, fileNameNext);
                if(!fileInformation) {
                    #ifdef FILESYS_DEBUG
                    printFormat(LOGGER, "\t[%s] is not found\n", fileNameNext);
                    #endif
                    return NULL;
                }
                if(fileInformation->attributes == DIRECTORY) {
                    #ifdef FILESYS_DEBUG
                    printFormat(LOGGER, "\tDig into folder path\n");
                    #endif
                    continue;
                }
            }
        }

        return fileInformation; /* found */
    }

    return NULL; /* not found */
}

struct File far *fopen(char *path) {
    /*notes: 1. read only
             2. absolute path
             3. path separator /
             4. file name must include spaces for padding
             5. path include file name
    */
    static unsigned int fileId = 0;
    struct FileInformation far *fileInformation = NULL;
    struct File far *file = NULL;

    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "fopen:\n");
    #endif

    fileInformation = openPath(path);
    if(!fileInformation) {
        return NULL;
    }

    file = kmalloc(sizeof(struct File));
    memset(file, NULL, sizeof(struct File));

    file->fileId = fileId++;
    file->processId = 0; //TODO
    file->size = fileInformation->size;
    file->clusterChain = buildFileClusterChain(fileInformation);
    movedata(FP_SEG(fileInformation->name), FP_OFF(fileInformation->name),
             FP_SEG(file->name), FP_OFF(file->name), FILE_NAME_SIZE);
    movedata(FP_SEG(fileInformation->extension), FP_OFF(fileInformation->extension),
             FP_SEG(file->extension), FP_OFF(file->extension), FILE_EXTENSION_SIZE);

    movedata(FP_SEG(&fileInformation->creationTime), FP_OFF(&fileInformation->creationTime),
             FP_SEG(&file->creationTime), FP_OFF(&file->creationTime), sizeof(struct FileTime));
    movedata(FP_SEG(&fileInformation->creationDate), FP_OFF(&fileInformation->creationDate),
             FP_SEG(&file->creationDate), FP_OFF(&file->creationDate), sizeof(struct FileDate));
    movedata(FP_SEG(&fileInformation->lastAccessDate), FP_OFF(&fileInformation->lastAccessDate),
             FP_SEG(&file->lastAccessDate), FP_OFF(&file->lastAccessDate), sizeof(struct FileDate));
    movedata(FP_SEG(&fileInformation->lastWriteTime), FP_OFF(&fileInformation->lastWriteTime),
             FP_SEG(&file->lastWriteTime), FP_OFF(&file->lastWriteTime), sizeof(struct FileTime));
    movedata(FP_SEG(&fileInformation->lastWriteDate), FP_OFF(&fileInformation->lastWriteDate),
             FP_SEG(&file->lastWriteDate), FP_OFF(&file->lastWriteDate), sizeof(struct FileDate));
    return file;
}


void initializeFileSystem(unsigned char bootDrive) {
    #ifdef FILESYS_DEBUG
    printFormat(LOGGER, "initializeFileSystem:\n");
    #endif
    drive = bootDrive;
    buffer = (unsigned char far *)kmalloc(SECTOR_SIZE);
    fatTable = getFatTable();
    rootEntriesTable = getRootEntriesTable();
}

#if 0
static void printFileName(enum PRINT_STREAM stream, unsigned char far *name, unsigned int size) {
    register unsigned int index;
    for(index = 0; (name[index] != ' ') && (index<size); index++) {
        printCharacter(stream, name[index]);
    }
}

static void showDirectory(unsigned char far *rootTable) {
    /* TODO: show sub directories */
    struct FileInformation far *file;
    unsigned int currentOffset = 0;
    unsigned int filesCount = 0;
    unsigned int directoriesCount = 0;

    #ifdef FILESYS_DEBUG
        printFormat(LOGGER, "showDirectory\n");
    #endif

    while(currentOffset < bootSector->biosParameterBlock.rootEntries &&
          rootTable[currentOffset] != NULL) {

        file = (struct FileInformation far *)MK_FP(FP_SEG(rootTable), FP_OFF(rootTable) + currentOffset);
        currentOffset += sizeof(struct FileInformation);

        /* don't show */
        if((file->name[0] == DELETED_FILE) || (file->attributes & VOLUME)) {
            continue;
        }

        #ifdef FILESYS_DEBUG
            printCharacter(LOGGER, '\t');
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
            #ifdef FILESYS_DEBUG
                printCharacter(LOGGER, '.');
                printFileName(LOGGER, file->extension, FILE_EXTENSION_SIZE);
                printFormat(LOGGER, ", file size:%d", file->size);
            #endif
        }
        #ifdef FILESYS_DEBUG
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
#endif