#ifndef _PWM_BACKLIGHT_H_
#define _PWM_BACKLIGHT_H_

#include <stdbool.h>

typedef void * pwm_backlight_handle_t;

typedef struct {
    bool pwm_control;
    bool output_invert;
    int gpio_num;
    
    int timer_idx;
    int channel_idx;
}pwm_backlight_config_t;

pwm_backlight_handle_t pwm_backlight_create(const pwm_backlight_config_t *config);

void pwm_backlight_set(pwm_backlight_handle_t bl, int brightness_percent);

void pwm_backlight_delete(pwm_backlight_handle_t bl);


#endif
