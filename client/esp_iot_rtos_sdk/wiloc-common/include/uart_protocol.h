#ifndef WILOC_COMMON_UART_PROTOCOL_H
#define WILOC_COMMON_UART_PROTOCOL_H

typedef union {
    struct {
        unsigned char count;
        unsigned char timestamp[8];
        unsigned char channel;
        unsigned char rssi[2];
        unsigned char mac[6][2];
        unsigned char byte1;
        unsigned char byte2;
    };
    unsigned char bytes[sizeof(1 + 8 + 1 + 2 + 6 * 2 + 1 + 1)];
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
