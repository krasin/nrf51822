/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2013 Paulo Sérgio Borges de Oliveira Filho
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <string.h>
#include "nrf51.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "log.h"

#define SET_BIT(n)      (1UL << n)
#define MAX_PDU_SIZE    (64UL)

#define MS(s)           (1000 * s)
#define RTC_PERIOD      (5)         /* ms */
#define RTC_PRESCALER   (((32768 * RTC_PERIOD) / 1000) - 1)
#define SCAN_WINDOW     (MS(1) / RTC_PERIOD)
#define SCAN_INTERVAL   (MS(2) / RTC_PERIOD)

void dbg_packet(int channel, uint8_t* pdu) {
  int pdu_type = pdu[0] & 0xF;
  int len = pdu[1] & 0x3F;
  log_uart("Advertising channel %d\n", channel);
  log_uart("CRC:      %s\n", (NRF_RADIO->CRCSTATUS == 1) ? "OK" : "FAIL");
  log_uart("PDU Type: 0x%02x\n", pdu_type);
  log_uart("TxAdd:    0x%02x\n", (pdu[0] >> 6) & 0x1);
  log_uart("RxAdd:    0x%02x\n", (pdu[0] >> 7) & 0x1);
  log_uart("Length:   0x%02x\n", len);

  if (pdu_type == 0 /* ADV_IND */) {
    log_uart("Address: %02x:%02x:%02x:%02x:%02x:%02x ", pdu[8], pdu[7], pdu[6], pdu[5], pdu[4], pdu[3]);
    int i = 9;
    while (i < len) {
      int curlen = pdu[i];
      i++;
      if (curlen == 0) {
	continue;
      }
      int curtyp = pdu[i];
      log_uart("(%d: ", curtyp);
      for (int j = 1; j < curlen; j++) {
	if (curtyp == 9) {
	  log_uart("%c", pdu[i+j]);
	} else {
	  log_uart("%02x ", pdu[i+j]);
	}
      }
      i += curlen;
      log_uart(") ");
    }
    log_uart("\n");
  }
                                                 
  uint8_t i;
  uint8_t f = pdu[1] + 3;
  log_uart("Payload: ");
  for (i = 3; i < f; i++) {
    log_uart("0x%02x ", pdu[i]);
  }
  log_uart("\n");

  log_uart("Raw: ");
  for (i = 0; i < MAX_PDU_SIZE; i++) {
    log_uart("0x%02x ", pdu[i]);
  }
  log_uart("\n");
}

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

void scan_channel(int channel, uint32_t window, uint8_t* pdu) {
  NRF_RADIO->DATAWHITEIV = channel & 0x3F;

  memset(pdu, 0, MAX_PDU_SIZE);
  NRF_RADIO->FREQUENCY = ch2freq(channel);

  NRF_RADIO->EVENTS_READY = 0UL;
  NRF_RADIO->TASKS_RXEN = 1UL;
  while (NRF_RADIO->EVENTS_READY == 0UL);

  NRF_RADIO->EVENTS_END = 0UL;
  NRF_RADIO->TASKS_START = 1UL;
  while (NRF_RADIO->EVENTS_END == 0UL && (NRF_RTC0->COUNTER - window < SCAN_WINDOW));

  NRF_RADIO->EVENTS_DISABLED = 0UL;
  NRF_RADIO->TASKS_DISABLE = 1UL;
  while (NRF_RADIO->EVENTS_DISABLED == 0UL);

  if (NRF_RADIO->EVENTS_END == 1UL) {
    dbg_packet(channel, pdu);
  }
}

#define START_TIMERS(t1, t2) do { t1 = t2 = NRF_RTC0->COUNTER; } while (0)


void wait_interval(uint32_t interval) {
  uint32_t  tmp = SCAN_INTERVAL - RTC_PERIOD * (NRF_RTC0->COUNTER - interval);
  if (tmp <= SCAN_INTERVAL) {
    nrf_delay_ms(tmp);
  }
}

