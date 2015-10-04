#ifndef ESP8266_OBD_ITSTR_H
#define ESP8266_OBD_ITSTR_H

#ifndef bool
#include <stdbool.h>
#endif

int logllu(unsigned long long int value, int base);

unsigned long long int powllu(int b, int e);

int logi(int value, int base);

int powi(int b, int e);

bool llutstr(char *buffer, int buffer_len, unsigned long long int n, int z, int lz);

bool itstr(char *buffer, int buffer_len, int n, int z, int lz);

#endif //ESP8266_OBD_ITSTR_H