#include "globalVars.h"

.macro FUNC name
.text
.code 32
.global \name
\name:
.endm




// From the ARM ARM (Architecture Reference Manual). Make sure you get the
// ARMv5 documentation which includes the ARMv6 documentation which is the
// correct processor type for the Broadcom BCM2835. The ARMv6-M manuals
// available on the ARM website are for Cortex-M parts only and are very
// different.
//
// See ARM section A2.2 (Processor Modes)

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_HYPERVISOR,   0x1A
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F
.equ    CPSR_MODE_MASK,         0x1F // Used to mask the Mode bit in the CPSR

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20



.section .init
.code 32
.align 2

.global _start
_start:
    @ set vectors
    ldr pc, reset_handler
    ldr pc, undefined_handler
    ldr pc, swi_handler
    ldr pc, prefetch_handler
    ldr pc, data_handler
    ldr pc, unused_handler
    ldr pc, irq_handler
    ldr pc, fiq_handler

reset_handler:		.word reset_callee
undefined_handler:	.word undefined_callee
swi_handler:		.word unused_callee
prefetch_handler:	.word abort_prefetch_callee
data_handler:		.word abort_data_callee
unused_handler:		.word unused_callee
irq_handler:		.word irq_callee
fiq_handler:		.word fiq_callee

reset_callee:

    // Zero all bits except the CPSR_MODE_MASK bits to be left with the mode value in r11
    // Store the CPSR start mode in a "global variable" that is accessible to all (including the C world)
    mrs r12, CPSR
    and r12, #CPSR_MODE_MASK    
    ldr r11, =_cpsr_startup_mode
    str r12, [r11]

    
// check if this is the first time
// check with "global" vars
//    ldr r3, =#PI_INIT
//    ldr r5, [r3]
//    cmp r5, #MAGIC_INIT_NUMBER
//    beq already_started_once
    
    mov r3, #0x0000
    ldr r3,[r3]
    
    ldr r4, =_start
    ldr r4,[r4]
    
    SUBS r3, r3,r4
    beq already_started_once
    
//    mov	r5, #MAGIC_INIT_NUMBER
//    str r5, [r3]
    
    // save boot parameter for later use
    ldr r5,=#R0_STORE
    str r0, [r5]
    ldr r5,=#R1_STORE
    str r1, [r5]
    ldr r5,=#R2_STORE
    str r2, [r5]

    // init global core init counters
    mov	r4, #0
    ldr r3, =#CORE0_ENTERED
    str r4, [r3]
    ldr r3, =#CORE1_ENTERED
    str r4, [r3]
    ldr r3, =#CORE2_ENTERED
    str r4, [r3]
    ldr r3, =#CORE3_ENTERED
    str r4, [r3]
    ldr r3, =#CORE1_STATE
    str r4, [r3]
    ldr r3, =#CORE2_STATE
    str r4, [r3]
    ldr r3, =#CORE3_STATE
    str r4, [r3]

    mov	r4, #CORE_RUNNING
    ldr r3, =#CORE0_STATE
    str r4, [r3]
    
    
.global multicore_start
multicore_start:
already_started_once:    
    // Did we start up in hypervisor mode? If we didn't go ahead an park the cpus, we can then get back to SVC mode
    // later on
    mrs r12, CPSR
    and r12, #CPSR_MODE_MASK    
    cmp r12, #CPSR_MODE_HYPERVISOR
    bne multicore_park    

    // We're in hypervisor mode and we need to switch back in order to allow us to continue successfully
    mrs r12, CPSR
    bic r12, r12, #CPSR_MODE_MASK
    orr r12, r12, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr SPSR_cxsf, r12

    add lr, pc, #4
    .word 0xE12EF30E	// MSR ELR_hyp,lr  , this register is not know when ARMv6 compiling
    .word 0xE160006E    // ERET this instruction is not know when Armv6 compiling
    
.global multicore_park
multicore_park:
    // read cpu id, stop slave cores
    mrc	p15, 0, r0, c0, c0, 5		// read MPIDR 
    and	r0, r0, #3 // #CORES-1		// get CPU ID   
    CMP     r0, #0
    BEQ     isCore0
    CMP     r0, #1
    BEQ     isCore1
    CMP     r0, #2
    BEQ     isCore2
    CMP     r0, #3
    BEQ     isCore3
unkownCore:  
    wfe
    b       unkownCore

    
