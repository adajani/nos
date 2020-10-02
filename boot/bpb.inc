;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (C) 2020 by Ahmad Dajani                                    ;
;                                                                       ;
; This file is part of NOS.                                             ;
;                                                                       ;
; NOS is free software: you can redistribute it and/or modify it        ;
; under the terms of the GNU Lesser General Public License as published ;
; by the Free Software Foundation, either version 3 of the License, or  ;
; (at your option) any later version.                                   ;
;                                                                       ;
; NOS is distributed in the hope that it will be useful,                ;
; but WITHOUT ANY WARRANTY; without even the implied warranty of        ;
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         ;
; GNU Lesser General Public License for more details.                   ;
;                                                                       ;
; You should have received a copy of the GNU Lesser General Public      ;
; License along with NOS.  If not, see <http://www.gnu.org/licenses/>.  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; @file error.inc
; @author Ahmad Dajani <eng.adajani@gmail.com>
; @date 2 Oct 2020
; @brief File containing BIOS parameter block structures
;
; @see define.inc

BiosParameterBlock struc
    BytesPerSector    DW 512
    SectorsPerCluster DB 1
    ReservedSectors   DW KERNEL_SECTOR_COUNT
    NumberOfFATs      DB 2
    RootEntries       DW 224
    TotalSectors      DW 2880
    Media             DB 0f8h                  ;Media descriptor type (f0: 1.4 MB floppy, f8: hard disk)
    SectorsPerFAT     DW 9
    SectorsPerTrack   DW 18
    HeadsPerCylinder  DW 2
    HiddenSectors     DD KERNEL_SECTOR_COUNT
    TotalSectorsBig   DD 0
BiosParameterBlock ends

BootSector struc
    OemName           DB "NOS/NOS."            ;8 bytes
    Bpb               BiosParameterBlock <>
    DriveNumber       DB 0                     ;Physical disk number. (0) is the A drive disk, hard disks start at 0x80.
    Unused            DB 0                     ;Current Head. Unused in FAT file system. (0)
    ExtBootSignature  DB 029h
    SerialNumber      DD 0deadbeefh
    VolumeLabel       DB "Boot Disk 0"         ;11 bytes
    FileSystem        DB "FAT12   "            ;8 bytes
BootSector ends