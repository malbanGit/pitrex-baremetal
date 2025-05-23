BUILD_FOR ?= zero1
BASE_DIR = ..
TARGET	= vterm.img

include ../Rules.mk

$(BUILD_DIR)vterm.o: vterm.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vterm.o -c vterm.c

vterm.img: $(BUILD_DIR)vterm.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vterm.elf.img $(BUILD_DIR)vterm.o $(LIBS)
	$(RM) vterm.img
	$(RM) /media/sf_E_DRIVE/printf.log
	arm-none-eabi-objcopy $(BUILD_DIR)vterm.elf.img -O binary vterm.img
	arm-none-eabi-objdump -l -S -D $(BUILD_DIR)vterm.elf.img > $(BUILD_DIR)vterm.img.s
ifdef MAKE_IMAGE
	cp vterm.img $(COPY_TARGET1)
	cp vterm.img $(COPY_TARGET2)
endif


