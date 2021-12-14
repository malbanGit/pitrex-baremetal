BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = gyrocks
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS := 	$(BUILD_DIR)gyrocks.o $(BUILD_DIR)objects.o $(BUILD_DIR)hershey_font.o

$(BUILD_DIR)gyrocks.o: gyrocks.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)gyrocks.o -c gyrocks.c

$(BUILD_DIR)objects.o: objects.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)objects.o -c objects.c

$(BUILD_DIR)hershey_font.o: hershey_font.c hershey_font.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)hershey_font.o -c hershey_font.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
