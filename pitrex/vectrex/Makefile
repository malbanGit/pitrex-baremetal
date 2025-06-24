BUILD_FOR ?= zero1
BASE_DIR = ../..
TARGET	= $(LIB_DIR)/libuspi.a $(LIB_DIR)/libvectrexInterface.a
VECTERXLIB = yes

include ../../Rules.mk

SRC_DIR_USPI := ./uspi/lib/
PITREX_DIR := ../pitrex

USB_OBJECTS :=	$(VECTERX_DIR)$(BUILD_DIR)circle_env.o \
	$(VECTERX_DIR)$(BUILD_DIR)util.o $(VECTERX_DIR)$(BUILD_DIR)usbstring.o $(VECTERX_DIR)$(BUILD_DIR)uspilibrary.o $(VECTERX_DIR)$(BUILD_DIR)usbstandardhub.o $(VECTERX_DIR)$(BUILD_DIR)usbrequest.o $(VECTERX_DIR)$(BUILD_DIR)usbmouse.o \
	$(VECTERX_DIR)$(BUILD_DIR)usbmidi.o $(VECTERX_DIR)$(BUILD_DIR)usbmassdevice.o $(VECTERX_DIR)$(BUILD_DIR)usbkeyboard.o $(VECTERX_DIR)$(BUILD_DIR)usbgamepad.o $(VECTERX_DIR)$(BUILD_DIR)usbfunction.o $(VECTERX_DIR)$(BUILD_DIR)usbendpoint.o \
	$(VECTERX_DIR)$(BUILD_DIR)usbdevicefactory.o $(VECTERX_DIR)$(BUILD_DIR)usbdevice.o $(VECTERX_DIR)$(BUILD_DIR)usbconfigparser.o $(VECTERX_DIR)$(BUILD_DIR)synchronize.o $(VECTERX_DIR)$(BUILD_DIR)string.o $(VECTERX_DIR)$(BUILD_DIR)smsc951x.o \
	$(VECTERX_DIR)$(BUILD_DIR)macaddress.o $(VECTERX_DIR)$(BUILD_DIR)lan7800.o $(VECTERX_DIR)$(BUILD_DIR)keymap.o $(VECTERX_DIR)$(BUILD_DIR)dwhcixferstagedata.o $(VECTERX_DIR)$(BUILD_DIR)dwhcirootport.o \
	$(VECTERX_DIR)$(BUILD_DIR)dwhciregister.o $(VECTERX_DIR)$(BUILD_DIR)dwhciframeschedper.o $(VECTERX_DIR)$(BUILD_DIR)dwhciframeschednsplit.o $(VECTERX_DIR)$(BUILD_DIR)dwhciframeschednper.o $(VECTERX_DIR)$(BUILD_DIR)dwhcidevice.o \
	$(VECTERX_DIR)$(BUILD_DIR)devicenameservice.o

VECTREX_OBJECTS := $(BUILD_DIR)vectrexInterface.o $(BUILD_DIR)osWrapper.o $(BUILD_DIR)mathSupport.o 

$(BUILD_DIR)mathSupport.o: mathSupport.c mathSupport.h vectrexInterface.h 
	$(CC) $(CFLAGS) -c mathSupport.c -o $(BUILD_DIR)mathSupport.o


$(BUILD_DIR)vectrexInterface.o: vectrexInterface.c vectrexInterface.h commands.c rasterFont.i vectorFont.i ini.c ini.h osWrapper.c osWrapper.h pipeline.i displayPipeline.c \
				vectrexInterface_bitmap.c vectrexInterface_settings.c vectrexInterface_gadgets.c vectrexInterface_smp.c vectrexInterface_VIAPipeline.c vectrexInterface_pipeline.c \
				handleDefs.i v_basicDefines.h pipeline.i pipelineSwitch.i samples.c psgSound.c inputDevice.c uart.c
	$(CC) $(CFLAGS) -c vectrexInterface.c -o $(BUILD_DIR)vectrexInterface.o

