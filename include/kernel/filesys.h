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

/*@file filesys.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 20 Apr 2021
* @brief File system interface header
*/

#ifndef __FILESYS_H
    #define __FILESYS_H
    #include <kernel/disk.h> /* initializeDisk */
    #include <kernel/fat12.h> /* initializeFAT12, getFatTable */

    /* #define FILESYS_DEBUG */

    struct ClusterChain {
        unsigned int cluster;
        unsigned int size;
        struct ClusterChain far *next;
    };

    struct File {
        int fileId;
        unsigned int processId;
        unsigned char name[FILE_NAME_SIZE];
        unsigned char extension[FILE_EXTENSION_SIZE];
        unsigned long size;
        struct FileTime creationTime;
        struct FileDate creationDate;
        struct FileDate lastAccessDate;
        struct FileTime lastWriteTime;
        struct FileDate lastWriteDate;
        struct ClusterChain far *clusterChain;
    };

    struct File far *fopen(char *path);
    struct FileInformation far *openPath(char *path);
    struct FileInformation far *readDirectoryContent(unsigned int cluster, char *fileNameNext);
    struct ClusterChain far *buildFileClusterChain(struct FileInformation far *fileInformation);
    void fclose(struct File far *file);
    void loadFile(struct File far *file, unsigned char far *outBuffer);
    void printFileName(enum PRINT_STREAM stream, unsigned char far *name, unsigned int size);
    void showDirectory(unsigned char far *rootTable);
    void initializeFileSystem(unsigned char bootDrive);
#endif