isCore1:  // cpu id == 1
    ldr r11, =#CORE1_STATE
    mov r12, #CORE_GRABBED
    str r12, [r11]
    
    // initialize stacks for all modes (overkill!)
    msr CPSR_c,#CPSR_MODE_ABORT|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT 	@ Abort Mode
    ldr r0, =__abt_stack_top1
    mov sp, r0

    msr CPSR_c,#CPSR_MODE_UNDEFINED|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT 	@ Undefined Mode
    ldr r0, =__und_stack_top1
    mov sp, r0

    msr CPSR_c,#CPSR_MODE_IRQ|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT 	@ IRQ Mode
    ldr r0, =__irq_stack_top1 // 0x3d00000+0x10000-0x4000
    mov sp, r0

    msr  CPSR_c,#CPSR_MODE_FIQ|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT	@ FIQ Mode
    ldr r0, =__fiq_stack_top1
    mov sp, r0
    
    msr CPSR_c,#CPSR_MODE_SVR|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT	@ Supervisor Mode
    ldr r0, =__svc_stack_top1
    mov sp, r0
    

    
    @ "https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830"
    @ enable access to cycle counter register
    // https://developer.arm.com/documentation/ddi0595/2020-12/AArch32-Registers/PMUSERENR--Performance-Monitors-User-Enable-Register?lang=en
    // https://forums.raspberrypi.com/viewtopic.php?t=181490    
    // http://neocontra.blogspot.com/2013/05/user-mode-performance-counters-for.html    
    // https://matthewarcus.wordpress.com/2018/01/27/using-the-cycle-counter-registers-on-the-raspberry-pi-3/

    // Arm® Architecture Reference Manual Armv8, for Armv8-A architecture profile.
    // see DDI0487G_b_armv8_arm.pdf, page 7144
    // enable access to cycle counter register
    mov	r0, #(1+2+4+8) // 1 = enable access to performance monitor, 4 = enable cycle counter read, 8 = event counter read
    MCR p15, 0, r0, C9, C14, 0 @ enable user cycle count access

    // PMCR, Performance Monitors Control Register
    // page 7105
    mov	r0, #1+2+4 // 1 enable, 2 Reset counter, 4 Reset cycle counter
    MCR p15, 0, r0, C9, C12, 0 @ Enable all counters in the PNMC control-register (this is a "general" enable)

    // PMCNTENSET, Performance Monitors Count Enable Set register
    // Enable cycle counter specifically
    // bit 31: enable cycle counter
    // bits 0-3: enable performance counters 0-3
    // DDI0487G_b_armv8_arm.pdf page 7101
    // Enable cycle counter specifically
    mov	r0, #0x80000003 // enable cycle counter and the first two event counters (this is a "specific" enable)
    MCR p15, 0, r0, C9, C12, 1 
    
    // PMEVTYPER<n>, Performance Monitors Event Type Registers, n = 0 - 30
    // DDI0487G_b_armv8_arm.pdf page 7117
    // event definition of cycle count at: 2887
    mov	r0, #0x00000011 // cycle count event type (the other counters are also supposed to count cycles)
    MCR p15, 0, r0, C14, C12, 0 //  set counter 0 to count cycle counts
    MCR p15, 0, r0, C14, C12, 1 //  set counter 1 to count cycle counts

    bl recognizeCore
    
copy_core1_park:
    ldr   r1, =parkCore_stub_start
    ldr   r2, =parkCore_stub_end
    ldr   r3, =#CORE1_PARK
4:  
    ldr   r0, [r1]
    str   r0, [r3]
    add   r1, r1, #4
    add   r3, r3, #4
    cmp   r1, r2
    blo   4b
    b	  CORE1_PARK	// branch to GLOBAL part routine
    
isCore2:  // cpu id == 2
    ldr r11, =#CORE2_ENTERED
    ldr r12,[r11]
    add r12, r12, #1
    str r12, [r11]
    wfe
    b       isCore2

isCore3:  // cpu id == 3
    ldr r11, =#CORE3_ENTERED
    ldr r12,[r11]
    add r12, r12, #1
    str r12, [r11]
    wfe
    b       isCore3


#define ARML_MBOX_CLR03	0xcc

