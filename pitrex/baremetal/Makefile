# I prefer "handmade" make files, it is a little bit of more worktime to start with
# but I can read them, others can, and I know what they do!
#
# export PATH=/home/chrissalo/gcc-arm/bin:$PATH

# -DLOADER_START=0x4000000
# must also be defined/changed in *.S files, since they do not parse defines
# REMEMBER in bcm2835
# MEM_COHERENT_REGION s defined now as 0x3f00000 !!!!

BUILD_DIR_PURE := build.baremetal
BUILD_DIR := $(BUILD_DIR_PURE)/

LIB_DIR :=../lib
INC_DIR := ../pitrex
INC_BCM_DIR := ./lib2835


CFLAGS := -fuse-ld=bfd -Ofast -I$(INC_DIR) -I$(INC_BCM_DIR) \
	-mfloat-abi=hard -nostartfiles -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -DRPI0 -DFREESTANDING -DPITREX_DEBUG -DMHZ1000 -DLOADER_START=0x4000000
CC	= arm-none-eabi-gcc
AS	= $(CC)
AR	= arm-none-eabi-ar

BAREMETAL_OBJECTS := $(BUILD_DIR)baremetalEntry.o $(BUILD_DIR)bareMetalMain.o $(BUILD_DIR)cstubs.o $(BUILD_DIR)rpi-armtimer.o $(BUILD_DIR)rpi-aux.o $(BUILD_DIR)rpi-gpio.o \
	$(BUILD_DIR)rpi-interrupts.o $(BUILD_DIR)rpi-systimer.o 

.PHONY: dirCheck

all:	dirCheck $(LIB_DIR)/libbaremetal.a

dirCheck:
	if [ ! -d $(BUILD_DIR_PURE) ]; then mkdir $(BUILD_DIR_PURE); fi

$(BUILD_DIR)baremetalEntry.o: baremetalEntry.S
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c baremetalEntry.S -o $(BUILD_DIR)baremetalEntry.o

$(BUILD_DIR)bareMetalMain.o: bareMetalMain.c rpi-aux.h rpi-gpio.h rpi-armtimer.h rpi-systimer.h rpi-interrupts.h
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c bareMetalMain.c -o $(BUILD_DIR)bareMetalMain.o

$(BUILD_DIR)rpi-armtimer.o: rpi-armtimer.c rpi-armtimer.h rpi-base.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rpi-armtimer.o -c rpi-armtimer.c

$(BUILD_DIR)rpi-aux.o: rpi-aux.c rpi-aux.h rpi-base.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rpi-aux.o -c rpi-aux.c

$(BUILD_DIR)rpi-gpio.o: rpi-gpio.c rpi-gpio.h rpi-base.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rpi-gpio.o -c rpi-gpio.c

$(BUILD_DIR)rpi-interrupts.o: rpi-interrupts.c rpi-interrupts.h rpi-base.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rpi-interrupts.o -c rpi-interrupts.c

$(BUILD_DIR)rpi-systimer.o: rpi-systimer.c rpi-systimer.h rpi-base.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rpi-systimer.o -c rpi-systimer.c

$(BUILD_DIR)cstubs.o: cstubs.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cstubs.o -c cstubs.c

$(LIB_DIR)/libbaremetal.a: $(BAREMETAL_OBJECTS)
	$(AR) -r $(LIB_DIR)/libbaremetal.a $(BAREMETAL_OBJECTS)

clean: dirCheck
	$(RM) $(BUILD_DIR)*.*
	$(RM) $(LIB_DIR)/libbaremetal.a

