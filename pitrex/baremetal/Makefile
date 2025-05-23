BUILD_FOR ?= zero1
BASE_DIR = ../..
TARGET	= $(LIB_DIR)/libbaremetal.a
BAREMETALLIB=yes

include ../../Rules.mk

OBJS := $(BUILD_DIR)pitrexio-gpio.o $(BUILD_DIR)baremetalEntry.o $(BUILD_DIR)bareMetalMain.o $(BUILD_DIR)cstubs.o $(BUILD_DIR)pi_support.o $(BUILD_DIR)sd.o $(BUILD_DIR)ff.o

$(BUILD_DIR)baremetalEntry.o: baremetalEntry.S
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c baremetalEntry.S -o $(BUILD_DIR)baremetalEntry.o

$(BUILD_DIR)pi_support.o: pi_support.c pi_support.h 
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c pi_support.c -o $(BUILD_DIR)pi_support.o

$(BUILD_DIR)bareMetalMain.o: bareMetalMain.c pi_support.h
	$(AS) $(CFLAGS) -D__ASSEMBLY__ -c bareMetalMain.c -o $(BUILD_DIR)bareMetalMain.o

$(BUILD_DIR)sd.o: sd.c sd.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)sd.o -c sd.c

$(BUILD_DIR)ff.o: ff.c ff.h ffconf.h diskio.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)ff.o -c ff.c

$(BUILD_DIR)cstubs.o: cstubs.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cstubs.o -c cstubs.c

$(BUILD_DIR)pitrexio-gpio.o: pitrexio-gpio.c pitrexio-gpio.h
	$(AS) $(CFLAGS) -c pitrexio-gpio.c -o $(BUILD_DIR)pitrexio-gpio.o

$(LIB_DIR)/libbaremetal.a: $(OBJS)
	$(AR) -r $(LIB_DIR)/libbaremetal.a $(OBJS)