// BCM 2710 only
// BCM 2711 would be 0xff800000
#define LOCAL_BASE	0x40000000
parkCore_stub_start:
	ldr	r7, =LOCAL_BASE			@ Load ARM local peripherals base address for later use

	mrc	p15, 0, r4, c0, c0, 5		// read MPIDR 
	and	r4, r4, #3 // #CORES-1		// get CPU ID   
	lsl	r4,r4, #2			// calc as offset from core 0 global vars
	
	movw	r1, #(CORE0_ENTERED & 0xffff)	// load address of ENTERED global var
	movt	r1, #(CORE0_ENTERED >>16)
	add 	r1, r1, r4	 		// r1 = entered address for this core

	movw	r2, #(CORE0_STATE & 0xffff)	// load address of STATE global var
	movt	r2, #(CORE0_STATE >>16)
	add 	r2, r2, r4	 		// r2 = state address for this core

	mov	r4, #CORE_PARKING		// declare core as parking
	str	r4, [r2]
	
	
	mrc	p15, 0, r6, c0, c0, 5		@ Read MPIDR into r6
	lsls	r6, r6, #30			@ Extract processor number field from MPIDR (shifted left by 30)
	lsrs	r6, r6, #(30-4)			@ Calculate offset to mailbox register (i.e. ARML_MBOX_CLR03 + coreid*0x10)
	adds	r6, r6, #ARML_MBOX_CLR03

0:	wfe

//	ldr r3,[r1]				// enter = enter +1
//	add r3, r3, #1
//	str r3, [r1]


	ldr	r3, [r7, r6]			@ Read message (secondary kernel entrypoint)
	cmp	r3, #0				@ If zero, there is no message
	beq	0b

	str	r3, [r7, r6]			@ Clear mailbox
	mov	r4, #CORE_RUNNING		// declare core as parking
	str	r4, [r2]
	
@ Jump to address given in Mailbox
	bx	r3
parkCore_stub_end:
    
    
    
.align 2
    
isCore0:  // cpu id == 0
    ldr r11, =#CORE0_ENTERED
    ldr r12,[r11]
    add r12, r12, #1
    str r12, [r11]


_setup_interrupt_table:

    // Copy vectors to 0x0000, 16 words
    mov r0, #0x0000
    ldr r1, =_start
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}

    
    
// following must be done for ALL cores!
// TODO
    
    
_setup_stacks0_table:
    // initialize stacks for all modes (overkill!)
    msr CPSR_c,#CPSR_MODE_ABORT|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT 	@ Abort Mode
    ldr r0, =__abt_stack_top0
    mov sp, r0

    msr CPSR_c,#CPSR_MODE_UNDEFINED|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT 	@ Undefined Mode
    ldr r0, =__und_stack_top0
    mov sp, r0

    msr CPSR_c,#CPSR_MODE_IRQ|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT 	@ IRQ Mode
    ldr r0, =__irq_stack_top0 // 0x3d00000+0x10000
    mov sp, r0

    msr  CPSR_c,#CPSR_MODE_FIQ|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT	@ FIQ Mode
    ldr r0, =__fiq_stack_top0
    mov sp, r0
    
    msr CPSR_c,#CPSR_MODE_SVR|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT	@ Supervisor Mode

    ldr r0, =__svc_stack_top0
    mov sp, r0
    
    // clear bss section
    mov   r0, #0
    ldr   r1, =__bss_start
    ldr   r2, =__bss_end__
4:  cmp   r1, r2
    strlo r0, [r1], #4
    blo   4b

    // if FP is not enabled
    // and compile is done with: -mhard-float -mfloat-abi=hard
    // than stdlib (e.g. printf()) will
    // crash!
    bl vfp_init
    bl firstInit
    // MMU
    // also sets ALIGNMENT CHECK
    // AND DATA/INSTRUCTION CACHE
    bl mmu_enable @ page table at 0x4000
    
    
    // enable access to cycle counter register
    @ "https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830"
//https://developer.arm.com/documentation/ddi0595/2020-12/AArch32-Registers/PMUSERENR--Performance-Monitors-User-Enable-Register?lang=en
// https://forums.raspberrypi.com/viewtopic.php?t=181490    
// http://neocontra.blogspot.com/2013/05/user-mode-performance-counters-for.html    
// https://matthewarcus.wordpress.com/2018/01/27/using-the-cycle-counter-registers-on-the-raspberry-pi-3/


    // Arm® Architecture Reference Manual Armv8, for Armv8-A architecture profile.
    // see DDI0487G_b_armv8_arm.pdf, page 7144
    mov	r0, #(1+4+8) // 1 = enable access to performance monitor, 4 = enable cycle counter read, 8 = event counter read
    MCR p15, 0, r0, C9, C14, 0 @ enable user cycle count access

    mov	r0, #1
    MCR p15, 0, r0, C9, C12, 0 @ Enable all counters in the PNMC control-register (this is a "general" enable)

    // Enable cycle counter specifically
    // bit 31: enable cycle counter
    // bits 0-3: enable performance counters 0-3
    // DDI0487G_b_armv8_arm.pdf page 7101
    mov	r0, #0x80000003 // enable cycle counter and the first two event counters (this is a "specific" enable)
    MCR p15, 0, r0, C9, C12, 1 
    
    // DDI0487G_b_armv8_arm.pdf page 7120
    mov	r0, #0x00000011 // cycle count event type (the other counters are also supposed to count cycles)
    MCR p15, 0, r0, C14, C12, 0 //  set counter 0 to count cycle counts
    MCR p15, 0, r0, C14, C12, 1 //  set counter 1 to count cycle counts
    
