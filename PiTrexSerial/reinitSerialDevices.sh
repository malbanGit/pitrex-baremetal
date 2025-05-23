#!/bin/bash

# reinitSerialDevice.sh
# Reinitializes serial devices by refreshing the device table and triggering a rescan.

echo "Reinitializing serial devices..."

# Function to check if the script is run as root
require_root() {
    if [ "$EUID" -ne 0 ]; then
        echo "Please run this script as root (e.g., using sudo)."
        exit 1
    fi
}

# Reload kernel modules for USB serial devices
reload_kernel_modules() {
    echo "Reloading kernel modules for serial devices..."
    modprobe -r usbserial 2>/dev/null
    modprobe usbserial 2>/dev/null
    modprobe -r ftdi_sio 2>/dev/null
    modprobe ftdi_sio 2>/dev/null
    echo "Kernel modules reloaded."
}

# Check for connected serial devices
check_devices() {
    echo "Scanning for serial devices..."
    dmesg | grep -i tty | tail -n 10
    echo "Current serial devices in /dev:"
    ls /dev/ttyS* /dev/ttyUSB* 2>/dev/null
}

# Run all steps
require_root
reload_kernel_modules
# check_devices

echo "Serial device reinitialization complete."
