BUILD_FOR ?= zero2
BASE_DIR = ..

include ../Rules.mk

SIM_DIR := ../sim/

all: $(BUILD_DIR)dis6502.o $(BUILD_DIR)display.o $(BUILD_DIR)game.o $(BUILD_DIR)mathbox.o $(BUILD_DIR)memory.o $(BUILD_DIR)pokey.o $(BUILD_DIR)vx_interface.o $(BUILD_DIR)debugger.o
	
$(BUILD_DIR)dis6502.o: dis6502.c dis6502.h memory.h
	$(CC) $(CFLAGS) -c dis6502.c -o $(BUILD_DIR)dis6502.o

$(BUILD_DIR)display.o: display.c display.h memory.h
	$(CC) $(CFLAGS) -c display.c -o $(BUILD_DIR)display.o

$(BUILD_DIR)game.o: game.c game.h memory.h display.h sim6502.h macro6502.h
	$(CC) $(CFLAGS) -c game.c -o $(BUILD_DIR)game.o

$(BUILD_DIR)mathbox.o: mathbox.c mathbox.h
	$(CC) $(CFLAGS) -c mathbox.c -o $(BUILD_DIR)mathbox.o

$(BUILD_DIR)debugger.o: debugger.c debugger.h display.h game.h sim6502.h mathbox.h pokey.h memory.c memory.h dis6502.h
	$(CC) $(CFLAGS) -c debugger.c -o $(BUILD_DIR)debugger.o

$(BUILD_DIR)memory.o: memory.c memory.h display.h game.h sim6502.h mathbox.h pokey.h
	$(CC) $(CFLAGS) -c memory.c -o $(BUILD_DIR)memory.o

$(BUILD_DIR)pokey.o: pokey.c pokey.h memory.h
	$(CC) $(CFLAGS) -c pokey.c -o $(BUILD_DIR)pokey.o

$(BUILD_DIR)vx_interface.o: vx_interface.c game.h memory.h display.h
	$(CC) $(CFLAGS) -c vx_interface.c -o $(BUILD_DIR)vx_interface.o

