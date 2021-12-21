BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = rebel
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS = $(BUILD_DIR)rdefender.o


$(BUILD_DIR)rdefender.o: rdefender.c 
	$(CC) $(CFLAGS) -c rdefender.c -o $(BUILD_DIR)rdefender.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
