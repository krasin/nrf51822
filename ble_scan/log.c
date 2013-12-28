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

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "boards.h"
#include "nrf_error.h"
#include "nrf_delay.h"
#include "simple_uart.h"
#include "ble.h"
#include "log.h"

#define BUFFER_LEN 128

static char buffer[BUFFER_LEN];

void log_uart_init(void)
{
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER,
                        CTS_PIN_NUMBER, RX_PIN_NUMBER,
                        HWFC);

    nrf_delay_ms(1);
    INIT_LOG();
}

void log_uart(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf(buffer, BUFFER_LEN, format, args);
    simple_uart_putstring((const uint8_t *) buffer);

    va_end(args);
}

const char *nrf_error_to_str(uint32_t error_code)
{
    switch (error_code) {
    case NRF_SUCCESS:
        return "NRF_SUCCESS";
    case NRF_ERROR_SVC_HANDLER_MISSING:
        return "NRF_ERROR_SVC_HANDLER_MISSING";
    case NRF_ERROR_SOFTDEVICE_NOT_ENABLED:
        return "NRF_ERROR_SOFTDEVICE_NOT_ENABLED";
    case NRF_ERROR_INTERNAL:
        return "NRF_ERROR_INTERNAL";
    case NRF_ERROR_NO_MEM:
        return "NRF_ERROR_NO_MEM";
    case NRF_ERROR_NOT_FOUND:
        return "NRF_ERROR_NOT_FOUND";
    case NRF_ERROR_NOT_SUPPORTED:
        return "NRF_ERROR_NOT_SUPPORTED";
    case NRF_ERROR_INVALID_PARAM:
        return "NRF_ERROR_INVALID_PARAM";
    case NRF_ERROR_INVALID_STATE:
        return "NRF_ERROR_INVALID_STATE";
    case NRF_ERROR_INVALID_LENGTH:
        return "NRF_ERROR_INVALID_LENGTH";
    case NRF_ERROR_INVALID_FLAGS:
        return "NRF_ERROR_INVALID_FLAGS";
    case NRF_ERROR_INVALID_DATA:
        return "NRF_ERROR_INVALID_DATA";
    case NRF_ERROR_DATA_SIZE:
        return "NRF_ERROR_DATA_SIZE";
    case NRF_ERROR_TIMEOUT:
        return "NRF_ERROR_TIMEOUT";
    case NRF_ERROR_NULL:
        return "NRF_ERROR_NULL";
    case NRF_ERROR_FORBIDDEN:
        return "NRF_ERROR_FORBIDDEN";
    case NRF_ERROR_INVALID_ADDR:
        return "NRF_ERROR_INVALID_ADDR";
    case NRF_ERROR_BUSY:
        return "NRF_ERROR_BUSY";
    default:
        return "UNKNOWN_ERROR";
    }
}

const char *nrf_evt_to_str(uint16_t evt_id)
{
    switch (evt_id) {
    /* BLE independent events */
    case BLE_EVT_TX_COMPLETE:
        return "BLE_EVT_TX_COMPLETE";

    /* BLE GAP events */
    case BLE_GAP_EVT_CONNECTED:
        return "BLE_GAP_EVT_CONNECTED";
    case BLE_GAP_EVT_DISCONNECTED:
        return "BLE_GAP_EVT_DISCONNECTED";
    case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        return "BLE_GAP_EVT_CONN_PARAM_UPDATE";
    case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
        return "BLE_GAP_EVT_SEC_PARAMS_REQUEST";
    case BLE_GAP_EVT_SEC_INFO_REQUEST:
        return "BLE_GAP_EVT_SEC_INFO_REQUEST";
    case BLE_GAP_EVT_PASSKEY_DISPLAY:
        return "BLE_GAP_EVT_PASSKEY_DISPLAY";
    case BLE_GAP_EVT_AUTH_KEY_REQUEST:
        return "BLE_GAP_EVT_AUTH_KEY_REQUEST";
    case BLE_GAP_EVT_AUTH_STATUS:
        return "BLE_GAP_EVT_AUTH_STATUS";
    case BLE_GAP_EVT_CONN_SEC_UPDATE:
        return "BLE_GAP_EVT_CONN_SEC_UPDATE";
    case BLE_GAP_EVT_TIMEOUT:
        return "BLE_GAP_EVT_TIMEOUT";
    case BLE_GAP_EVT_RSSI_CHANGED:
        return "BLE_GAP_EVT_RSSI_CHANGED";

    /* BLE GATT Client events */
    case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
        return "BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP";
    case BLE_GATTC_EVT_REL_DISC_RSP:
        return "BLE_GATTC_EVT_REL_DISC_RSP";
    case BLE_GATTC_EVT_CHAR_DISC_RSP:
        return "BLE_GATTC_EVT_CHAR_DISC_RSP";
    case BLE_GATTC_EVT_DESC_DISC_RSP:
        return "BLE_GATTC_EVT_DESC_DISC_RSP";
    case BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP:
        return "BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP";
    case BLE_GATTC_EVT_READ_RSP:
        return "BLE_GATTC_EVT_READ_RSP";
    case BLE_GATTC_EVT_CHAR_VALS_READ_RSP:
        return "BLE_GATTC_EVT_CHAR_VALS_READ_RSP";
    case BLE_GATTC_EVT_WRITE_RSP:
        return "BLE_GATTC_EVT_WRITE_RSP";
    case BLE_GATTC_EVT_HVX:
        return "BLE_GATTC_EVT_HVX";
    case BLE_GATTC_EVT_TIMEOUT:
        return "BLE_GATTC_EVT_TIMEOUT";

    /* BLE GATT Server events */
    case BLE_GATTS_EVT_WRITE:
        return "BLE_GATTS_EVT_WRITE";
    case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        return "BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST";
    case BLE_GATTS_EVT_SYS_ATTR_MISSING:
        return "BLE_GATTS_EVT_SYS_ATTR_MISSING";
    case BLE_GATTS_EVT_HVC:
        return "BLE_GATTS_EVT_HVC";
    case BLE_GATTS_EVT_SC_CONFIRM:
        return "BLE_GATTS_EVT_SC_CONFIRM";
    case BLE_GATTS_EVT_TIMEOUT:
        return "BLE_GATTS_EVT_TIMEOUT";

    /* BLE L2CAP events */
    case BLE_L2CAP_EVT_RX:
        return "BLE_L2CAP_EVT_RX";

    /* Unknown */
    case BLE_EVT_INVALID:
        return "BLE_EVT_INVALID";
    default:
        sprintf(buffer, "#%u", evt_id);
        return (const char *) buffer;
    }
}
