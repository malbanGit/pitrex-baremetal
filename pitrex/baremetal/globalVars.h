#define CORE1_NON_SHAREABLE 	0x3b00000	// buffer for current Vectorpipeline

#define GLOBAL_CODE_AREA 	0x3c00000	// global park routines, normal executable
#define STACK_LOWEST_IRQ 	0x3d00000	// stack for IRQ, not normal (device), somehow is faster in IRQ mode
#define MEM_COHERENT_REGION	0x3e00000 	// mem coherent, device 
#define STACK_LOWEST 		0x3f00000	// stack other, normal not executable
#define LOADER_START 		0x4000000 	// loader program, normal executable


// Globals Vars are accessable via direct memory "pokes"
// from both the loader and the program running
// needed to pass settings/core states and the like
#define GLOBAL_VARS 	(LOADER_START-100)
#define PI_INIT	 	(GLOBAL_VARS)
#define R0_STORE	(PI_INIT+4)
#define R1_STORE 	(R0_STORE+4)
#define R2_STORE 	(R1_STORE+4)
#define SETTING_STORE 	(R2_STORE+4)

#define CORE0_ENTERED	(SETTING_STORE+4)
#define CORE1_ENTERED	(CORE0_ENTERED+4)
#define CORE2_ENTERED	(CORE1_ENTERED+4)
#define CORE3_ENTERED	(CORE2_ENTERED+4)

#define CORE0_STATE	(CORE3_ENTERED+4)
#define CORE1_STATE	(CORE0_STATE+4)
#define CORE2_STATE	(CORE1_STATE+4)
#define CORE3_STATE	(CORE2_STATE+4)


#define CORE_UNKOWN 0
#define CORE_GRABBED 1
#define CORE_PARKING 2
#define CORE_RUNNING 3



#define __svc_stack_top0 (STACK_LOWEST+0x20000)
#define __svc_stack_top1 (__svc_stack_top0+0x20000) 
#define __svc_stack_top2 (__svc_stack_top1+0x20000) 
#define __svc_stack_top3 (__svc_stack_top2+0x20000) 

#define __und_stack_top0 (__svc_stack_top3 + 0x01000)
#define __und_stack_top1 (__und_stack_top0 + 0x01000)
#define __und_stack_top2 (__und_stack_top1 + 0x01000)
#define __und_stack_top3 (__und_stack_top2 + 0x01000)

#define __abt_stack_top0 (__und_stack_top3 + 0x01000)
#define __abt_stack_top1 (__abt_stack_top0 + 0x01000)
#define __abt_stack_top2 (__abt_stack_top1 + 0x01000)
#define __abt_stack_top3 (__abt_stack_top2 + 0x01000)

#define __fiq_stack_top0 (__abt_stack_top3 + 0x01000)
#define __fiq_stack_top1 (__fiq_stack_top0 + 0x01000)
#define __fiq_stack_top2 (__fiq_stack_top1 + 0x01000)
#define __fiq_stack_top3 (__fiq_stack_top2 + 0x01000)

#define __irq_stack_top0 (STACK_LOWEST_IRQ+0x1000)
#define __irq_stack_top1 (__irq_stack_top0+0x1000) 
#define __irq_stack_top2 (__irq_stack_top1+0x1000) 
#define __irq_stack_top3 (__irq_stack_top2+0x1000) 

#define MAGIC_INIT_NUMBER 111

#define CORE1_PARK (GLOBAL_CODE_AREA)
#define CORE2_PARK (CORE1_PARK+1000)
#define CORE3_PARK (CORE2_PARK+1000)

