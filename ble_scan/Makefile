TARGET_CHIP := NRF51822_QFAA_CA
BOARD := BOARD_PCA10001
DEVICE := NRF51

CFLAGS := -g

C_SOURCE_FILES += main.c
C_SOURCE_FILES += log.c
C_SOURCE_FILES += nrf_delay.c
C_SOURCE_FILES += simple_uart.c

OUTPUT_NAME := ble_scan
SDK_PATH = ../../../../


#Uncomment correct line if you have s110 programmed on the chip.
DEVICE_VARIANT := xxaa
#DEVICE_VARIANT := xxab

USE_SOFTDEVICE := S110
#USE_SOFTDEVICE := s210

SDK_PATH = $(HOME)/lib/sdk/nrf51822/
TEMPLATE_PATH = ../template/

GDB_PORT_NUMBER = 2331

include $(TEMPLATE_PATH)Makefile
