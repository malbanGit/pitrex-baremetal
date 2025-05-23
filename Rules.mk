# I prefer "handmade" make files, it is a little bit of more worktime to start with
# but I can read them, others can, and I know what they do!
#
# export PATH=/home/chrissalo/gcc-arm/bin:$PATH

# or zero2, zero1
BUILD_FOR ?= zero1
BASE_DIR ?= ..

MAKE_IMAGE = yes
SERIAL_TRANSFER = yes
SERIAL_PORT_NAME = /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K4MVA-if00-port0

BAREMETAL_DIR := $(BASE_DIR)/pitrex/baremetal/
VECTREX_DIR := $(BASE_DIR)/pitrex/vectrex/
LOADER_DIR := $(BASE_DIR)/pitrex/loader/
INC_DIR_PITREX := $(BASE_DIR)/pitrex/
INC_DIR_USPI := $(VECTREX_DIR)uspi/include

ifeq ($(BUILD_FOR),zero2)
BUILD_DIR_PURE := build7
BUILD_DIR := $(BUILD_DIR_PURE)/
LIB_DIR := $(BASE_DIR)/pitrex/lib7
IMG_DIR := piZero2

CFLAGS := -fuse-ld=bfd -Ofast -I$(INC_DIR_PITREX)  -I$(INC_DIR_USPI) -L$(LIB_DIR) \
	-mhard-float -mfloat-abi=hard -mfpu=neon-fp-armv8 -march=armv8-a -mtune=cortex-a53 \
	-ffreestanding -nostartfiles -DPITREX_DEBUG -DRASPPI=3 
else
BUILD_DIR_PURE := build
BUILD_DIR := $(BUILD_DIR_PURE)/
LIB_DIR :=$(BASE_DIR)/pitrex/lib
IMG_DIR := piZero1

CFLAGS := -fuse-ld=bfd -Ofast -I$(INC_DIR_PITREX)  -I$(INC_DIR_USPI) -L$(LIB_DIR) \
	-mfloat-abi=hard -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s \
	-ffreestanding -nostartfiles -DPITREX_DEBUG -DRASPPI=1
endif

COPY_TARGET1 = /media/sf_ubuntu
COPY_TARGET2 = /media/sf_E_DRIVE/$(IMG_DIR)

LIBS_CHECK := $(LIB_DIR)/libbaremetal.a $(LIB_DIR)/libuspi.a $(LIB_DIR)/libvectrexInterface.a
LIBS := -lvectrexInterface -luspi -lm -lc $(LIB_DIR)/linkerHeapDefBoot.ld -lbaremetal
LIBSGCC := -lvectrexInterface -luspi -lm -lsupc++ $(LIB_DIR)/linkerHeapDefBoot.ld -lbaremetal

CPP	= arm-none-eabi-g++
CC	= arm-none-eabi-gcc
GCC	= gcc
AS	= $(CC)
LD	= arm-none-eabi-ld
AR	= arm-none-eabi-ar

BAREMETAL_LIB_EXISTS := $(or $(and $(wildcard $(BAREMETAL_DIR)libbaremetal.a),1),0)
VECTREX_LIB_EXISTS := $(or $(and $(wildcard $(LIB_DIR)libvectrexInterface.a),1),0)
LOADER_EXISTS := $(or $(and $(wildcard $(LOADER_DIR)loader.pit),1),0)


ALL_TARGETS = dirCheck 
ifneq ($(BAREMETALLIB),yes)
ifneq ($(BAREMETAL_LIB_EXISTS),1)
ALL_TARGETS += baremetalLib 
endif
endif

ifneq ($(VECTERXLIB),yes)
ifneq ($(BAREMETALLIB),yes)
ifneq ($(VECTREX_LIB_EXISTS),1)
ALL_TARGETS += vectrexLib 
endif
endif
endif

ifneq ($(LOADER),yes)
ifneq ($(BAREMETALLIB),yes)
ifneq ($(VECTERXLIB),yes)
ifneq ($(LOADER_EXISTS),1)
ALL_TARGETS += loader 
endif
endif
endif
endif


.PHONY: baremetalLib
.PHONY: vectrexLib
.PHONY: loader
.PHONY: dirCheck

all:	$(ALL_TARGETS) $(TARGET)

dirCheck:
	if [ ! -d $(BUILD_DIR_PURE) ]; then mkdir $(BUILD_DIR_PURE); fi

baremetalLib:
	$(MAKE) -C $(BAREMETAL_DIR) all BUILD_FOR=$(BUILD_FOR)

vectrexLib: 
	$(MAKE) -C $(VECTREX_DIR) all BUILD_FOR=$(BUILD_FOR)

loader: 
	$(MAKE) -C $(LOADER_DIR) all BUILD_FOR=$(BUILD_FOR)

clean:: dirCheck
	$(RM) ../sim/$(BUILD_DIR)*.*
	$(RM) $(BUILD_DIR)*.*
	$(RM) $(TARGET)

clean-all:: dirCheck
ifneq ($(LOADER),yes)
ifneq ($(BAREMETALLIB),yes)
ifneq ($(VECTERXLIB),yes)
	$(MAKE) -C $(BAREMETAL_DIR) clean-all BUILD_FOR=$(BUILD_FOR)
endif	
endif	
endif	
ifneq ($(LOADER),yes)
ifneq ($(BAREMETALLIB),yes)
ifneq ($(VECTERXLIB),yes)
	$(MAKE) -C $(LOADER_DIR) clean-all BUILD_FOR=$(BUILD_FOR)
endif	
endif	
endif	
ifneq ($(LOADER),yes)
ifneq ($(BAREMETALLIB),yes)
ifneq ($(VECTERXLIB),yes)
	$(MAKE) -C $(VECTREX_DIR) clean-all BUILD_FOR=$(BUILD_FOR)
endif	
endif	
endif	
	$(RM) $(BUILD_DIR)*.o
	$(RM) $(TARGET)
	