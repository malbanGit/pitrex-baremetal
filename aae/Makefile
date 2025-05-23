BUILD_FOR = zero2
TARGET_BASE = aae
TARGET	= $(TARGET_BASE).img

include ../Rules.mk


OBJS :=  $(BUILD_DIR)log.o $(BUILD_DIR)vector.o  $(BUILD_DIR)samples.o  $(BUILD_DIR)pokey.o   $(BUILD_DIR)cpuintrf.o \
	$(BUILD_DIR)dips.o $(BUILD_DIR)earom.o   $(BUILD_DIR)mathbox.o $(BUILD_DIR)aae_avg.o $(BUILD_DIR)cpu_control.o  \
	$(BUILD_DIR)input.o $(BUILD_DIR)rand.o $(BUILD_DIR)acommon.o $(BUILD_DIR)cinesnd.o $(BUILD_DIR)cinemat.o $(BUILD_DIR)cinematvid.o \
	$(BUILD_DIR)loaders.o $(BUILD_DIR)atari_vg.o  $(BUILD_DIR)matari_vg.o \
	$(BUILD_DIR)5220intf.o $(BUILD_DIR)tms5220.o  $(BUILD_DIR)pokyintf.o $(BUILD_DIR)swavg.o  $(BUILD_DIR)swmathbx.o  \
	$(BUILD_DIR)m6809_1.o $(BUILD_DIR)m6809_2.o $(BUILD_DIR)m6502.o $(BUILD_DIR)m6502zp.o $(BUILD_DIR)z80.o $(BUILD_DIR)ccpu.o $(BUILD_DIR)m68kcpu.o \
	$(BUILD_DIR)m68kops.o $(BUILD_DIR)softfloat.o $(BUILD_DIR)tempest.o $(BUILD_DIR)spacduel.o $(BUILD_DIR)bzone.o $(BUILD_DIR)omegrace.o \
	$(BUILD_DIR)SegaG80.o $(BUILD_DIR)SegaG80snd.o $(BUILD_DIR)mhavoc.o $(BUILD_DIR)starwars.o $(BUILD_DIR)asteroid.o $(BUILD_DIR)quantum.o $(BUILD_DIR)llander.o $(BUILD_DIR)aaemain.o 

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

$(BUILD_DIR)cinemat.o: cinemat.h cinemat.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cinemat.o -c cinemat.c

$(BUILD_DIR)cinematvid.o: cinematvid.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cinematvid.o -c cinematvid.c

$(BUILD_DIR)cinesnd.o: cinesnd.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cinesnd.o -c cinesnd.c

$(BUILD_DIR)cpu_control.o: cpu_control.h cpu_control.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cpu_control.o -c cpu_control.c

$(BUILD_DIR)aae_avg.o: aae_avg.h aae_avg.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)aae_avg.o -c aae_avg.c

$(BUILD_DIR)ccpu.o: ccpu.h ccpu.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)ccpu.o -c ccpu.c

$(BUILD_DIR)m6809_1.o: m6809/m6809.h m6809/m6809_1.h m6809/m6809_1.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6809_1.o -c m6809/m6809_1.c

$(BUILD_DIR)m6809_2.o: m6809/m6809.h m6809/m6809_2.h m6809/m6809_2.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6809_2.o -c m6809/m6809_2.c

$(BUILD_DIR)m6502.o: m6502/m6502.h m6502/m6502.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6502.o -c m6502/m6502.c

$(BUILD_DIR)m6502zp.o: m6502/m6502.h m6502/m6502.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6502zp.o -c m6502/m6502zp.c

$(BUILD_DIR)z80.o: z80/z80.h z80/z80.c z80/z80daisy.h mz80/mz80.h mz80/mz80.c 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)z80.o -c z80/z80.c

$(BUILD_DIR)m68kcpu.o: musashi/m68kops.c musashi/m68kcpu.c musashi/m68kcpu.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m68kcpu.o -c musashi/m68kcpu.c

$(BUILD_DIR)m68kops.o: musashi/m68kops.c musashi/m68kops.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m68kops.o -c musashi/m68kops.c

$(BUILD_DIR)softfloat.o: musashi/softfloat/softfloat.c musashi/softfloat/softfloat.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)softfloat.o -c musashi/softfloat/softfloat.c

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

$(BUILD_DIR)SegaG80.o: SegaG80.h SegaG80.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)SegaG80.o -c SegaG80.c

$(BUILD_DIR)SegaG80snd.o: SegaG80snd.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)SegaG80snd.o -c SegaG80snd.c

$(BUILD_DIR)tempest.o: tempest.h tempest.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)tempest.o -c tempest.c

$(BUILD_DIR)spacduel.o: spacduel.h spacduel.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)spacduel.o -c spacduel.c

$(BUILD_DIR)bzone.o: bzone.h bzone.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)bzone.o -c bzone.c

$(BUILD_DIR)llander.o: llander.h llander.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)llander.o -c llander.c

$(BUILD_DIR)asteroid.o: asteroid.h asteroid.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)asteroid.o -c asteroid.c

$(BUILD_DIR)mhavoc.o: mhavoc.h mhavoc.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)mhavoc.o -c mhavoc.c

$(BUILD_DIR)quantum.o: quantum.h quantum.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)quantum.o -c quantum.c

$(BUILD_DIR)starwars.o: starwars.h starwars.c machine/starwars.c sndhrdw/starwars.c machine/starwars.h sndhrdw/starwars.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)starwars.o -c starwars.c

$(BUILD_DIR)aaemain.o: aaemain.h aaemain.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)aaemain.o -c aaemain.c

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
