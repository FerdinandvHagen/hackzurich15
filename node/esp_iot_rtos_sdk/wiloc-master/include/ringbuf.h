#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include "c_types.h"

typedef struct{
    unsigned char* p_o;				    /**< Original pointer */
    unsigned char* volatile p_r;		/**< Read pointer */
    unsigned char* volatile p_w;		/**< Write pointer */
    volatile int fill_cnt;	            /**< Number of filled slots */
    int size;				            /**< Buffer size */
} ringbuf;

int ICACHE_FLASH_ATTR ringbuf_init(ringbuf *r, unsigned char *buf, int size);
int ICACHE_FLASH_ATTR ringbuf_owr(ringbuf *r, unsigned char c);
int ICACHE_FLASH_ATTR ringbuf_put(ringbuf *r, unsigned char c);
int ICACHE_FLASH_ATTR ringbuf_get(ringbuf *r, unsigned char *c);
int ICACHE_FLASH_ATTR ringbuf_truncate(ringbuf *r);

#endif
