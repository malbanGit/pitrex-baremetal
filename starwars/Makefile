BUILD_FOR ?= zero2
TARGET_BASE = swsbt
TARGET	= $(TARGET_BASE).img
BASE_DIR = ..

include ../Rules.mk

.PHONY: sbt

OBJS :=  $(BUILD_DIR)log.o $(BUILD_DIR)vector.o $(BUILD_DIR)pokey.o   $(BUILD_DIR)cpuintrf.o \
	$(BUILD_DIR)dips.o $(BUILD_DIR)earom.o $(BUILD_DIR)mathbox.o \
	$(BUILD_DIR)input.o $(BUILD_DIR)rand.o \
	$(BUILD_DIR)loaders.o $(BUILD_DIR)atari_vg.o  \
	$(BUILD_DIR)5220intf.o $(BUILD_DIR)tms5220.o $(BUILD_DIR)pokyintf.o $(BUILD_DIR)swavg.o  $(BUILD_DIR)swmathbx.o  \
	$(BUILD_DIR)m6809_1.o $(BUILD_DIR)m6809_2.o \
	$(BUILD_DIR)matari_vg.o $(BUILD_DIR)starwars.o

sbt: m6809/starwars-unbanked.c

m6809/starwars-unbanked.c:
	$(MAKE) -C translate BUILD_FOR=$(BUILD_FOR) all
	$(MAKE) -C translate/starwars BUILD_FOR=$(BUILD_FOR) all
	cp translate/starwars/starwars-unbanked.c m6809/starwars-unbanked.c
	cp translate/starwars/starwars-unbanked1.c m6809/starwars-unbanked1.c
	cp translate/starwars/starwars-bank0.c m6809/starwars-bank0.c
	cp translate/starwars/starwars-bank1.c m6809/starwars-bank1.c

$(BUILD_DIR)log.o: log.h log.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)log.o -c log.c

$(BUILD_DIR)vector.o: vector.h vector.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vector.o -c vector.c

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

$(BUILD_DIR)m6809_1.o: m6809/m6809.h m6809/m6809_1.h m6809/m6809_1.c m6809/sbt1.h m6809/starwars-unbanked.c m6809/starwars-bank0.c m6809/starwars-bank1.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6809_1.o -c m6809/m6809_1.c

$(BUILD_DIR)m6809_2.o: m6809/m6809.h m6809/m6809_2.h m6809/m6809_2.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)m6809_2.o -c m6809/m6809_2.c

$(BUILD_DIR)swavg.o: vidhrdw/swavg.h vidhrdw/swavg.c vector.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)swavg.o -c vidhrdw/swavg.c

$(BUILD_DIR)atari_vg.o: vidhrdw/atari_vg.h vidhrdw/atari_vg.c vector.h
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

$(BUILD_DIR)starwars.o: starwars.c machine/starwars.c sndhrdw/starwars.c machine/starwars.h sndhrdw/starwars.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)starwars.o -c starwars.c

$(TARGET): sbt $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
	arm-none-eabi-objdump -l -drwCS -D $(BUILD_DIR)$(TARGET_BASE).elf.img > $(BUILD_DIR)$(TARGET_BASE).elf.img.s
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif

clean-all::
	$(MAKE) -C translate BUILD_FOR=$(BUILD_FOR) clean
	$(MAKE) -C translate/starwars BUILD_FOR=$(BUILD_FOR) clean
	$(RM) m6809/starwars-unbanked.c
	$(RM) m6809/starwars-bank0.c
	$(RM) m6809/starwars-bank1.c
	$(RM) translate/starwars/starwars-unbanked.c
	$(RM) translate/starwars/starwars-unbanked.dis
	$(RM) translate/starwars/starwars-unbanked.tags
	$(RM) translate/starwars/starwars-unbanked.asm
	$(RM) translate/starwars/starwars-unbanked.bin
	$(RM) translate/starwars/starwars-bank0.c
	$(RM) translate/starwars/starwars-bank0.dis
	$(RM) translate/starwars/starwars-bank0.tags
	$(RM) translate/starwars/starwars-bank0.asm
	$(RM) translate/starwars/starwars-bank0.bin
	$(RM) translate/starwars/starwars-bank1.c
	$(RM) translate/starwars/starwars-bank1.dis
	$(RM) translate/starwars/starwars-bank1.tag
	$(RM) translate/starwars/starwars-bank1.asm
	$(RM) translate/starwars/starwars-bank1.bin
	$(RM) translate/starwars/starwars-sound.c
	$(RM) translate/starwars/starwars-sound.dis
	$(RM) translate/starwars/starwars-sound.tag
	$(RM) translate/starwars/starwars-sound.asm
	$(RM) translate/starwars/starwars-sound.bin