static void setup(uint8_t* pdu) {
  /* Start UART logging module. */
  log_uart_init();

  /* Start high frequency clock (16 MHz). */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0UL;
  NRF_CLOCK->TASKS_HFCLKSTART = 1UL;
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0UL);

  /* Start low frequency clock (32.768 kHz). */
  NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos;
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);

  /* Start Real Timer Counter 0 (RTC0). */
  NRF_RTC0->PRESCALER = RTC_PRESCALER;
  NRF_RTC0->TASKS_START = 1UL;

  /* Start to configure the RADIO.
   *
   * We clear PCNF0 and CPNF1 registers to use OR operations in the next
   * operations.
   */
  NRF_RADIO->PCNF0 = 0UL;
  NRF_RADIO->PCNF1 = 0UL;

  /* Set RADIO mode to Bluetooth Low Energy. */
  NRF_RADIO->MODE = RADIO_MODE_MODE_Ble_1Mbit << RADIO_MODE_MODE_Pos;

  /* Set transmission power to 0dBm. */
    NRF_RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_0dBm
      << RADIO_TXPOWER_TXPOWER_Pos;    

    /* Set access address to 0x8E89BED6. This is the access address to be used
     * when send packets in advertise channels.
     *
     * Since the access address is 4 bytes long and the prefix is 1 byte long,
     * we first set the base address length to be 3 bytes long.
     *
     * Then we split the full access address in:
     * 1. Prefix0:  0x0000008E (LSB -> Logic address 0)
     * 2. Base0:    0x89BED600 (3 MSB)
     *
     * At last, we enable reception for this address.
     */
    NRF_RADIO->PCNF1        |= 3UL << RADIO_PCNF1_BALEN_Pos;
    NRF_RADIO->BASE0        = 0x89BED600;
    NRF_RADIO->PREFIX0      = 0x0000008E;
    NRF_RADIO->RXADDRESSES  = 0x00000001;

    /* Enable data whitening. */
    NRF_RADIO->PCNF1 |= RADIO_PCNF1_WHITEEN_Enabled << RADIO_PCNF1_WHITEEN_Pos;

    /* Set maximum PAYLOAD size. */
    NRF_RADIO->PCNF1 |= MAX_PDU_SIZE << RADIO_PCNF1_MAXLEN_Pos;

    /* Configure CRC.
     *
     * First, we set the length of CRC field to 3 bytes long and ignore the
     * access address in the CRC calculation.
     *
     * Then we set CRC initial value to 0x555555.
     *
     * The last step is to set the CRC polynomial to
     * x^24 + x^10 + x^9 + x^6 + x^4 + x^3 + x + 1.
     */
    NRF_RADIO->CRCCNF =     RADIO_CRCCNF_LEN_Three << RADIO_CRCCNF_LEN_Pos |
                            RADIO_CRCCNF_SKIP_ADDR_Skip
						   << RADIO_CRCCNF_SKIP_ADDR_Pos;
    NRF_RADIO->CRCINIT =    0x555555UL;
    NRF_RADIO->CRCPOLY =    SET_BIT(24) | SET_BIT(10) | SET_BIT(9) |
      SET_BIT(6) | SET_BIT(4) | SET_BIT(3) |
      SET_BIT(1) | SET_BIT(0);

    /* Configure header size.
     *
     * The Advertise has the following format:
     * PDU Type(4b) | RFU(2b) | TxAdd(1b) | RxAdd(1b) | Length(6b) | RFU(2b)
     *
     * And the nRF51822 RADIO packet has the following format
     * (directly editable fields):
     * S0 (0/1 bytes) | LENGTH ([0, 8] bits) | S1 ([0, 8] bits)
     *
     * We can match those fields with the Link Layer fields:
     * S0 (1 byte)      --> PDU Type(4bits)|RFU(2bits)|TxAdd(1bit)|RxAdd(1bit)
     * LENGTH (6 bits)  --> Length(6bits)
     * S1 (2 bits)      --> S1(2bits)
     */
    NRF_RADIO->PCNF0 |= (1 << RADIO_PCNF0_S0LEN_Pos) |  /* 1 byte */
      (6 << RADIO_PCNF0_LFLEN_Pos) |  /* 6 bits */
      (2 << RADIO_PCNF0_S1LEN_Pos);   /* 2 bits */

    /* Set the pointer to write the incoming packet. */
    NRF_RADIO->PACKETPTR = (uint32_t) pdu;
}

int main(void)
{
  uint32_t window = 0, interval = 0;
  uint8_t pdu[MAX_PDU_SIZE];
  memset(pdu, 0, sizeof(pdu));
  setup(pdu);

  while (1) {
    log_uart("Scanning...\n");
    // log_uart("Scanning with wnd: %u ticks, interval: %u ticks\n",
    //     SCAN_WINDOW, SCAN_INTERVAL);

    /* Advertising channel 37 */
    START_TIMERS(interval, window);
    scan_channel(37, window, pdu);
    wait_interval(interval);

    /* Advertising channel 38 */
    START_TIMERS(interval, window);
    scan_channel(38, window, pdu);
    wait_interval(interval);

    /* Advertising channel 39 */
    START_TIMERS(interval, window);
    scan_channel(39, window, pdu);
    wait_interval(interval);
  }
}
