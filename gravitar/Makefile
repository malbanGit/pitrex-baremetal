EMU_NAME := gravitar

BUILD_FOR ?= zero2
BASE_DIR = ..
TARGET_BASE = $(EMU_NAME)
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

SIM_DIR := ../sim/

.PHONY: simLib

OBJS := $(BUILD_DIR)main.o $(BUILD_DIR)sim.o 

SIMOBJS := $(SIM_DIR)$(BUILD_DIR)display.o $(SIM_DIR)$(BUILD_DIR)vx_interface.o $(SIM_DIR)$(BUILD_DIR)memory.o \
	$(SIM_DIR)$(BUILD_DIR)game.o $(SIM_DIR)$(BUILD_DIR)debugger.o $(SIM_DIR)$(BUILD_DIR)dis6502.o \
	$(SIM_DIR)$(BUILD_DIR)mathbox.o $(SIM_DIR)$(BUILD_DIR)pokey.o \

simLib:
	$(MAKE) -C $(SIM_DIR) -f Makefile BUILD_FOR=$(BUILD_FOR) all

$(BUILD_DIR)sim.o: $(EMU_NAME).c $(SIM_DIR)framework.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)sim.o -DSINGLE_GAME="\"$(EMU_NAME)\"" -DGAME_INC="\"../$(EMU_NAME)/$(EMU_NAME).c\"" -c $(SIM_DIR)framework.c

$(BUILD_DIR)main.o: main.c  $(EMU_NAME).c 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)main.o -c main.c -DSINGLE_GAME="\"$(EMU_NAME)\""

$(TARGET): simLib $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(SIMOBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
