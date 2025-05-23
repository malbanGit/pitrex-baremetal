BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET	= hello.img pacman.img window.img maze.img perspective.img

include ../Rules.mk

$(BUILD_DIR)hello.o: hello.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)hello.o -c hello.c

hello.img: $(BUILD_DIR)hello.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)hello.elf.img $(BUILD_DIR)hello.o $(LIBS)
	$(RM) hello.img
	arm-none-eabi-objcopy $(BUILD_DIR)hello.elf.img -O binary hello.img
ifdef MAKE_IMAGE
	cp hello.img $(COPY_TARGET1)
	cp hello.img $(COPY_TARGET2)
endif

$(BUILD_DIR)pacman.o: pacman.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pacman.o -c pacman.c

pacman.img: $(BUILD_DIR)pacman.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pacman.elf.img $(BUILD_DIR)pacman.o $(LIBS)
	$(RM) pacman.img
	arm-none-eabi-objcopy $(BUILD_DIR)pacman.elf.img -O binary pacman.img
ifdef MAKE_IMAGE
	cp pacman.img $(COPY_TARGET1)
	cp pacman.img $(COPY_TARGET2)
endif

$(BUILD_DIR)window.o: window.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)window.o -c window.c

window.img: $(BUILD_DIR)window.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)window.elf.img $(BUILD_DIR)window.o $(LIBS)
	$(RM) window.img
	arm-none-eabi-objcopy $(BUILD_DIR)window.elf.img -O binary window.img
ifdef MAKE_IMAGE
	cp window.img $(COPY_TARGET1)
	cp window.img $(COPY_TARGET2)
endif


$(BUILD_DIR)maze.o: maze.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)maze.o -c maze.c

maze.img: $(BUILD_DIR)maze.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)maze.elf.img $(BUILD_DIR)maze.o $(LIBS)
	$(RM) maze.img
	arm-none-eabi-objcopy $(BUILD_DIR)maze.elf.img -O binary maze.img
ifdef MAKE_IMAGE
	cp maze.img $(COPY_TARGET1)
	cp maze.img $(COPY_TARGET2)
endif


$(BUILD_DIR)perspective.o: perspective.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)perspective.o -c perspective.c

perspective.img: $(BUILD_DIR)perspective.o $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)perspective.elf.img $(BUILD_DIR)perspective.o $(LIBS)
	$(RM) perspective.img
	arm-none-eabi-objcopy $(BUILD_DIR)perspective.elf.img -O binary perspective.img
ifdef MAKE_IMAGE
	cp perspective.img $(COPY_TARGET1)
	cp perspective.img $(COPY_TARGET2)
endif

