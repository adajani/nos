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

/*@file disk.h
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 4 Jan 2020
* @brief BIOS Disk Input/Output header file
*/

#ifndef __DISK_H
    #define __DISK_H
    #define DISK_ATTEMPT 3
    /* #define DISK_DEBUG */
    
    #ifdef DISK_DEBUG
        #include <conio.h> /* printFormat */
    #endif

    #define SECTOR_SIZE 512

    enum OPERATION_STATUS {
        SUCCESS = 0,
        FAILURE = -1,
    };

    enum OPERATION_TYPE {
        READ = 2,
        WRITE = 3
    };

    enum DISK_TYPE {
        FLOPPY_A = 0,
        FLOPPY_B = 1,
        HARDDISK_0 = 0x80,
        HARDDISK_1 = 0x81
    };

    struct DiskParameters {
        unsigned int sectorsPerTrack : 6;
        unsigned char headsPerCylinder;
    };

    void initializeDisk(unsigned char drive);
    int resetDisk(unsigned char drive);
    int getDiskParameters(struct DiskParameters *diskParameters, unsigned char drive);
    int DiskOperation(unsigned char operation, unsigned char numberOfSectors, unsigned char cylinder, unsigned char sector,
                      unsigned char head, unsigned char drive, void far *buffer);
    int DiskOperationLBA(unsigned char operation, unsigned char numberOfSectors, unsigned int logicalBlockAddressing,
                         unsigned char drive, void far *buffer);
#endif