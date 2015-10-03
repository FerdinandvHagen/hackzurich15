#ifndef WILOC_COMMON_UART_PROTOCOL_H
#define WILOC_COMMON_UART_PROTOCOL_H

// available control bytes

#define CTRL_DATA 1

typedef union {
    struct {
        unsigned char count;
        unsigned char identifier;
        unsigned char timestamp[8];
        unsigned char channel[2];
        unsigned char rssi[2];
        unsigned char mac[6][2];
        unsigned char byte1[2];
        unsigned char byte2[2];
    };
    unsigned char bytes[1 + 1 + 8 + 2 + 2 + 6 * 2 + 2 + 2];
} uart_data_struct;

typedef struct {
    unsigned char count;
    int timestamp;
    unsigned char channel;
    char rssi;
    unsigned char mac[6];
    unsigned char byte1;
    unsigned char byte2;
} uart_data_packet;

#endif //WILOC_COMMON_UART_PROTOCOL_H