$(BUILD_DIR)osWrapper.o: osWrapper.c osWrapper.h  vectrexInterface.h junzip.c junzip.h puff.c puff.h
	$(CC) $(CFLAGS) -c osWrapper.c -o $(BUILD_DIR)osWrapper.o

$(BUILD_DIR)circle_env.o: circle_env.c circle_env.h 
	$(CC) $(CFLAGS) -c circle_env.c -o $(BUILD_DIR)circle_env.o

$(BUILD_DIR)util.o: $(SRC_DIR_USPI)util.c $(INC_DIR_USPI)/uspi/util.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)util.c -o $(BUILD_DIR)util.o

$(BUILD_DIR)usbstring.o: $(SRC_DIR_USPI)usbstring.c $(INC_DIR_USPI)/uspi/usbstring.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbstring.c -o $(BUILD_DIR)usbstring.o

$(BUILD_DIR)uspilibrary.o: $(SRC_DIR_USPI)uspilibrary.c $(INC_DIR_USPI)/uspi/uspilibrary.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)uspilibrary.c -o $(BUILD_DIR)uspilibrary.o

$(BUILD_DIR)usbstandardhub.o: $(SRC_DIR_USPI)usbstandardhub.c $(INC_DIR_USPI)/uspi/usbstandardhub.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbstandardhub.c -o $(BUILD_DIR)usbstandardhub.o

$(BUILD_DIR)usbrequest.o: $(SRC_DIR_USPI)usbrequest.c $(INC_DIR_USPI)/uspi/usbrequest.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbrequest.c -o $(BUILD_DIR)usbrequest.o

$(BUILD_DIR)usbmouse.o: $(SRC_DIR_USPI)usbmouse.c $(INC_DIR_USPI)/uspi/usbmouse.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbmouse.c -o $(BUILD_DIR)usbmouse.o

$(BUILD_DIR)usbmidi.o: $(SRC_DIR_USPI)usbmidi.c $(INC_DIR_USPI)/uspi/usbmidi.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbmidi.c -o $(BUILD_DIR)usbmidi.o

$(BUILD_DIR)usbmassdevice.o: $(SRC_DIR_USPI)usbmassdevice.c $(INC_DIR_USPI)/uspi/usbmassdevice.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbmassdevice.c -o $(BUILD_DIR)usbmassdevice.o

$(BUILD_DIR)usbkeyboard.o: $(SRC_DIR_USPI)usbkeyboard.c $(INC_DIR_USPI)/uspi/usbkeyboard.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbkeyboard.c -o $(BUILD_DIR)usbkeyboard.o

$(BUILD_DIR)usbgamepad.o: $(SRC_DIR_USPI)usbgamepad.c $(INC_DIR_USPI)/uspi/usbgamepad.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbgamepad.c -o $(BUILD_DIR)usbgamepad.o

$(BUILD_DIR)usbfunction.o: $(SRC_DIR_USPI)usbfunction.c $(INC_DIR_USPI)/uspi/usbfunction.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbfunction.c -o $(BUILD_DIR)usbfunction.o

$(BUILD_DIR)usbendpoint.o: $(SRC_DIR_USPI)usbendpoint.c $(INC_DIR_USPI)/uspi/usbendpoint.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbendpoint.c -o $(BUILD_DIR)usbendpoint.o

$(BUILD_DIR)usbdevicefactory.o: $(SRC_DIR_USPI)usbdevicefactory.c $(INC_DIR_USPI)/uspi/usbdevicefactory.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbdevicefactory.c -o $(BUILD_DIR)usbdevicefactory.o

$(BUILD_DIR)usbdevice.o: $(SRC_DIR_USPI)usbdevice.c $(INC_DIR_USPI)/uspi/usbdevice.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbdevice.c -o $(BUILD_DIR)usbdevice.o

$(BUILD_DIR)usbconfigparser.o: $(SRC_DIR_USPI)usbconfigparser.c $(INC_DIR_USPI)/uspi/usbconfigparser.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)usbconfigparser.c -o $(BUILD_DIR)usbconfigparser.o

