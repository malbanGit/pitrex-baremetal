BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = ww_vi
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS := 	$(BUILD_DIR)wwviaudio.o $(BUILD_DIR)wwvi_font.o $(BUILD_DIR)rumble.o $(BUILD_DIR)wordwarvi.o


$(BUILD_DIR)rumble.o: rumble.h rumble.c compat.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rumble.o -c rumble.c

$(BUILD_DIR)wwvi_font.o: wwvi_font.h wwvi_font.c compat.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)wwvi_font.o -c wwvi_font.c

$(BUILD_DIR)wwviaudio.o: wwviaudio.h wwviaudio.c compat.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)wwviaudio.o -c wwviaudio.c

$(BUILD_DIR)wordwarvi.o: wordwarvi.c compat.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)wordwarvi.o -c wordwarvi.c

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
