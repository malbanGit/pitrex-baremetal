#pragma once

// https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
// https://www.keil.com/support/man/docs/armasm/armasm_dom1359731126962.htm









#define isb() asm volatile ("mcr p15, #0, %[zero], c7, c5,  #4" : : [zero] "r" (0) )
#define dsb() asm volatile ("mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0) )
#define dmb() asm volatile ("mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0) )

#define invalidate_instruction_cache()	asm volatile ("mcr p15, #0, %[zero], c7, c5,  #0" : : [zero] "r" (0) )
#define flush_prefetch_buffer()		asm volatile ("mcr p15, #0, %[zero], c7, c5,  #4" : : [zero] "r" (0) )
#define flush_branch_target_cache()	asm volatile ("mcr p15, #0, %[zero], c7, c5,  #6" : : [zero] "r" (0) )

#define invalidate_data_cache()		asm volatile ("mcr p15, 0, %0, c7, c6,  0\n" \
						      "mcr p15, 0, %0, c7, c10, 4\n" : : "r" (0) : "memory")

// same as flush_dcache_all
#define clean_data_cache()		asm volatile ("mcr p15, 0, %0, c7, c10, 0\n" \
  						      "mcr p15, 0, %0, c7, c10, 4\n" : : "r" (0) : "memory")


#define ARM_CONTROL_MMU				(1 << 0) // MMU
#define ARM_CONTROL_STRICT_ALIGNMENT		(1 << 1) // Alignment
#define ARM_CONTROL_L1_CACHE			(1 << 2) // D Cache
#define ARM_CONTROL_BRANCH_PREDICTION		(1 << 11)
#define ARM_CONTROL_L1_INSTRUCTION_CACHE 	(1 << 12) /* Icache enable		*/

#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION)


// https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830
// https://developer.arm.com/documentation/ddi0301/h/DDI0301H_arm1176jzfs_r0p7_trm.pdf#page=270
// download PDF look at page 270
#define CYCLE_COUNTER_ENABLE       (1<<0)  // enable all three counters   - E
#define COUNTER_ZERO               (1<<1)  // reset two count registers   - P
#define CYCLE_COUNTER_RESET        (1<<2)  // reset cycle counter register- C
#define CYCLE_COUNTER_DIVIDE_64    (1<<3)  // cycle count divider (64)    - D

#define PMNC(v)  asm volatile("mcr p15, 0, %0, c15, c12, 0" :: "r"((v|0x0ffff000)))
#define CCNT(v)  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(v))

// read COUNTER (not cycl counter) 0
#define CCNT0(v)  asm volatile("mrc p15, 0, %0, c15, c12, 2" : "=r"(v))
#define CCNT1(v)  asm volatile("mrc p15, 0, %0, c15, c12, 3" : "=r"(v))

//%0000 11111111 11111111 0000 0000 0000
// counter 0 and counter 1 both also count cycles
// 0x0ffff000

void icache_disable(void);
void dcache_disable(void);

static inline void cache_flush(void)
{
	unsigned long i = 0;
	/* clean entire data cache */
	asm volatile("mcr p15, 0, %0, c7, c10, 0" : : "r" (i));
	/* invalidate both caches and flush btb */
	asm volatile("mcr p15, 0, %0, c7, c7, 0" : : "r" (i));
	/* mem barrier to sync things */
	asm volatile("mcr p15, 0, %0, c7, c10, 4" : : "r" (i));
}

// https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830
static inline void init_perfcounters (int do_reset, int enable_divider)
{
  // in general enable all counters (including cycle counter)
  int value = CYCLE_COUNTER_ENABLE;

  // peform reset:
  if (do_reset)
  {
    value |= COUNTER_ZERO;     // reset all counters to zero.
    value |= CYCLE_COUNTER_RESET;     // reset cycle counter to zero.
  }

  if (enable_divider)
    value |= CYCLE_COUNTER_DIVIDE_64;     // enable "by 64" divider for CCNT.

  PMNC(value);

}
// http://infocenter.arm.com/help/topic/com.arm.doc.ddi0301h/DDI0301H_arm1176jzfs_r0p7_trm.pdf#page=270
//[6] ECCUsed to enable and disable Cycle Counter interrupt reporting:0 = Disable interrupt, reset value1 = Enable interrupt.

static inline unsigned int get_cyclecount (void)
{
  unsigned int value;
  // Read CCNT Register
  CCNT(value);
  return value;
}

// waiter in arm cycles
// that means in nano seconds
static inline unsigned int waitUntil(unsigned int waiter)
{
  unsigned int value;
  // Read CCNT Register
  do
  {
    CCNT(value);
  } while (value < waiter);
  return value;
}




void mmu_disable(void);
char * getErrorText(int errNo);
void vfp_deinit(void);

int peek(unsigned int address);
unsigned char peekByte(unsigned int address);
void pokeByte(unsigned int address, unsigned char c);
char *getLoadParameter(int i);


void/*__attribute__ ((noinline)) */poke(unsigned int address, unsigned int value);
