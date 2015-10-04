#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define BAUD 460800 //3686400

#define SSID "AUFVH"
#define PASSWORD "ferdinandvonhagen"

#define SERVER_IP "192.168.188.20"
#define SERVER_PORT 3003

#define DOUBLE_CLK_FREQ

// hardware configuration
// DEV:
//
// DEPLOY

//#define DEV

#ifdef DEV

#define UART UART0

#else

#define UART UART0

#endif

#endif