$(BUILD_DIR)synchronize.o: $(SRC_DIR_USPI)synchronize.c $(INC_DIR_USPI)/uspi/synchronize.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)synchronize.c -o $(BUILD_DIR)synchronize.o

$(BUILD_DIR)string.o: $(SRC_DIR_USPI)string.c $(INC_DIR_USPI)/uspi/string.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)string.c -o $(BUILD_DIR)string.o

$(BUILD_DIR)smsc951x.o: $(SRC_DIR_USPI)smsc951x.c $(INC_DIR_USPI)/uspi/smsc951x.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)smsc951x.c -o $(BUILD_DIR)smsc951x.o

$(BUILD_DIR)macaddress.o: $(SRC_DIR_USPI)macaddress.c $(INC_DIR_USPI)/uspi/macaddress.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)macaddress.c -o $(BUILD_DIR)macaddress.o

$(BUILD_DIR)lan7800.o: $(SRC_DIR_USPI)lan7800.c $(INC_DIR_USPI)/uspi/lan7800.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)lan7800.c -o $(BUILD_DIR)lan7800.o

$(BUILD_DIR)keymap.o: $(SRC_DIR_USPI)keymap.c $(INC_DIR_USPI)/uspi/keymap.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)keymap.c -o $(BUILD_DIR)keymap.o

$(BUILD_DIR)dwhcixferstagedata.o: $(SRC_DIR_USPI)dwhcixferstagedata.c $(INC_DIR_USPI)/uspi/dwhcixferstagedata.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhcixferstagedata.c -o $(BUILD_DIR)dwhcixferstagedata.o

$(BUILD_DIR)dwhcirootport.o: $(SRC_DIR_USPI)dwhcirootport.c $(INC_DIR_USPI)/uspi/dwhcirootport.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhcirootport.c -o $(BUILD_DIR)dwhcirootport.o

$(BUILD_DIR)dwhciregister.o: $(SRC_DIR_USPI)dwhciregister.c $(INC_DIR_USPI)/uspi/dwhciregister.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhciregister.c -o $(BUILD_DIR)dwhciregister.o

$(BUILD_DIR)dwhciframeschedper.o: $(SRC_DIR_USPI)dwhciframeschedper.c $(INC_DIR_USPI)/uspi/dwhciframeschedper.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhciframeschedper.c -o $(BUILD_DIR)dwhciframeschedper.o

$(BUILD_DIR)dwhciframeschednsplit.o: $(SRC_DIR_USPI)dwhciframeschednsplit.c $(INC_DIR_USPI)/uspi/dwhciframeschednsplit.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhciframeschednsplit.c -o $(BUILD_DIR)dwhciframeschednsplit.o

$(BUILD_DIR)dwhciframeschednper.o: $(SRC_DIR_USPI)dwhciframeschednper.c $(INC_DIR_USPI)/uspi/dwhciframeschednper.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhciframeschednper.c -o $(BUILD_DIR)dwhciframeschednper.o

$(BUILD_DIR)dwhcidevice.o: $(SRC_DIR_USPI)dwhcidevice.c $(INC_DIR_USPI)/uspi/dwhcidevice.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)dwhcidevice.c -o $(BUILD_DIR)dwhcidevice.o

$(BUILD_DIR)devicenameservice.o: $(SRC_DIR_USPI)devicenameservice.c $(INC_DIR_USPI)/uspi/devicenameservice.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR_USPI)devicenameservice.c -o $(BUILD_DIR)devicenameservice.o

$(LIB_DIR)/libuspi.a: $(USB_OBJECTS)
	$(AR) -r $(LIB_DIR)/libuspi.a $(USB_OBJECTS)

$(LIB_DIR)/libvectrexInterface.a: $(VECTREX_OBJECTS)
	$(AR) -r $(LIB_DIR)/libvectrexInterface.a $(VECTREX_OBJECTS)

