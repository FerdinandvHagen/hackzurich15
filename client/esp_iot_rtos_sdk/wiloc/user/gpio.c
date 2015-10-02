#include "gpio.h"
#include "espressif/esp_common.h"

void ICACHE_FLASH_ATTR gpio_signal(int count) {
    while (count-- > 0) {
        gpio_output_set(BIT2, 0, BIT2, 0);
        gpio_output_set(0, BIT2, BIT2, 0);
    }
}

void ICACHE_FLASH_ATTR gpio_init() {
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO2_U);

    gpio_output_set(0, BIT2, BIT2, 0);
}