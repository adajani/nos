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

; @file macro.inc
; @author Ahmad Dajani <eng.adajani@gmail.com>
; @date 2 Oct 2020
; @brief File containing helper macros for boot loader
;
; @see function.inc
; @see define.inc
; @see bpb.inc

; @brief Macro to print null terminated string on console using BIOS call
; @param[in] msg as string
; @note AX saved in stack
; @return
PrintMessage macro msg
    mov si, offset msg
    call print
endm

; @brief Macro to initialize boot loader segments and set the stack
; @param address
; @note CS=DS=ES=SS=0x7c00
; @note SS * 16 + address = linear address for push/pop
; @return
InitStack macro address
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    cli
    mov sp, address
    sti
endm

; @brief Macro to run loaded kernel by using far jump
; @param[in] kSegment Kernel segment address (16 bit)
; @param[in] bootDrive boot drive number (8 bit)
; @return
Run_Kernel macro kSegment, bootDrive
    mov dl, bootDrive
    mov ax, kSegment
    mov es, ax
    mov ds, ax
    mov ss, ax
    ;Far jump to kernel using retf
    push es
    push 0h
    retf
endm