/*

Raspberry boot process (general)
--------------------------------

    GPU loads (firmware loader) bootcode.bin - executes (from first FAT partition)
    GPU loads start.elf – executes (contains the ARM STUB)

somewhere in those steps if available a "fixup.dat" is loaded and applied, which patches some firmware stuff, 
upon other things after that the ARM and GPU memory are correctly returned from mailbox calls

    GPU loads config.txt and cmdline.txt
    GPU generates device tree (if not switched off by config.txt)
    GPU sets the „graphic“ system…
    GPU loads kernel.img (or other file denoted by config.txt) „normally“ to 0x8000 – but other locations can be specified by config.txt
    GPU writes simple „init“ code to 0x0000 which ends with a branch to the loaded kernel
    GPU releases ARM from reset
    ARM starts from a reset at location 0x0000 (reset vector)
    When ARM „reaches“ the loaded kernel address, registers r0-r2 are set with: r0 = #0, r1 = machine type, r2 = pointer to device tree blob

Startup (multicore):
--------------------
Before control is given to kernel, the system executes a first stub (if kernel_old=1 is not set in config.txt), like: https://github.com/raspberrypi/tools/blob/master/armstubs/armstub7.S.
This is loaded to location 0x0000 and executed upon start.

It sets up mailboxes for the cores 1-3 and leaves them in HYP mode.
Also device tree and kernel start address is patched in. After that the firmware fills in after the magic word - 0x5afe570b, the appropriate vectors.
The magic word is deleted.

But this also means, the different cores are waiting somewhere around 0x0090 or so to be woken up, do not overwrite these locations!

To be able to fetch those cores you
a) have to switch to NON HYP mode
b) call the apropriate mailbox with a start address, after a SEV the called core will wake up,
   check its mailbox and jump to the adress given there - the rest is up to the user.

   
PiTrex baremetal boot
---------------------
(non NOOBS!)
Due to the NOOBS partition layout NOOBS loads the kernel.img not from the first FAT partition. 
ATM the filesystem in piTrex baremetal can only access the first partition - this garbles things up a bit. 
Possibly the below mechanisms also work with NOOBS. But they would be wastly redundant. 
NOOBS is a bootloader menu system in itself. So what would you do? Chose raspbian and then a 
second bootloader the pitrex loader is loaded? One could probably configure noobs directly do boot up different pitrex kernels...

    The piTrex „kernel“ is named as default: „kernel.img“ and loaded to 0x8000 (from start.elf)
    baremetalEntry.S:
    
    Registers r0, r1, r2 are saved to „save“ location (memory: 0x80, 0x84, 0x88) in case we need them for a „linux“ boot. 
    On my pi, r2 points to an outer world region: 0x1BFE9C00 and does not collide with anything I ever do. 
    If this changes in the future, 
    I have to relocate the device tree block to a save location!
    
    Vectors (reset/IRQ/exceptions ...) are set up, 8 vectors starting from address 0x0000, 
    followed by an indirect jump table, again 8 data entries (all uint_32 –> 4 bytes) thus occupying the first (8+8)*4 = 64 bytes (0x40)
    
    Stack pointers for different modes are setup to a total memory region: see above
    
    VFP, MMU (page table at 0x4000), Data Cache, Instruction Cache, unaligned memory access, and cycle counter access is switched on
    
    Control is given to „C“ -> kernelMain()
    
    kernelMain():
    Clock speed of ARM is set to 1000Mhz / UART is initialized
    
    Control is given to -> main()
    SD card initialized and the first FAT partitition mounted
    „loader.pit“ is loaded to memory address: 0x4000000 and started

Addition with multi core!
-------------------------
Upon first start (after power up) the boot code checks for cores 1-3.

After core 0 is booted up, the rest of the cores are woken up from their slumber from 0x0090 ff.
They copy a new park loop to the GLOBAL memory and after initializing their stuff, like
- fpu
- mmu, cache, alignemnts
- stack
- cycle counting

They are again parked there with everything setup and awaiting a mailbox call, just as if they were still waiting in the stub.


GLOBAL VARIABLES
----------------
In PiTrex baremetal context mean, these are memory reagions, which are setup ONCE upon power up of PiTrex.
The routines are in "high" memory reagions, like 0x3c00000.
They should NEVER be overwritten, neither by the loader, nor by "new" loaded programs (kernels) to location 0x8000.

The global variables and routine sections house variables and code sections, which after setup are available to BOTH programs in memory:
- the current kernel
- and the loader

Befor switching from loader to kernel or vice versa, the global stuff must be "reset", in general meaning, 
the multi cores should be parked again like as if they were parked by the ARM stub, so that the next 
started kernel can start as if just booted.
The pitrex internal boot will recognize, that the global area is already initialized, and leave it be.

*/
