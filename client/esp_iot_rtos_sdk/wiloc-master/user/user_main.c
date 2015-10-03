/*
The MIT License (MIT)

Copyright (c) 2015 alu96

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <uart_protocol.h>
#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

#include "lwip/sockets.h"

#include "uart.h"
#include "uart_protocol.h"

#include "user_config.h"
#include "gpio.h"
#include "itstr.h"

// this field depends on total count of our nodes
#define MAX_INGORE_MACS 16

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define DBG(str, ...) printf(str "\n", ##__VA_ARGS__)

#define printmac(buf, i) printf("%02X:%02X:%02X:%02X:%02X:%02X", buf[i+0], buf[i+1], buf[i+2], buf[i+3], buf[i+4], buf[i+5])

xTimerHandle timer;

unsigned char ignored_macs[MAX_INGORE_MACS][6];
int ignored_macs_count = 0;

void uart_tx(unsigned char byte1, unsigned char byte2, char rssi, char *mac, unsigned char channel, unsigned int timestamp);

/**
 * UART TX packet structure:
 * | packet count : 8 | timestamp : 8 * 8 | rssi : 8 * 2 | end : 8 |
 */

unsigned char packet_cnt = 1;

void main_task(void *pvParameters) {
    DBG("main task...");

    gpio_init();

    // disable prom mode temporarily
    //wifi_promiscuous_enable(1);

    vTaskSuspend(NULL);
}

// UART RX parsing

typedef enum {
    CTRL_BYTE, DATA, END
} uart_rx_state;

unsigned char uart_ctrl;

bool valid_packet;
uart_data_struct data;
int data_len = 0;

uart_rx_state rx_state;

void uart_received() {
    for (int i = 0; i < data_len; i++) {
        printf("%02X", data.bytes[i]);
    }
}

void uart_parse(char c) {
    switch (rx_state) {
        case CTRL_BYTE:
            uart_ctrl = c;
            rx_state = DATA;
            valid_packet = true;
            break;

        case DATA:
            if (c == 0) {
                bool valid = false;
                switch (uart_ctrl) {
                    case CTRL_DATA:
                        if (data_len == sizeof(uart_data_struct)) {
                            valid = true;
                        }
                        break;
                }
                if (valid) {
                    uart_received();
                } else {
                    DBG("other length expected");
                }
                rx_state = CTRL_BYTE;
                break;
            }
            switch (uart_ctrl) {
                case CTRL_DATA:
                    if (data_len > sizeof(uart_data_struct)) {
                        valid_packet = false;
                        rx_state = END;
                        DBG("buffer overflow");
                    } else {
                        data.bytes[data_len++] = c;
                    }
                    break;
            }
            break;
        case END:
            if (c == 0) {
                rx_state = CTRL_BYTE;
                break;
            }

    }
}

void uart_rx(int len) {
    int i;

    for (i = 0; i < len; i++) {
        unsigned char c = uart_rx_one_char(UART);

        uart_parse(c);

        printf("rx:%d", uart_rx_one_char(UART));
    }
}



/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void) {
    system_update_cpu_freq(160);
    uart_init_new(BAUD, uart_rx);

    printf("setting cpu freq to 160 MHz\n");

    printf("SDK version:%s\n", system_get_sdk_version());

    unsigned char mac[6];

    wifi_get_macaddr(STATION_IF, mac);
    printf("MAC-STA:");
    printmac(mac, 0);
    printf("\n");

    wifi_get_macaddr(SOFTAP_IF, mac);
    printf("MAC- AP:");
    printmac(mac, 0);
    printf("\n");

#ifdef DOUBLE_CLK_FREQ
    system_update_cpu_freq(160);
#endif


    DBG(" ---- set opmode");
    if (!wifi_set_opmode(STATIONAP_MODE)) {
        DBG(" ---- > failed to set opmode");
    }
    DBG(" ---- done");

    xTaskCreate(main_task, "main", 256, NULL, 2, NULL);

    DBG(" ---- vTaskStartScheduler");

    //vTaskStartScheduler();
}

#pragma clang diagnostic pop