//    msr CPSR_c,#CPSR_MODE_USER|CPSR_IRQ_INHIBIT|CPSR_FIQ_INHIBIT	@ User Mode

    mrrc	p15, 1, r1, r2, c15		@ CPU Extended Control Register
    ldr r11, =auxControl //-> if bit is set, SMP is enabled, should have been done by  ARM stub, before your code is started
    str r1, [r11]

    b      kernelMain
  
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

FUNC halt
        bl print_halt
	//wfe
	b halt

@ following are the actual exception handlers	
FUNC undefined_callee
//        bl print_undefined
	msr   CPSR_c, CPSR_MODE_SVR			@ Switch to SVC mode so we can the retrieve the orignal lr
	mov   r0, #0				@ Set type parameter
	sub   r1, lr, #4;			@ Set address parameter
						@ Subtracting 4 adjusts for the instruction queue giving the address of the instruction that caused this exception
   	b    reset_callee 			@ Call the debug_exception function - does not return

FUNC abort_prefetch_callee
//        bl print_prefetch
	msr   CPSR_c, CPSR_MODE_SVR			@ Switch to SVC mode so we can the retrieve the orignal lr
	mov   r0, #1				@ Set type parameter
	sub   r1, lr, #4;			@ Set address parameter
						@ Subtracting 4 adjusts for the instruction queue giving the address of the instruction that caused this exception
   	b    reset_callee @debug_exception	@ Call the debug_exception function - does not return

FUNC abort_data_callee
//        bl print_data
	mov   r0, #2				@ Set type parameter
	sub   r1, lr, #8;			@ Set address parameter
						@ Subtracting 8 adjusts for the instruction queue giving the address of the instruction that caused this exception
   	b    reset_callee @debug_exception	@ Call the debug_exception function - does not return

FUNC irq_callee 
//        bl print_irq
        b c_irq_handler				@ void __attribute__((interrupt("IRQ"))) c_irq_handler(void)

FUNC fiq_callee
//        bl print_fiq
        b c_fiq_handler				@ void __attribute__((interrupt("FIQ"))) c_fiq_handler(void)

FUNC unused_callee
//        bl print_hang
        b unused_callee
/*    
    
FUNC _get_stack_pointer
        @ Return the stack pointer value
        str     sp, [sp]
        ldr     r0, [sp]
        mov     pc, lr    @ Return from the function

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
/*
    .align 16

.global _kernelLoad
_kernelLoad:    
.INCBIN "linux.img"

    .align  3

    .globl  DelayLoop
DelayLoop:
    subs    r0, r0, #1
    bhi DelayLoop
    mov pc, lr
*/
    
    
    
    
    

#define CONFIG_PREEMPT

/*
 * From: linux/arch/arm/mm/proc-macros.S
 *
 * dcache_line_size - get the minimum D-cache line size from the CTR register
 * on ARMv7.
 */
	.macro	dcache_line_size, reg, tmp
	mrc	p15, 0, \tmp, c0, c0, 1		@ read ctr
	lsr	\tmp, \tmp, #16
	and	\tmp, \tmp, #0xf		@ cache line size encoding
	mov	\reg, #4			@ bytes per word
	mov	\reg, \reg, lsl \tmp		@ actual cache line size
	.endm

/*
 * The secondary kernel init calls v7_flush_dcache_all before it enables
 * the L1; however, the L1 comes out of reset in an undefined state, so
 * the clean + invalidate performed by v7_flush_dcache_all causes a bunch
 * of cache lines with uninitialized data and uninitialized tags to get
 * written out to memory, which does really unpleasant things to the main
 * processor.  We fix this by performing an invalidate, rather than a
 * clean + invalidate, before jumping into the kernel.
 *
 * This function is cloned from arch/arm/mach-tegra/headsmp.S, and needs
 * to be called for both secondary cores startup and primary core resume
 * procedures.
 */
       .globl  invalidate_data_cache_l1_only
