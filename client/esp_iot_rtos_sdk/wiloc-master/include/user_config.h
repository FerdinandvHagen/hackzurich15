#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define BAUD 460800

#define SSID "AUFVH"
#define PASSWORD "ferdinandvonhagen"

// hardware configuration
// DEV:
//
// DEPLOY

#define DEV

#ifdef DEV

#define UART UART0

#endif

#endif