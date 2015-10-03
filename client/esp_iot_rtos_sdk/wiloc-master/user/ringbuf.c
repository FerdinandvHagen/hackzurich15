#include "ringbuf.h"

/**
* \brief init a ringbuf object
* \param r pointer to a ringbuf object
* \param buf pointer to a byte array
* \param size size of buf
* \return 0 if successful, otherwise failed
*/
int ICACHE_FLASH_ATTR ringbuf_init(ringbuf *r, unsigned char *buf, int size) {
    if (r == NULL || buf == NULL || size < 2) return -1;

    r->p_o = r->p_r = r->p_w = buf;
    r->fill_cnt = 0;
    r->size = size;

    return 0;
}

/**
* \brief put a character into ring buffer, overwrite if buffer is already full
* \param r pointer to a ringbuf object
* \param c character to be put
* \return 0 if successful, otherwise failed
*/
int ICACHE_FLASH_ATTR ringbuf_owr(ringbuf *r, unsigned char c) {
    unsigned char c_old;
    if (ringbuf_put(r, c)) {
        // ringbuf is full
        ringbuf_get(r, &c_old);
        ringbuf_put(r, c);
        return -1;
    }
    return 0;
}

/**
* \brief put a character into ring buffer
* \param r pointer to a ringbuf object
* \param c character to be put
* \return 0 if successful, otherwise failed
*/
int ICACHE_FLASH_ATTR ringbuf_put(ringbuf *r, unsigned char c) {
    if (r->fill_cnt >= r->size)
        return -1; // ring buffer is full, this should be atomic operation

    r->fill_cnt++;

    *r->p_w++ = c;

    if (r->p_w >= r->p_o + r->size) // rollback if write pointer go pass
        r->p_w = r->p_o;            // the physical boundary

    return 0;
}

/**
* \brief get a character from ring buffer
* \param r pointer to a ringbuf object
* \param c read character
* \return 0 if successful, otherwise failed
*/
int ICACHE_FLASH_ATTR ringbuf_get(ringbuf *r, unsigned char *c) {
    if (r->fill_cnt <= 0)
        return -1; // ring buffer is empty, this should be atomic operation

    r->fill_cnt--;

    *c = *r->p_r++;

    if (r->p_r >= r->p_o + r->size) // rollback if write pointer go pass
        r->p_r = r->p_o;            // the physical boundary

    return 0;
}

/**
* \brief remove all characters from ringbuffer
* \param r pointer to a ringbuf object
* \return 0 if successful, otherwise failed
*/
int ICACHE_FLASH_ATTR ringbuf_truncate(ringbuf *r) {
    if (r == NULL) {
        return -1;
    }

    r->fill_cnt = 0;
    r->p_r = r->p_w = r->p_o; // reset read and write pointer to beginning

    return 0;
}
