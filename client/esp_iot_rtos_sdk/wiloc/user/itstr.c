/*
 * integer to string conversion
 */
#include "itstr.h"

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
