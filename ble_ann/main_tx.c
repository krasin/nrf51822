/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_led_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_led_radio_tx_example
*
* @brief Radio Transmitter Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral. 
*
*/

#include <stdint.h>
#include "radio_config.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "simple_uart.h"
#include "boards.h"

// Packet to transmit
static uint8_t packet[] = { 0x40, 0x13, 0x00,
			    0x90, 0xD8, 0x7A, 0xBD, 0xA3, 0xED,
			    0x0B, 0x09, 0x42, 0x75, 0x74, 0x6F, 0x76, 0x6f, 0x2d, 0x34, 0x2e, 0x30 };  

static __inline int8_t ch2freq(uint8_t ch)
{
  switch (ch) {
  case 37:
    return 2;
  case 38:
    return 26;
  case 39:
    return 80;
  default:
    if (ch > 39)
      return -1;
    else if (ch < 11)
      return 4 + (2 * ch);
    else
      return 6 + (2 * ch);
  }
}

void init(void)
{
  /* Start 16 MHz crystal oscillator */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;

  /* Wait for the external oscillator to start up */
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
  {
  }

  // Set radio configuration parameters
  radio_configure();
  
  simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);

  // Set payload pointer
  NRF_RADIO->PACKETPTR = (uint32_t)packet;  


  // We will only announce on channel 37 for now
  int channel = 37;
  NRF_RADIO->DATAWHITEIV = channel & 0x3F;
  NRF_RADIO->FREQUENCY = ch2freq(channel);

}

/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
  init();

  while(true)
  {
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN = 1;
    while (NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->TASKS_START = 1U;
    NRF_RADIO->EVENTS_END = 0U;  
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }
    nrf_delay_ms(100);
  }
}

/**
 *@}
 **/
