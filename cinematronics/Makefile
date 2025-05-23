BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = cine
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS := 	$(BUILD_DIR)cineops.o $(BUILD_DIR)cinemu.o	

$(BUILD_DIR)cineops.o: cineops.c cinedbg.i  cineops.i  startframe.i
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cineops.o -c cineops.c

$(BUILD_DIR)cinemu.o: cinemu.c cines.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cinemu.o -c cinemu.c

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(SIMOBJS) $(LIBS) \
	
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
	