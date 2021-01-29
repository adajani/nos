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

/*@file disk.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 4 Jan 2020
* @brief BIOS Disk Input/Output header file
*/
#include <kernel/disk.h>
#include <string.h> /* FP_SEG, FP_OFF */
#include <bios.h> /* CALL_DISKETTE_BIOS */

static struct DiskParameters diskParameters;

int resetDisk(unsigned char drive) {
    _AH = 0;
    _DL = drive;
    CALL_DISKETTE_BIOS();
    if((_FLAGS & 1) || (_AH != 0)) {
        return FAILURE;
    }
    return SUCCESS;
}

int getDiskParameters(struct DiskParameters *diskParameters, unsigned char drive) {
    _AH = 8;
    _DL = drive;
    CALL_DISKETTE_BIOS();
    if((_FLAGS & 1) || (_AH != 0)) {
        return FAILURE;
    }
    diskParameters->sectorsPerTrack = _CL;
    diskParameters->headsPerCylinder = _DH + 1; /* zero based */
    return SUCCESS;
}

int DiskOperation(unsigned char operation, unsigned char numberOfSectors,
                  unsigned char cylinder, unsigned char sector, unsigned char head,
                  unsigned char drive, void far *buffer) {
    register unsigned int attempt;
    for(attempt=0; attempt<DISK_ATTEMPT; attempt++) {
        _AH = operation;
        _AL = numberOfSectors;
        _CH = cylinder;
        _CL = sector;
        _DH = head;
        _DL = drive;
        _ES = FP_SEG(buffer);
        _BX = FP_OFF(buffer);
        CALL_DISKETTE_BIOS();
        if((_FLAGS & 1) == 0 && (_AH == 0)) {
            if(_AL != numberOfSectors) {
                return FAILURE;
            }
            return SUCCESS;
        }
        if(resetDisk(drive) == FAILURE) {
            return FAILURE;
        }
    }
    return FAILURE;
}

int DiskOperationLBA(unsigned char operation, unsigned char numberOfSectors,
                     unsigned int logicalBlockAddressing,
                     unsigned char drive, void far *buffer) {
    unsigned char cylinder;
    unsigned char sector;
    unsigned char head;

    cylinder = logicalBlockAddressing / (diskParameters.headsPerCylinder * diskParameters.sectorsPerTrack);
    head = (logicalBlockAddressing / diskParameters.sectorsPerTrack) % diskParameters.headsPerCylinder;
    sector = (logicalBlockAddressing % diskParameters.sectorsPerTrack) + 1;

    #ifdef DISK_DEBUG
        printFormat(LOGGER, "DiskOperationLBA lba=%d -> cylinder=%d, head=%d, sector=%d\n",
                    logicalBlockAddressing, cylinder, head, sector);
    #endif

    return DiskOperation(operation, numberOfSectors, cylinder, sector, head, drive, buffer);
}

void initializeDisk(unsigned char drive) {
    #ifdef DISK_DEBUG
        static char *bootDrive[] = {"floppy a", "floppy b", "harddisk 0", "harddisk 1"};
        printFormat(LOGGER, "Booting from %s\n", bootDrive[drive]);
    #endif
    
    (void)resetDisk(drive);
    #ifdef DISK_DEBUG
        /* _AX contains the return value from the previous function */
        printFormat(LOGGER, "resetDisk status = %d\n", _AX);
    #endif

    (void)getDiskParameters(&diskParameters, drive);
    #ifdef DISK_DEBUG
        printFormat(LOGGER, "getDiskParameters status = %d\n", _AX);
        if(status == SUCCESS) {
            printFormat(LOGGER, " headsPerCylinder=%d\n", diskParameters.headsPerCylinder);
            printFormat(LOGGER, " sectorsPerTrack=%d\n", diskParameters.sectorsPerTrack);
        }
    #endif
}