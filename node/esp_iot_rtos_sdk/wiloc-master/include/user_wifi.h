#ifndef ESP8266_OBD_USER_WIFI_H
#define ESP8266_OBD_USER_WIFI_H

typedef void (*wifi_connect_cb)();

void user_wifi_init(wifi_connect_cb cb);

#endif //ESP8266_OBD_USER_WIFI_H
