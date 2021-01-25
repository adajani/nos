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

; @file boot.asm
; @author Ahmad Dajani <eng.adajani@gmail.com>
; @date 2 Oct 2020
; @brief File containing main OS boot loader
;
; This simple OS boot loader is based on reading specific number of sectors,
; which represent the kernel (code/data). Then execute the kernel.
;
; @see macro.inc
; @see function.inc
; @see define.inc
; @see error.inc
; @see bpb.inc

.286
include define.inc
include macro.inc
include bpb.inc

_Text SEGMENT PUBLIC USE16
    org 7c00h
    assume CS:_Text, DS:_Text
    bootEntry proc
        jmp _main
            bs BootSector <>
        _main proc
            InitStack STACK_ADDRESS

            ;Save boot drive
            mov bs.DriveNumber, dl

            PrintMessage msgLoading

            ;Read kernel sectors into ES:BX=>Buffer
            mov ax, KERNEL_SEGMENT 
            mov es, ax
            xor bx, bx

            mov cx, KERNEL_SECTOR_COUNT ;CX=>Number of sectors to read
            xor dx, dx                  ;DX:AX=>LBA
            mov ax, KERNEL_LBA_START
            call ReadSector
            jc ReadSectorError

            PrintMessage msgDone

            ;Check kernel header
            mov ax, KERNEL_SEGMENT
            mov es, ax

            cmp byte ptr es:[0], JMP_OPCODE
            jne KernelError
            ;TODO more checks

            Run_Kernel KERNEL_SEGMENT, bs.DriveNumber

            include error.inc
        _main endp

        include function.inc
        include data.inc
        @SkipBytes     DB 510-($-bootEntry) DUP (NULL)
        @BootSignature DW BOOT_SIGNATURE
    bootEntry endp
_Text ENDS

END bootEntry