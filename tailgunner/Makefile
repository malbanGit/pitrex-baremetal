
BUILD_FOR ?= zero1
BASE_DIR = ..
TARGET_BASE = tailgunner
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS := 	$(BUILD_DIR)ops.o $(BUILD_DIR)tailgunner.o

tailgunr-ops.c:
	$(MAKE) -C ./translate -f Makefile tailgunr-ops.c
	cp ./translate/tailgunr-ops.c ./tailgunr-ops.c

$(BUILD_DIR)ops.o: ops.c tailgunr-ops.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)ops.o -c ops.c

$(BUILD_DIR)tailgunner.o: tailgunner.c mdep.i mdep.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)tailgunner.o -c tailgunner.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
	arm-none-eabi-objdump -l -drwCS -D $(BUILD_DIR)$(TARGET_BASE).elf.img > $(BUILD_DIR)$(TARGET_BASE).elf.img.s
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
