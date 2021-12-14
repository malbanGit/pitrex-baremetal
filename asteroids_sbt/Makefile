BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = asteroids_sbt
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS := 	$(BUILD_DIR)dvg.o $(BUILD_DIR)gmain.o

game.c: trans.c 035127-02.np3  035143-02.j2  035144-02.h2  035145-02.ef2
	$(GCC) -o trans trans.c
	./trans > game.c

$(BUILD_DIR)dvg.o: dvg.h dvg.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)dvg.o -c dvg.c

$(BUILD_DIR)gmain.o: game.c gmain.c rom.h sound.i
	$(CC) $(CFLAGS) -o $(BUILD_DIR)gmain.o -c gmain.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
