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

BAREMETAL_DIR := ../baremetal/
VECTREX_DIR := ../vectrex/
LOADER_DIR := ../loader/
INC_DIR_PITREX := ../
INC_DIR_LIBRARY := ../baremetal/lib2835
INC_DIR_USPI := $(VECTREX_DIR)uspi/include
LIB_DIR :=../lib
SIM_DIR := ../sim/

# -g
CFLAGS := -fuse-ld=bfd -Ofast -I$(INC_DIR_PITREX) -I$(INC_DIR_LIBRARY) -I$(INC_DIR_USPI) -L$(LIB_DIR) \
	-mfloat-abi=hard -nostartfiles -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -DRPI0 -DFREESTANDING -DPITREX_DEBUG -DMHZ1000 -DLOADER_START=0x4000000
CC	= arm-none-eabi-gcc
AS	= $(CC)
AR	= arm-none-eabi-ar

.PHONY: dirCheck
.PHONY: vectrexLib baremetalLib loaderObjects

all:	dirCheck pitrex.img loader.pit

dirCheck:
	if [ ! -d $(BUILD_DIR_PURE) ]; then mkdir $(BUILD_DIR_PURE); fi

baremetalLib: dirCheck
	$(MAKE) -C $(BAREMETAL_DIR) -f Makefile all

vectrexLib: dirCheck
	$(MAKE) -C $(VECTREX_DIR) -f Makefile.baremetal all

loaderObjects: dirCheck \
	$(BUILD_DIR)bootStrapMain.o \
	$(BUILD_DIR)loaderEntry.o \
	$(BUILD_DIR)loaderMain.o

$(BUILD_DIR)loaderEntry.o: loaderEntry.S
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c loaderEntry.S -o $(BUILD_DIR)loaderEntry.o

$(BUILD_DIR)loaderMain.o: loaderMain.c
#	$(AS) $(CFLAGS) -D__ASSEMBLY__ -E -c loaderMain.c -o $(BUILD_DIR)loaderMain.p
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c loaderMain.c -o $(BUILD_DIR)loaderMain.o

$(BUILD_DIR)bootStrapMain.o: bootStrapMain.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)bootStrapMain.o -c bootStrapMain.c

pitrex.img: baremetalLib vectrexLib loaderObjects  
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pitrex.elf.img \
	$(BUILD_DIR)bootStrapMain.o  -lvectrexInterface -lm -lff12c  -ldebug  -lhal -lutils -lconsole -lff12c -lbob -li2c -lbaremetal  -larm -lbcm2835
	$(RM) pitrex.img
	arm-none-eabi-objcopy $(BUILD_DIR)pitrex.elf.img -O binary pitrex.img
ifdef MAKE_IMAGE
	cp pitrex.img /media/sf_ubuntu
	cp pitrex.img /media/sf_E_DRIVE
endif

loader.pit: baremetalLib vectrexLib loaderObjects 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)loader.elf.img \
	$(BUILD_DIR)loaderEntry.o \
	$(BUILD_DIR)loaderMain.o  -lvectrexInterface -luspi -lm -lff12c  -ldebug  -lhal -lutils -lconsole -lff12c -lbob -li2c -lbaremetal -lbcm2835 -larm $(LIB_DIR)/linkerHeapDefBoot.ld
	$(RM) loader.pit
	arm-none-eabi-objcopy $(BUILD_DIR)loader.elf.img -O binary loader.img
#	arm-none-eabi-objdump -l -drwCS -D $(BUILD_DIR)loader.elf.img > $(BUILD_DIR)loader.elf.img.s
	dd if=loader.img of=loader.pit ibs=1 skip=67076096
	rm loader.img
ifdef MAKE_IMAGE
	cp loader.pit /media/sf_ubuntu
	cp loader.pit /media/sf_E_DRIVE
	cp loader.pit /media/sf_J_DRIVE
endif

clean: dirCheck
	$(MAKE) -C $(BAREMETAL_DIR) -f Makefile clean
	$(MAKE) -C $(VECTREX_DIR) -f Makefile.baremetal clean
	$(RM) $(BUILD_DIR)*.*
	$(RM) pitrex.img
	$(RM) loader.img
	$(RM) loader.pit

clean-all: dirCheck
	$(MAKE) -C $(BAREMETAL_DIR) -f Makefile clean
	$(MAKE) -C $(LOADER_DIR) -f Makefile clean
	$(MAKE) -C $(VECTREX_DIR) -f Makefile.baremetal clean
	$(RM) $(BUILD_DIR)*.*
	$(RM) pitrex.img
	$(RM) loader.img
	$(RM) loader.pit
