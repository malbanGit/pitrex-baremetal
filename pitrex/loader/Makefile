BUILD_FOR ?= zero2
BASE_DIR = ../..
ifeq ($(BUILD_FOR),zero2)
TARGET	= pitrex7.img loader.pit
else
TARGET	= pitrex.img loader.pit
endif
LOADER = yes

include ../../Rules.mk

loaderObjects: $(BUILD_DIR)bootStrapMain.o \
	$(BUILD_DIR)loaderEntry.o \
	$(BUILD_DIR)loaderMain.o

$(BUILD_DIR)loaderEntry.o: loaderEntry.S
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c loaderEntry.S -o $(BUILD_DIR)loaderEntry.o

$(BUILD_DIR)loaderMain.o: loaderMain.c
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c loaderMain.c -o $(BUILD_DIR)loaderMain.o

$(BUILD_DIR)bootStrapMain.o: bootStrapMain.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)bootStrapMain.o -c bootStrapMain.c

pitrex.img: $(BUILD_DIR)bootStrapMain.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pitrex.elf.img \
	$(BUILD_DIR)bootStrapMain.o -lc -lbaremetal
	$(RM) pitrex.img
	arm-none-eabi-objcopy $(BUILD_DIR)pitrex.elf.img -O binary pitrex.img
	arm-none-eabi-objdump -l -S -D $(BUILD_DIR)pitrex.elf.img > $(BUILD_DIR)pitrex.img.s
ifdef MAKE_IMAGE
	cp pitrex.img /media/sf_ubuntu
	cp pitrex.img /media/sf_E_DRIVE
else
ifdef SERIAL_TRANSFER
	sudo ../../PiTrexSerial/piserial -d $(SERIAL_PORT_NAME) -o pitrex.img -i pitrex.img -a 1
endif
endif

pitrex7.img: $(BUILD_DIR)bootStrapMain.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pitrex7.elf.img \
	$(BUILD_DIR)bootStrapMain.o -lc -lbaremetal
	$(RM) pitrex7.img
	arm-none-eabi-objcopy $(BUILD_DIR)pitrex7.elf.img -O binary pitrex7.img
	arm-none-eabi-objdump -l -S -D $(BUILD_DIR)pitrex7.elf.img > $(BUILD_DIR)pitrex7.img.s
ifdef MAKE_IMAGE
	cp pitrex7.img /media/sf_ubuntu
	cp pitrex7.img /media/sf_E_DRIVE
else
ifdef SERIAL_TRANSFER
	sudo ../../PiTrexSerial/piserial -d $(SERIAL_PORT_NAME) -o pitrex7.img -i pitrex7.img -a 1
endif
endif

loader.pit: loaderObjects  $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)loader.elf.img \
	$(BUILD_DIR)loaderEntry.o $(BUILD_DIR)loaderMain.o $(LIBS)
	$(RM) loader.pit
	arm-none-eabi-objcopy $(BUILD_DIR)loader.elf.img -O binary loader.img
	arm-none-eabi-objdump -l -drwCS -D $(BUILD_DIR)loader.elf.img > $(BUILD_DIR)loader.elf.img.s
	dd if=loader.img of=loader.pit ibs=1 skip=67076096
	rm loader.img
ifdef MAKE_IMAGE
	cp loader.pit /media/sf_ubuntu
	cp loader.pit /media/sf_E_DRIVE/$(IMG_DIR)
else
ifdef SERIAL_TRANSFER
	sudo ../../PiTrexSerial/piserial -d $(SERIAL_PORT_NAME) -o $(IMG_DIR)/loader.pit -i loader.pit -a 1 -b -t
endif
endif

clean-all:: dirCheck
	$(MAKE) -C $(BAREMETAL_DIR) clean-all BUILD_FOR=$(BUILD_FOR)
	$(MAKE) -C $(VECTREX_DIR) clean-all BUILD_FOR=$(BUILD_FOR)
	