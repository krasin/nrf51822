TARGET_CHIP := NRF51822_QFAA_CA
BOARD := BOARD_PCA10000
DEVICE := NRF51

C_SOURCE_FILES += main_tx.c
C_SOURCE_FILES += radio_config.c
C_SOURCE_FILES += simple_uart.c
C_SOURCE_FILES += nrf_delay.c

OUTPUT_NAME := radio_transmitter
SDK_PATH = ../../../../


#Uncomment correct line if you have s110 programmed on the chip.
DEVICE_VARIANT := xxaa
#DEVICE_VARIANT := xxab

#USE_SOFTDEVICE := s110
#USE_SOFTDEVICE := s210

SDK_PATH = $(HOME)/lib/sdk/nrf51822/
TEMPLATE_PATH = ../template/

include $(TEMPLATE_PATH)Makefile
