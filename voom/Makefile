BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = voom
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS = $(BUILD_DIR)voom-vectrex.o


$(BUILD_DIR)voom-vectrex.o: voom-vectrex.c voom.h voom.c
	$(CC) $(CFLAGS) -c voom-vectrex.c -o $(BUILD_DIR)voom-vectrex.o

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
