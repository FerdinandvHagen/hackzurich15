#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define BAUD 460800 //3686400

//#define CHANNEL_HOP_ENABLED
#define CHANNEL_HOP_INTERVAL 50

#define FIXED_CHANNEL 1

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