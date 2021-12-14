#include "../baremetal/globalVars.h"

.section .init
.code 32
.align 2
// I should probably build an own linker script

.org (LOADER_START-0x8000) 

_start:
    @ clear bss section
    mov   r0, #0
    ldr   r1, =__bss_start
    ldr   r2, =__bss_end__
4:  cmp   r1, r2
    strlo r0, [r1], #4
    blo   4b    

    bl    main
