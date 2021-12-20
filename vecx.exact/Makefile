BUILD_FOR ?= zero1
BASE_DIR = ..
TARGET_BASE = vectrexexact
TARGET	= $(TARGET_BASE).img
VECX_DIR := ../vectrexBase/

include ../Rules.mk

OBJS := 	$(BUILD_DIR)edac.o $(BUILD_DIR)e8910.o $(BUILD_DIR)e6522.o \
		$(BUILD_DIR)main.o $(BUILD_DIR)e6809.o $(BUILD_DIR)vecx.o


$(BUILD_DIR)edac.o: $(VECX_DIR)edac.c $(VECX_DIR)edac.h $(VECX_DIR)e6522.h $(VECX_DIR)vecx.h $(VECTREX_DIR)vectrexInterface.h vectrexConfig.h
	$(CC) $(CFLAGS) -I. -o $(BUILD_DIR)edac.o -c $(VECX_DIR)edac.c 

$(BUILD_DIR)e8910.o: $(VECX_DIR)e8910.c $(VECX_DIR)e8910.h $(VECX_DIR)vecx.h $(VECTREX_DIR)vectrexInterface.h vectrexConfig.h
	$(CC) $(CFLAGS) -I. -o $(BUILD_DIR)e8910.o -c $(VECX_DIR)e8910.c

$(BUILD_DIR)e6522.o: $(VECX_DIR)e6522.c $(VECX_DIR)e6522.h $(VECX_DIR)vecx.h $(VECTREX_DIR)vectrexInterface.h vectrexConfig.h
	$(CC) $(CFLAGS) -I. -o $(BUILD_DIR)e6522.o -c $(VECX_DIR)e6522.c

$(BUILD_DIR)main.o: $(VECX_DIR)main.c $(VECX_DIR)e6809.h $(VECX_DIR)vecx.h $(VECTREX_DIR)vectrexInterface.h vectrexConfig.h $(VECX_DIR)ser.h $(VECX_DIR)ser.c
	$(CC) $(CFLAGS) -I. -o $(BUILD_DIR)main.o -c $(VECX_DIR)main.c

$(BUILD_DIR)e6809.o: $(VECX_DIR)e6809.c $(VECX_DIR)e6809.h $(VECX_DIR)vecx.h $(VECTREX_DIR)vectrexInterface.h vectrexConfig.h
	$(CC) $(CFLAGS) -I. -o $(BUILD_DIR)e6809.o -c $(VECX_DIR)e6809.c

$(BUILD_DIR)vecx.o: $(VECX_DIR)vecx.c $(VECX_DIR)vecx.h $(VECX_DIR)e6809.h $(VECX_DIR)e6522.h $(VECX_DIR)e8910.h $(VECX_DIR)edac.h $(VECX_DIR)bios.i $(VECTREX_DIR)vectrexInterface.h vectrexConfig.h
	$(CC) $(CFLAGS) -I. -o $(BUILD_DIR)vecx.o -c $(VECX_DIR)vecx.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
