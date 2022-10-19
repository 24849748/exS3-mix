#ifndef _VMOTOR_H_
#define _VMOTOR_H_


#include "driver/gpio.h"

esp_err_t vmotor_init(gpio_num_t pin, uint32_t level);

void vmotor_on(gpio_num_t pin);
void vmotor_off(gpio_num_t pin);
void vmotor_blink(gpio_num_t pin);


#endif
