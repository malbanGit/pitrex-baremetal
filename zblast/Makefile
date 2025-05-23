BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = zblast
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS := 	$(BUILD_DIR)svgalib-vectrex.o $(BUILD_DIR)vectrextokeyboard.o $(BUILD_DIR)intensitypalette.o \
		$(BUILD_DIR)window.o \
		$(BUILD_DIR)zblast.o $(BUILD_DIR)font.o $(BUILD_DIR)levels.o $(BUILD_DIR)hiscore.o \
		$(BUILD_DIR)anim3d.o $(BUILD_DIR)sod2.o $(BUILD_DIR)grokfile.o $(BUILD_DIR)joy.o

$(BUILD_DIR)window.o: lib/window/window.c lib/window/window.h
	$(CC) $(CFLAGS) -c lib/window/window.c -o $(BUILD_DIR)window.o

$(BUILD_DIR)vectrextokeyboard.o: lib/svgalib-vectrex/vectrextokeyboard.c lib/svgalib-vectrex/vectrextokeyboard.h \
	lib/svgalib-vectrex/vectrexcontrollermap.h
	$(CC) $(CFLAGS) -c lib/svgalib-vectrex/vectrextokeyboard.c -o $(BUILD_DIR)vectrextokeyboard.o

$(BUILD_DIR)intensitypalette.o: lib/svgalib-vectrex/intensitypalette.c \
	lib/svgalib-vectrex/intensitypalette.h
	$(CC) $(CFLAGS) -c lib/svgalib-vectrex/intensitypalette.c -o $(BUILD_DIR)intensitypalette.o

$(BUILD_DIR)svgalib-vectrex.o: lib/svgalib-vectrex/svgalib-vectrex.c lib/svgalib-vectrex/svgalib-vectrex.h \
	lib/svgalib-vectrex/vectrextokeyboard.h \
	lib/svgalib-vectrex/intensitypalette.h
	$(CC) $(CFLAGS) -c lib/svgalib-vectrex/svgalib-vectrex.c -o $(BUILD_DIR)svgalib-vectrex.o

$(BUILD_DIR)zblast.o: zblast.c
	$(CC) $(CFLAGS) -c zblast.c -o $(BUILD_DIR)zblast.o

$(BUILD_DIR)font.o: font.c font.h
	$(CC) $(CFLAGS) -c font.c -o $(BUILD_DIR)font.o

$(BUILD_DIR)levels.o: levels.c levels.h
	$(CC) $(CFLAGS) -c levels.c -o $(BUILD_DIR)levels.o

$(BUILD_DIR)hiscore.o: hiscore.c hiscore.h
	$(CC) $(CFLAGS) -c hiscore.c -o $(BUILD_DIR)hiscore.o

$(BUILD_DIR)anim3d.o: anim3d.c anim3d.h
	$(CC) $(CFLAGS) -c anim3d.c -o $(BUILD_DIR)anim3d.o

$(BUILD_DIR)sod2.o: sod2.c sod2.h
	$(CC) $(CFLAGS) -c sod2.c -o $(BUILD_DIR)sod2.o

$(BUILD_DIR)grokfile.o: grokfile.c grokfile.h
	$(CC) $(CFLAGS) -c grokfile.c -o $(BUILD_DIR)grokfile.o

$(BUILD_DIR)joy.o: joy.c joy.h
	$(CC) $(CFLAGS) -c joy.c -o $(BUILD_DIR)joy.o

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
