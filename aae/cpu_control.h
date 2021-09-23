#ifndef CPUINTFAAE_H
#define CPUINTFAAE_H

#include <stdio.h>


#define CPU_NONE  0
#define CPU_6502Z 1
#define CPU_6502  2
#define CPU_MZ80  3
#define CPU_6809  4
#define CPU_68000 5
#define CPU_CCPU  6



#define INT_TYPE_NONE 0
#define INT_TYPE_NMI  5
#define INT_TYPE_INT  6

#define INT_TYPE_68K1 1
#define INT_TYPE_68K2 2
#define INT_TYPE_68K3 3
#define INT_TYPE_68K4 4
#define INT_TYPE_68K5 5
#define INT_TYPE_68K6 6
#define INT_TYPE_68K7 7


void initz80N(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite, int cpunum);
void initz80(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite,int cpunum);
void init8080(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite,int cpunum);
void init6502Z(struct MemoryReadByte *read, struct MemoryWriteByte *write,int cpunum);
void init6502(struct MemoryReadByte *read, struct MemoryWriteByte *write, int cpunum);
void init68k();
int cpu_scale_by_cycles(int val);
void init_cpu_config();
void run_cpus_to_cycles();
int return_tickcount(int reset);
int get_video_ticks(int val);
void cpu_needs_reset(int cpunum);
void cpu_resetter(int cpunum);
int get_elapsed_ticks(int cpunum);
void cpu_disable_interrupts(int cpunum,int val);
void set_pending_interrupt(int int_type, int cpunum);
void process_pending_interrupts(int cpunum);
void cpu_do_interrupt(int int_type, int cpunum);
void add_eterna_ticks(int cpunum, int ticks);
int get_eterna_ticks(int cpunum);
void add_hertz_ticks(int cpunum, int ticks);
int get_hertz_counter();
int get_current_cpu();

#endif
