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
#include "monitor_mode.h"

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

void promisc_cb(uint8 *buf, uint16 len) {
    gpio_signal(2);

    printf("%d -> %3d: %d \n", system_get_time(), wifi_get_channel(), len);

    //Extract the first to bytes from the system
    uint8 byte1;
    uint8 byte2;
    char mac[6];
    char rssi;
    uint8 channel;

    if (len != 12) {
        if (len == 128) {
            struct sniffer_buf2 *sb = (struct sniffer_buf2 *) buf;
            byte1 = sb->buf[0];
            byte2 = sb->buf[1];
            rssi = sb->rx_ctrl.rssi;
            channel = (uint8) sb->rx_ctrl.channel;

            //MAC-Address is just always the second address in the packet because this is always the direct Transmission Address
            int i;
            for (i = 0; i < 6; ++i) {
                mac[i] = sb->buf[10 + i];
            }
        } else {
            struct sniffer_buf *sb = (struct sniffer_buf *) buf;
            byte1 = sb->buf[0];
            byte2 = sb->buf[1];
            rssi = sb->rx_ctrl.rssi;
            channel = (uint8) sb->rx_ctrl.channel;

            //MAC-Address is just always the second address in the packet because this is always the direct Transmission Address
            int i;
            for (i = 0; i < 6; ++i) {
                mac[i] = sb->buf[10 + i];
            }
        }

        uart_tx(byte1, byte2, rssi, mac, wifi_get_channel(), system_get_time());
    } else {
        printf("Packet not convertible. Ignoring");
    }

    gpio_signal(1);
}

/**
 * UART TX packet structure:
 * | packet count : 8 | timestamp : 8 * 8 | rssi : 8 * 2 | end : 8 |
 */

unsigned char packet_cnt = 1;

void uart_tx(unsigned char byte1, unsigned char byte2, char rssi, char *mac, unsigned char channel, unsigned int timestamp) {
    uart_data_struct tx;

    tx.count = packet_cnt;
    tx.identifier = 1; // TODO

    itstr((char *) tx.timestamp, sizeof(tx.timestamp), timestamp, 16, sizeof(tx.timestamp));
    itstr((char *) tx.channel, 2, channel, 16, 2);
    itstr((char *) tx.rssi, sizeof(tx.rssi), (unsigned char)rssi, 16, sizeof(tx.rssi));

    int i;
    for (i = 0; i < 6; i++) {
        itstr((char *) tx.mac[i], 2, mac[i], 16, 2);
    }

    itstr((char *) tx.byte1, 2, byte1, 16, 2);
    itstr((char *) tx.byte2, 2, byte2, 16, 2);

    // send control byte

    uart_tx_one_char(UART, CTRL_DATA);

    // send bytes constructed so far and termination byte

    for (i = 0; i < sizeof(tx.bytes); i++) {
        uart_tx_one_char(UART, tx.bytes[i]);
    }

    uart_tx_one_char(UART, 0);

    packet_cnt++;
    if (!packet_cnt) {
        packet_cnt = 1;
    }
}

void channelHop(xTimerHandle pxTimer) {
    // 1 - 13 channel hopping
    uint8 new_channel = 10; //wifi_get_channel() % 13 + 1;
    DBG("  --- hop to %d", new_channel);
    wifi_set_channel(new_channel);
}

void main_task(void *pvParameters) {
    DBG("main task...");

    gpio_init();

    //wifi_promiscuous_enable(1);

    unsigned char mac[6];
    int i;

    for (i; i < 6; i++) {
        mac[i] = (char) i;
    }

    while (1) {
        uart_tx(0xff, 0xee, -14, (char *) mac, 10, 0x10203040);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    vTaskSuspend(NULL);
}

// UART RX parsing

typedef enum {
    WAITING, CTRL_BYTE, DATA, END
};

void uart_rx(int len) {

    int i;

    for (i = 0; i < len; i++) {
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

#ifdef DEV
    system_uart_swap();
#endif

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

    DBG("----- monitor mode test");

    DBG(" ---- set opmode");
    if (!wifi_set_opmode(0x1)) {
        DBG(" ---- > failed to set opmode");
    }
    DBG(" ---- done");

    DBG(" ---- monitor mode setup");

    vTaskDelay(100 / portTICK_RATE_MS);
    wifi_station_disconnect();
    vTaskDelay(100 / portTICK_RATE_MS);

    wifi_set_promiscuous_rx_cb(promisc_cb);

    timer = xTimerCreate("channel_hopping", CHANNEL_HOP_INTERVAL / portTICK_RATE_MS, pdTRUE, (void *) 0, channelHop);

#ifdef CHANNEL_HOP_ENABLED
    if (timer) {
        if (xTimerStart(timer, 0) != pdPASS) {
            DBG(" ---- failed to start timer");
        }
    } else {
        DBG(" ---- failed to create timer");
    }
#endif

    DBG(" ---- done");

    xTaskCreate(main_task, "main", 256, NULL, 2, NULL);

    DBG(" ---- vTaskStartScheduler");

    //vTaskStartScheduler();
}

#pragma clang diagnostic pop