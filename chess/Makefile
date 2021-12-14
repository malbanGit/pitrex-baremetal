BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = chess
TARGET	= $(TARGET_BASE).img

include ../Rules.mk
CFLAGS += -DPITREX

OBJS := 	$(BUILD_DIR)chess_main.o $(BUILD_DIR)chess_engine.o $(BUILD_DIR)platform.o $(BUILD_DIR)window.o

$(BUILD_DIR)chess_main.o: chess_main.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)chess_main.o -c chess_main.c

$(BUILD_DIR)chess_engine.o: chess_engine.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)chess_engine.o -c chess_engine.c

$(BUILD_DIR)platform.o: platform.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)platform.o -c platform.c

$(BUILD_DIR)window.o: window.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)window.o -c window.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(SIMOBJS) $(LIBS)
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
	