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
