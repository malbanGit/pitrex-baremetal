BUILD_DIR_PURE := build.baremetal
BUILD_DIR := $(BUILD_DIR_PURE)/

CC		= gcc

X_CFLAGS	= -I/usr/X11/include
X_LIBRARIES	= -L/usr/X11R6/lib -lX11

CFLAGS		= -Wall -O1 -g -fstack-check -fstack-protector $(X_CFLAGS) -DNO_PI
LIBRARIES	= 

# -fstack-protector
.PHONY: dirCheck

all:	dirCheck aae 

dirCheck:
	if [ ! -d $(BUILD_DIR_PURE) ]; then mkdir $(BUILD_DIR_PURE); fi

$(BUILD_DIR)x_interface.o: x_interface.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)x_interface.o -c x_interface.c

$(BUILD_DIR)log.o: log.h log.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)log.o -c log.c

$(BUILD_DIR)vector.o: vector.h vector.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vector.o -c vector.c

$(BUILD_DIR)samples.o: samples.h samples.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)samples.o -c samples.c

$(BUILD_DIR)pokey.o: pokey.h pokey.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pokey.o -c pokey.c

$(BUILD_DIR)cpuintrf.o: cpuintrf.h cpuintrf.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cpuintrf.o -c cpuintrf.c

$(BUILD_DIR)dips.o: dips.h dips.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)dips.o -c dips.c

$(BUILD_DIR)earom.o: earom.h earom.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)earom.o -c earom.c

$(BUILD_DIR)loaders.o: loaders.h loaders.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)loaders.o -c loaders.c

$(BUILD_DIR)mathbox.o: mathbox.h mathbox.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)mathbox.o -c mathbox.c

$(BUILD_DIR)input.o: input.h input.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)input.o -c input.c

$(BUILD_DIR)rand.o: rand.h rand.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rand.o -c rand.c

$(BUILD_DIR)acommon.o: acommon.h acommon.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)acommon.o -c acommon.c

$(BUILD_DIR)cpu_control.o: cpu_control.h cpu_control.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cpu_control.o -c cpu_control.c

$(BUILD_DIR)aae_avg.o: aae_avg.h aae_avg.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)aae_avg.o -c aae_avg.c

$(BUILD_DIR)m6809.o: m6809/m6809.h m6809/m6809.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6809.o -c m6809/m6809.c

$(BUILD_DIR)m6502.o: m6502/m6502.h m6502/m6502.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6502zp.o -c m6502/m6502zp.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6502.o -c m6502/m6502.c

$(BUILD_DIR)ccpu.o: ccpu.h ccpu.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)ccpu.o -c ccpu.c

$(BUILD_DIR)mz80.o: mz80/mz80.h mz80/mz80.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)mz80.o -c mz80/mz80.c

$(BUILD_DIR)swavg.o: vidhrdw/swavg.h vidhrdw/swavg.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)swavg.o -c vidhrdw/swavg.c

$(BUILD_DIR)atari_vg.o: vidhrdw/atari_vg.h vidhrdw/atari_vg.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)atari_vg.o -c vidhrdw/atari_vg.c
	
$(BUILD_DIR)swmathbx.o: machine/swmathbx.h machine/swmathbx.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)swmathbx.o -c machine/swmathbx.c
	
$(BUILD_DIR)matari_vg.o: machine/matari_vg.h machine/matari_vg.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)matari_vg.o -c machine/matari_vg.c

$(BUILD_DIR)pokyintf.o: sndhrdw/pokyintf.h sndhrdw/pokyintf.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pokyintf.o -c sndhrdw/pokyintf.c

$(BUILD_DIR)5220intf.o: sndhrdw/5220intf.h sndhrdw/5220intf.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)5220intf.o -c sndhrdw/5220intf.c
	
$(BUILD_DIR)tms5220.o: sndhrdw/tms5220.h sndhrdw/tms5220.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)tms5220.o -c sndhrdw/tms5220.c

$(BUILD_DIR)omegrace.o: omegrace.h omegrace.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)omegrace.o -c omegrace.c

$(BUILD_DIR)mhavoc.o: mhavoc.h mhavoc.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)mhavoc.o -c mhavoc.c

$(BUILD_DIR)starwars.o: starwars.h starwars.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)starwars.o -c starwars.c

$(BUILD_DIR)aaemain.o: aaemain.h aaemain.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)aaemain.o -c aaemain.c

aae:  $(BUILD_DIR)log.o $(BUILD_DIR)vector.o  $(BUILD_DIR)samples.o  $(BUILD_DIR)pokey.o   $(BUILD_DIR)cpuintrf.o \
	$(BUILD_DIR)dips.o $(BUILD_DIR)earom.o   $(BUILD_DIR)mathbox.o $(BUILD_DIR)aae_avg.o $(BUILD_DIR)cpu_control.o  \
	$(BUILD_DIR)input.o $(BUILD_DIR)rand.o $(BUILD_DIR)acommon.o $(BUILD_DIR)loaders.o $(BUILD_DIR)atari_vg.o  $(BUILD_DIR)matari_vg.o \
	$(BUILD_DIR)5220intf.o $(BUILD_DIR)tms5220.o  $(BUILD_DIR)pokyintf.o $(BUILD_DIR)swavg.o  $(BUILD_DIR)swmathbx.o  \
	$(BUILD_DIR)m6809.o $(BUILD_DIR)m6502.o $(BUILD_DIR)mz80.o $(BUILD_DIR)x_interface.o \
	$(BUILD_DIR)omegrace.o $(BUILD_DIR)mhavoc.o $(BUILD_DIR)starwars.o   $(BUILD_DIR)aaemain.o 
	$(CC) $(CFLAGS) -o aae \
	$(BUILD_DIR)log.o \
	$(BUILD_DIR)vector.o \
	$(BUILD_DIR)pokey.o \
	$(BUILD_DIR)samples.o \
	$(BUILD_DIR)cpuintrf.o \
	$(BUILD_DIR)earom.o \
	$(BUILD_DIR)dips.o \
	$(BUILD_DIR)mathbox.o \
	$(BUILD_DIR)aae_avg.o \
	$(BUILD_DIR)cpu_control.o \
	$(BUILD_DIR)input.o \
	$(BUILD_DIR)rand.o \
	$(BUILD_DIR)acommon.o \
	$(BUILD_DIR)loaders.o \
	$(BUILD_DIR)pokyintf.o \
	$(BUILD_DIR)5220intf.o \
	$(BUILD_DIR)tms5220.o \
	$(BUILD_DIR)matari_vg.o \
	$(BUILD_DIR)m6502zp.o \
	$(BUILD_DIR)m6502.o \
	$(BUILD_DIR)mz80.o \
	$(BUILD_DIR)m6809.o \
	$(BUILD_DIR)swmathbx.o \
	$(BUILD_DIR)swavg.o \
	$(BUILD_DIR)atari_vg.o \
	$(BUILD_DIR)omegrace.o \
	$(BUILD_DIR)mhavoc.o \
	$(BUILD_DIR)starwars.o \
	$(BUILD_DIR)x_interface.o \
	$(BUILD_DIR)aaemain.o \
	$(X_LIBRARIES) -lm 

clean: dirCheck
	$(RM) $(BUILD_DIR)*.o
	$(RM) aae
