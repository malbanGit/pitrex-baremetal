BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = vhyperoid
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS = 	$(BUILD_DIR)svgalib-vectrex.o $(BUILD_DIR)vectrextokeyboard.o $(BUILD_DIR)intensitypalette.o \
	$(BUILD_DIR)hyperoid.o $(BUILD_DIR)roidsupp.o $(BUILD_DIR)sound.o $(BUILD_DIR)svga.o $(BUILD_DIR)window.o

$(BUILD_DIR)window.o: window.c window.h
	$(CC) $(CFLAGS) -c window.c -o $(BUILD_DIR)window.o

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

$(BUILD_DIR)hyperoid.o: hyperoid.c
	$(CC) $(CFLAGS) -c hyperoid.c -o $(BUILD_DIR)hyperoid.o

$(BUILD_DIR)roidsupp.o: roidsupp.c roidsupp.h
	$(CC) $(CFLAGS) -c roidsupp.c -o $(BUILD_DIR)roidsupp.o

$(BUILD_DIR)sound.o: sound.c sound.h
	$(CC) $(CFLAGS) -c sound.c -o $(BUILD_DIR)sound.o

$(BUILD_DIR)svga.o: svga.c
	$(CC) $(CFLAGS) -c svga.c -o $(BUILD_DIR)svga.o

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
