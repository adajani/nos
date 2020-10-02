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

; @file define.inc
; @author Ahmad Dajani <eng.adajani@gmail.com>
; @date 2 Oct 2020
; @brief File containing boot loader constants

KERNEL_SECTOR_COUNT  EQU 32
KERNEL_LBA_START     EQU 1
KERNEL_SEGMENT       EQU 60h
BOOT_SIGNATURE       EQU 0AA55h
NULL                 EQU 0
CR_LF                EQU 0ADh ;CARRIAGE_RETURN, LINE_FEED
READ_SECTOR_ATTEMPTS EQU 3
JMP_OPCODE           EQU 0EBh
STACK_ADDRESS        EQU 0ffffh