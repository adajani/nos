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

; @file c0t.asm
; @author Ahmad Dajani <eng.adajani@gmail.com>
; @date 2 Oct 2020
; @brief File containing the startup routines for OS kernel
;
; This simple OS boot loader is based on reading specific number of sectors,
; which represent the kernel (code/data). Then execute the kernel.
;

extrn _main
public __heap_start

KERNEL_STAK_SIZE EQU 512

DGROUP group _text, _data, _bss, _stack

_text segment byte public USE16 'code'
    org 0h
    assume cs:DGROUP, ds:DGROUP, ss:DGROUP
    kernel_entry proc near
        jmp short kernel_main
        ;TODO add kernel data structure

        kernel_main:
            cli
                ;Stack address is calculated to start right after the BSS section
                lea bx, stack_label
                add bx, KERNEL_STAK_SIZE
                mov sp, bx
                inc bx
                ;Heap start right after the stack
                mov __heap_start, bx
            sti

            call near ptr _main
            jmp $
    kernel_entry endp
_text ends

_data segment word public 'DATA'
    __heap_start dw ?
_data ends

_bss segment word public 'BSS'
_bss ends

_stack SEGMENT PUBLIC 'STACK'
    stack_label label
_stack ENDS

end kernel_entry