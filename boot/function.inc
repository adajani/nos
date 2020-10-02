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

; @file function.inc
; @author Ahmad Dajani <eng.adajani@gmail.com>
; @date 2 Oct 2020
; @brief File containing helper functions for boot loader
;
; @see macro.inc
; @see bpb.inc


; @brief Function to print null terminated string on console using BIOS call
; @param[in] DS data segment of string 
; @param[in] SI offset of string
; @note AX saved in stack
; @return
print proc
    LOCALS @@
    push ax
    mov ah, 0eh ;Teletype function.
@@next:
    lodsb ;Load next byte from string from SI to AL
    or al, al
    je @@done
    int 10h
    jmp short @@next
@@done:
    pop ax
    ret
print endp

; @brief Function to read sectors and load data into memory using BIOS call
; @param[in] DX High 16 bit of LBA
; @param[in] AX Low 16 bit of LBA
; @param[in] CX Sector count
; @param[in] ES Segment to buffer
; @param[in] BX Offset to buffer
; @note ES:BX is the buffer address where the read LBA goes in
; @return CF Set to 1 if error occurred
ReadSector proc
    LOCALS @@
    pusha
@@next:
    mov di, READ_SECTOR_ATTEMPTS
@@retry:
    pusha
    
    ;ax = LBA / SPT
    ;dx = LBA % SPT = sector - 1
    div word ptr cs:bs.bpb.SectorsPerTrack

    mov cx, dx
    inc cx      ;cx = sector no.

    ;ax = (LBA / SPT) / HPC = cylinder
    ;dx = (LBA / SPT) % HPC = head
    xor dx, dx
    div word ptr cs:bs.bpb.HeadsPerCylinder

    ;ch = LSB 0...7 of cylinder no.
    mov ch, al

    shl ah, 6
    ;cl = MSB 8...9 of cylinder no. + sector no.
    or cl, ah

    ;dh = head no.
    mov dh, dl

    mov dl, byte ptr cs:bs.DriveNumber

    ;ah = 2 = read function no.
    ;al = sector count = 1
    mov ax, 201h

    int 13h                     ;Read sectors
    jnc @@readNextSector        ;CF = 0 if no error

    xor ah, ah                  ;Reset function
    int 13h                     ;Reset drive

    popa
    dec di
    ;Extra attempt
    jnz @@retry
    jmp short @@done

@@readNextSector:
    popa
    dec cx
    ;Last sector
    jz @@done

    ;Adjust offset for next sector
    add bx, word ptr cs:bs.bpb.BytesPerSector
    add ax, 1

    ;Adjust LBA for next sector
    adc dx, 0

    PrintMessage msgProgress

    jmp short @@next
@@done:
    popa
    ret
ReadSector endp
