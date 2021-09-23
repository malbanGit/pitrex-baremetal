# Chess makefile
BUILD_DIR := build/
PITREX_DIR := ../../pitrex/pitrex/pitrex/
VECTREX_DIR := ../../pitrex/pitrex/vectrex/
SETTINGS := /opt/pitrex/settings
CFLAGS := -g -I../../pitrex/pitrex/ -DSETTINGS_DIR="\"$(SETTINGS)\"" -DAVOID_TICKS -DPIZERO -DRPI0 -DPITREX -lpthread
# CFLAGS := -Ofast -I../pitrex/ -DPIZERO -DRPI0
CC := gcc

.PHONY: dirCheck


all:	chess
	echo All up to date

dirCheck:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi

clean:
	$(RM) $(BUILD_DIR)*.*


# pitrex lib files
$(BUILD_DIR)bcm2835.o: $(PITREX_DIR)bcm2835.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)bcm2835.o -c $(PITREX_DIR)bcm2835.c

$(BUILD_DIR)pitrexio-gpio.o: $(PITREX_DIR)pitrexio-gpio.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pitrexio-gpio.o -c $(PITREX_DIR)pitrexio-gpio.c


# vectrex lib files
$(BUILD_DIR)vectrexInterface.o: $(VECTREX_DIR)vectrexInterface.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vectrexInterface.o -c $(VECTREX_DIR)vectrexInterface.c
$(BUILD_DIR)osWrapper.o: $(VECTREX_DIR)osWrapper.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)osWrapper.o -c $(VECTREX_DIR)osWrapper.c
$(BUILD_DIR)baremetalUtil.o: $(VECTREX_DIR)baremetalUtil.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)baremetalUtil.o -c $(VECTREX_DIR)baremetalUtil.c


# project files
$(BUILD_DIR)chess_main.o: chess_main.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)chess_main.o -c chess_main.c

$(BUILD_DIR)chess_engine.o: chess_engine.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)chess_engine.o -c chess_engine.c

$(BUILD_DIR)platform.o: platform.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)platform.o -c platform.c

$(BUILD_DIR)window.o: window.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)window.o -c window.c


# build
chess: $(BUILD_DIR)chess_main.o $(BUILD_DIR)chess_engine.o $(BUILD_DIR)platform.o $(BUILD_DIR)bcm2835.o $(BUILD_DIR)pitrexio-gpio.o $(BUILD_DIR)vectrexInterface.o $(BUILD_DIR)osWrapper.o $(BUILD_DIR)baremetalUtil.o $(BUILD_DIR)window.o
	$(RM) chess
	$(CC) $(CFLAGS) -o chess \
	$(BUILD_DIR)bcm2835.o \
	$(BUILD_DIR)pitrexio-gpio.o \
	$(BUILD_DIR)vectrexInterface.o \
	$(BUILD_DIR)osWrapper.o \
	$(BUILD_DIR)baremetalUtil.o \
	$(BUILD_DIR)window.o \
	$(BUILD_DIR)chess_main.o \
	$(BUILD_DIR)chess_engine.o \
	$(BUILD_DIR)platform.o

install:
	install -o root -g games -m 6555 chess /opt/pitrex/bin