invalidate_data_cache_l1_only:
	push	{r4-r6}
	mov	r0, #0
	mcr	p15, 2, r0, c0, c0, 0
	mrc	p15, 1, r0, c0, c0, 0

	movw	r1, #0x7fff
	and	r2, r1, r0, lsr #13

	movw	r1, #0x3ff

	and	r3, r1, r0, lsr #3		@ NumWays - 1
	add	r2, r2, #1			@ NumSets

	and	r0, r0, #0x7
	add	r0, r0, #4			@ SetShift

	clz	r1, r3				@ WayShift
	add	r4, r3, #1			@ NumWays
1:	sub	r2, r2, #1			@ NumSets--
	mov	r3, r4				@ Temp = NumWays
2:	subs	r3, r3, #1			@ Temp--
	mov	r5, r3, lsl r1
	mov	r6, r2, lsl r0
	orr	r5, r5, r6			@ Reg = (Temp<<WayShift)|(NumSets<<SetShift)
	mcr	p15, 0, r5, c7, c6, 2
	bgt	2b
	cmp	r2, #0
	bgt	1b
	dsb	st
	isb
	pop	{r4-r6}
	bx	lr

/*
 *	was: v7_flush_dcache_all()
 *
 *	Invalidate the whole D-cache.
 *
 *	Corrupted registers: r0-r5, r7, r9-r11
 */
	.globl	invalidate_data_cache
invalidate_data_cache:
	push	{r4-r5, r7, r9-r11}
	dmb					@ ensure ordering with previous memory accesses
	mrc	p15, 1, r0, c0, c0, 1		@ read clidr
	mov	r3, r0, lsr #23			@ move LoC into position
	ands	r3, r3, #7 << 1			@ extract LoC*2 from clidr
	beq	5f				@ if loc is 0, then no need to clean
	mov	r10, #0				@ start clean at cache level 0
1:	add	r2, r10, r10, lsr #1		@ work out 3x current cache level
	mov	r1, r0, lsr r2			@ extract cache type bits from clidr
	and	r1, r1, #7			@ mask of the bits for current cache only
	cmp	r1, #2				@ see what cache we have at this level
	blt	4f				@ skip if no cache, or just i-cache
#ifdef CONFIG_PREEMPT
	mrs	r9, cpsr			@ make cssr&csidr read atomic
	cpsid	i
#endif
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	isb					@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0		@ read the new csidr
#ifdef CONFIG_PREEMPT
	msr	cpsr_c, r9
#endif
	and	r2, r1, #7			@ extract the length of the cache lines
	add	r2, r2, #4			@ add 4 (line length offset)
	movw	r4, #0x3ff
	ands	r4, r4, r1, lsr #3		@ find maximum number on the way size
	clz	r5, r4				@ find bit position of way size increment
	movw	r7, #0x7fff
	ands	r7, r7, r1, lsr #13		@ extract max number of the index size
2:	mov	r9, r7				@ create working copy of max index
3:	orr	r11, r10, r4, lsl r5		@ factor way and cache number into r11
	orr	r11, r11, r9, lsl r2		@ factor index number into r11
	mcr	p15, 0, r11, c7, c6, 2		@ invalidate by set/way
	subs	r9, r9, #1			@ decrement the index
	bge	3b
	subs	r4, r4, #1			@ decrement the way
	bge	2b
4:	add	r10, r10, #2			@ increment cache number
	cmp	r3, r10
	bgt	1b
5:	mov	r10, #0				@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb	st
	isb
	pop	{r4-r5, r7, r9-r11}
	bx	lr

/*
 *	Clean the whole D-cache.
 *
 *	Corrupted registers: r0-r5, r7, r9-r11
 */
	.globl	clean_data_cache
clean_data_cache:
	push	{r4-r5, r7, r9-r11}
	dmb					@ ensure ordering with previous memory accesses
	mrc	p15, 1, r0, c0, c0, 1		@ read clidr
	mov	r3, r0, lsr #23			@ move LoC into position
	ands	r3, r3, #7 << 1			@ extract LoC*2 from clidr
	beq	5f				@ if loc is 0, then no need to clean
	mov	r10, #0				@ start clean at cache level 0
1:	add	r2, r10, r10, lsr #1		@ work out 3x current cache level
	mov	r1, r0, lsr r2			@ extract cache type bits from clidr
	and	r1, r1, #7			@ mask of the bits for current cache only
	cmp	r1, #2				@ see what cache we have at this level
	blt	4f				@ skip if no cache, or just i-cache
