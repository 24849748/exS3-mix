#ifndef _LED_H_
#define _LED_H_

#include "driver/gpio.h"


esp_err_t led_init(gpio_num_t pin, uint32_t level);

void led_on(gpio_num_t pin);
void led_off(gpio_num_t pin);

void led_blink(gpio_num_t pin);


#endif
