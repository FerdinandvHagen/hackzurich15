#include <iostream>
#include "itstr.h"
#include "../esp_iot_rtos_sdk/wiloc-common/include/uart_protocol.h"

using namespace std;

#define DBG(str, ...) printf(str "\n", ##__VA_ARGS__)

#define UART 0

/// ---------------------

typedef enum {
    CTRL_BYTE, DATA, END
} uart_rx_state;

unsigned char uart_ctrl;

uart_data_struct data;
int data_len = 0;

uart_rx_state rx_state = CTRL_BYTE;

void uart_received() {
    for (int i = 0; i < data_len; i++) {
        printf("%02X", data.bytes[i]);
    }

    uart_data_packet packet;

    printf("%s\n", data.bytes);

    //packet.count = data.count;

}

void uart_parse(char c) {
    switch (rx_state) {
        case CTRL_BYTE:
            uart_ctrl = c;
            rx_state = DATA;
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

void uart_tx_one_char(int uart, char c) {
    printf("%02X", c);

    uart_parse(c);
}

// ----------------------------

void uart_tx(int timestamp, char rssi, char *mac);


int logllu(unsigned long long int value, int base) {
    int i = 0;
    while (value > 0) {
        value /= (unsigned long long int)base;
        i++;
    }
    return i;
}

unsigned long long int powllu(int b, int e) {
    unsigned long long int r = 1;

    while (e-- > 0)
        r *= (unsigned long long int)b;

    return r;
}

int logi(int value, int base) {
    int i = 0;
    while (value > 0) {
        value /= base;
        i++;
    }
    return i;
}

int powi(int b, int e) {
    int r = 1;

    while (e-- > 0)
        r *= b;

    return r;
}


char tokenChars[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

bool llutstr(char *buffer, int buffer_len, unsigned long long int n, int z, int lz) {
    if (z > 1 && z <= sizeof(tokenChars)) {
        unsigned long long int k;
        int size, i, s;
        int pointer = 0;

        s = logllu(n, z);

        if (lz > s)
            size = lz;
        else
            size = s;

        if (buffer_len < size) {
            return false;
        }

        for (i = lz - s; i > 0; i--) {
            buffer[pointer++] = '0';
        }

        for (i = s - 1; i >= 0; i--) {
            k = n / powllu(z, i);
            buffer[pointer++] = tokenChars[k];
            n -= k * powllu(z, i);
        }

        buffer[pointer] = 0;

        return true;
    }

    return false;
}

bool itstr(char *buffer, int buffer_len, int n, int z, int lz) {
    if (z > 1 && z <= sizeof(tokenChars)) {
        int k;
        int size, i, s;
        int pointer = 0;

        s = logi(n, z);

        if (lz > s)
            size = lz;
        else
            size = s;

        if (buffer_len < size) {
            return false;
        }

        for (i = lz - s; i > 0; i--) {
            buffer[pointer++] = '0';
        }

        for (i = s - 1; i >= 0; i--) {
            k = n / powi(z, i);
            buffer[pointer++] = tokenChars[k];
            n -= k * powi(z, i);
        }

        buffer[pointer] = 0;

        return true;
    }

    return false;
}

/// ---------------------

unsigned char packet_cnt = 1;
void uart_tx(unsigned char byte1, unsigned char byte2, char rssi, char *mac, unsigned char channel, unsigned int timestamp) {
    uart_data_struct tx;

    tx.count = packet_cnt;
    itstr((char *) tx.timestamp, sizeof(tx.timestamp), timestamp, 16, sizeof(tx.timestamp));
    itstr((char *) tx.channel, 2, channel, 16, 2);
    itstr((char *) tx.rssi, sizeof(tx.rssi), rssi, 16, sizeof(tx.rssi));

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

int main() {
    cout << "Hello, World!" << endl;

    unsigned char mac[6];
    int i;

    for (int i; i < 6; i++) {
        mac[i] = (char) i;
    }

    uart_tx(0xff, 0xee, -14, (char *) mac, 10, 0x10203040);

    return 0;
}