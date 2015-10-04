#include <espressif/c_types.h>

#include "esp_common.h"
#include <espressif/esp_wifi.h>
#include <espressif/esp_libc.h>
#include <espressif/esp_misc.h>
#include "user_wifi.h"

wifi_connect_cb cb;


bool first_connect = true;
bool connected = false;

// start wifi dependant services
void on_connect() {
    if (cb) {
        cb();
    }
}

// stop wifi dependant services
void on_disconnect() {

}

void wifi_handle_event_cb(System_Event_t *evt) {
    printf("event %x\n", evt->event_id);
    switch (evt->event_id) {
        case EVENT_STAMODE_CONNECTED:
            printf("connect to ssid %s, channel %d\n",
                   evt->event_info.connected.ssid,
                   evt->event_info.connected.channel);
            break;
        case EVENT_STAMODE_DISCONNECTED:
            printf("disconnect from ssid %s, reason %d\n",
                   evt->event_info.disconnected.ssid,
                   evt->event_info.disconnected.reason);

            if (connected) {
                connected = false;
                on_disconnect();
            }

            break;
        case EVENT_STAMODE_AUTHMODE_CHANGE:
            printf("mode: %d -> %d\n",
                   evt->event_info.auth_change.old_mode,
                   evt->event_info.auth_change.new_mode);
            break;
        case EVENT_STAMODE_GOT_IP:
            printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
                   IP2STR(&evt->event_info.got_ip.ip),
                   IP2STR(&evt->event_info.got_ip.mask),
                   IP2STR(&evt->event_info.got_ip.gw));
            printf("\n");

            first_connect = false;
            connected = true;

            on_connect();

            break;
        case EVENT_SOFTAPMODE_STACONNECTED:
            printf("station: " MACSTR "join, AID = %d\n",
                   MAC2STR(evt->event_info.sta_connected.mac),
                   evt->event_info.sta_connected.aid);
            break;
        case EVENT_SOFTAPMODE_STADISCONNECTED:
            printf("station: " MACSTR "leave, AID = %d\n", MAC2STR(evt->event_info.sta_disconnected.mac),
                   evt->event_info.sta_disconnected.aid);
            break;
        default:
            break;
    }
}

void ICACHE_FLASH_ATTR user_wifi_init(wifi_connect_cb callback) {
    wifi_set_event_handler_cb(wifi_handle_event_cb);

    cb = callback;
}