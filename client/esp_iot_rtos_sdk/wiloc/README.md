# esp8266_uart_bridge
Firmware for ESP8266 based on [esp_iot_rtos_sdk](https://github.com/espressif/esp_iot_rtos_sdk) to make UART port accessible over network. Currently only TCP connections are supported.

## Configuration
Configuration options are defined in `include/user_config.h`. Dynamic configuration after flashing is not supported yet.

## Building
Clone this repository inside esp_iot_rtos_sdk and change into cloned folder.
Make sure a compiler toolchain is installed, if not see [here](https://github.com/esp8266/esp8266-wiki/wiki/Toolchain).
To build the project, run `make`.
```
cd /path/to/esp_iot_rtos_sdk
git clone https://github.com/alu96/esp8266_uart_bridge.git
cd esp8266_uart_bridge
make
```

## Flashing
After a successful compilation, the firmware can be flashed with `flash.sh`. This script depends on [esptool](https://github.com/themadinventor/esptool) and might need some modifications before use.