#ifdef CONFIG_PREEMPT
	mrs	r9, cpsr			@ make cssr&csidr read atomic
	cpsid	i
#endif
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	isb					@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0		@ read the new csidr
#ifdef CONFIG_PREEMPT
	msr	cpsr_c, r9
#endif
	and	r2, r1, #7			@ extract the length of the cache lines
	add	r2, r2, #4			@ add 4 (line length offset)
	movw	r4, #0x3ff
	ands	r4, r4, r1, lsr #3		@ find maximum number on the way size
	clz	r5, r4				@ find bit position of way size increment
	movw	r7, #0x7fff
	ands	r7, r7, r1, lsr #13		@ extract max number of the index size
2:	mov	r9, r7				@ create working copy of max index
3:	orr	r11, r10, r4, lsl r5		@ factor way and cache number into r11
	orr	r11, r11, r9, lsl r2		@ factor index number into r11
	mcr	p15, 0, r11, c7, c10, 2		@ clean by set/way
	subs	r9, r9, #1			@ decrement the index
	bge	3b
	subs	r4, r4, #1			@ decrement the way
	bge	2b
4:	add	r10, r10, #2			@ increment cache number
	cmp	r3, r10
	bgt	1b
5:	mov	r10, #0				@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb	st
	isb
	pop	{r4-r5, r7, r9-r11}
	bx	lr

/*
 *	was: v7_flush_kern_dcache_area(void *addr, size_t size)
 *
 *	Ensure that the data held in the page kaddr is written back
 *	to the page in question.
 *
 *	- addr	- kernel address
 *	- size	- region size
 */
	.globl	CleanAndInvalidateDataCacheRange
CleanAndInvalidateDataCacheRange:
	dcache_line_size r2, r3
	add	r1, r0, r1
	sub	r3, r2, #1
	bic	r0, r0, r3
#ifdef CONFIG_ARM_ERRATA_764369
	dsb
#endif
1:	mcr	p15, 0, r0, c7, c14, 1		@ clean & invalidate D line / unified line
	add	r0, r0, r2
	cmp	r0, r1
	blo	1b
	dsb	st
	bx	lr
    

    
#if RASPPI != 1
#define BCM2835_PERI_BASE		0x3F000000	
#else
#define BCM2835_PERI_BASE		0x20000000	
#endif
    
    
#define BCM2835_ST_BASE			(BCM2835_PERI_BASE + 0x003000)    
FUNC lib_bcm2835_st_read
	ldr r2, =BCM2835_ST_BASE
	ldrd r0,r1,[r2,#4]
	bx lr
    
#define BCM2835_GPIO_BASE		(BCM2835_PERI_BASE + 0x200000)	
FUNC lib_bcm2835_gpio_fsel
@ void bcm2835_gpio_fsel(uint8_t pin, uint8_t mode)
	push {lr}
	mov r3, r0
	ldr r0, =BCM2835_GPIO_BASE
1:							@ Baking Pi Lesson 3 OK03
	cmp r3,#9				@ http://www.cl.cam.ac.uk/freshers/raspberrypi/tutorials/os/ok03.html
	subhi r3,#10
	addhi r0,#4
	bhi 1b					
	add r3, r3, lsl #1			
	mov r2, #7
	lsl r2, r3
	lsl r1, r3
	ldr	r3, [r0]				
	and	r1, r2, r1
	bic	r3, r3, r2
	orr	r3, r3, r1
	str	r3, [r0]
	pop {pc}
        
/**************************/    

	.globl	v_memcpy
	.type   v_memcpy, %function
v_memcpy:
	push	{r0}

	cmp	r2, #127
	bls	2f
	tst	r1, #3
	bne	2f
	tst	r0, #3
	bne	2f

	push	{r4-r10}
1:	ldmia	r1!, {r3-r10}
	sub	r2, #8*4
	stmia	r0!, {r3-r10}
	pld	[r1, #8*4*2]
	cmp	r2, #8*4-1
	bhi	1b
	pop	{r4-r10}

2:	cmp	r2, #0
	beq	4f

3:	ldrb	r3, [r1], #1
	subs	r2, #1
	strb	r3, [r0], #1
	bne	3b

4:	pop	{r0}
	bx	lr



_cpsr_startup_mode:   .word    0x0
.global systemConfigControl
systemConfigControl: .word    0x0
.global auxControl
auxControl: .word    0x0


