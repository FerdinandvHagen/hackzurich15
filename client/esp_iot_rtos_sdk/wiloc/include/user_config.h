#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define BAUD 38400

//#define CHANNEL_HOP_ENABLED
#define CHANNEL_HOP_INTERVAL 50

// hardware configuration
// DEV:
//
// DEPLOY

#define DEV

#ifdef DEV

#define UART UART0

#endif

#endif