TARGET_CHIP := NRF51822_QFAA_CA
BOARD := BOARD_PCA10001
DEVICE := NRF51

CFLAGS += -DBLE_STACK_SUPPORT_REQD

C_SOURCE_FILES += main.c
C_SOURCE_FILES += battery.c
C_SOURCE_FILES += nrf_delay.c
C_SOURCE_FILES += led.c

# Bluetooth library files
C_SOURCE_FILES += ble_hrs.c ble_bondmngr.c ble_bas.c ble_dis.c ble_srv_common.c
C_SOURCE_FILES += ble_conn_params.c ble_advdata.c softdevice_handler.c

# Common library files
C_SOURCE_FILES += pstorage.c crc16.c app_timer.c app_button.c app_gpiote.c

# Add current directory to the includes
LIBRARY_PATHS += .


OUTPUT_NAME := ble_app_hrs
SDK_PATH = ../../../../


#Uncomment correct line if you have s110 programmed on the chip.
#DEVICE_VARIANT := xxaa
DEVICE_VARIANT := xxab

USE_SOFTDEVICE := S110

SOFTDEVICE := $(HOME)/lib/softdevice/s110_nrf51822_5.2.1_softdevice.hex
SDK_PATH = $(HOME)/lib/sdk/nrf51822/
TEMPLATE_PATH = ../template/

include $(TEMPLATE_PATH)Makefile
