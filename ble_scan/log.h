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

#ifndef __LOG_H__
#define __LOG_H__

void log_uart_init(void);
void log_uart(const char *format, ...);
const char *nrf_error_to_str(uint32_t error_code);
const char *nrf_evt_to_str(uint16_t evt_id);

#define INIT_LOG()                                                          \
    do {                                                                    \
        log_uart("\r\n");                                                   \
    } while (0)

#define LOG(level, fmt, arg...)                                             \
    do {                                                                    \
        log_uart(level ":%s:%s() " fmt "\r\n",                              \
                __FILE__, __func__, ## arg);                                \
    } while (0)

#define DBG(fmt, arg...)                                                    \
    do {                                                                    \
        LOG("DEBUG", fmt, ## arg);                                          \
    } while (0)

#define ERROR(fmt, arg...)                                                  \
    do {                                                                    \
        LOG("ERROR", fmt, ## arg);                                          \
    } while (0)

#define NRF_ERROR(file, line, err)                                          \
    do {                                                                    \
        log_uart("ERROR:%s:%lu %s\r\n", file, line, nrf_error_to_str(err)); \
    } while (0)

#endif
