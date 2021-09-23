.section .init
.code 32
.align 2
.global _start
// I should probably build an own linker script

.set _LOADER_START, 0x4000000


.org (_LOADER_START-0x8000) 
// REMEMBER in bcm2835
// MEM_COHERENT_REGION s defined now as 0x3f00000 !!!!

_start:

    @ clear bss section
    mov   r0, #0
    ldr   r1, =__bss_start
    ldr   r2, =__bss_end__
4:  cmp   r1, r2
    strlo r0, [r1], #4
    blo   4b    







    bl    loaderMain

.macro FUNC name
.text
.code 32
.global \name
\name:
.endm


FUNC _enable_interrupts
        mrs     r0, cpsr
        bic     r0, r0, #0x80
        msr     cpsr_c, r0
        mov     pc, lr     @ Return from the function


FUNC _disable_interrupts
    mrs     r0, cpsr
 @ set bit 7 to disable
    orr     r0, r0, #0x80
    msr     cpsr_c, r0
    mov     pc, lr


    .align  3

    .globl  DelayLoop
DelayLoop:
    subs    r0, r0, #1
    bhi DelayLoop
    mov pc, lr
