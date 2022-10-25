#ifndef _VMOTOR_H_
#define _VMOTOR_H_


#include "driver/gpio.h"
#include "widgets_conf.h"

esp_err_t motor_init(gpio_num_t pin, uint32_t level);

void motor_on(gpio_num_t pin);
void motor_off(gpio_num_t pin);
void motor_reverse(gpio_num_t pin);
void motor_set(gpio_num_t pin, uint32_t level);


#endif
