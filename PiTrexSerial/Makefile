all: piserial


piserial: piserial.c Makefile
	cc -o piserial piserial.c
	sudo ./reinitSerialDevices.sh
#	sudo ./piserial -d /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K4MVA-if00-port0
	sudo ./piserial -d /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K4MVA-if00-port0

clean:
	rm -f piserial *~
