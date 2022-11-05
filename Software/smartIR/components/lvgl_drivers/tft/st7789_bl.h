#ifndef _ST7789_BL_H_
#define _ST7789_BL_H_

#include <stdbool.h>

typedef void * st7789_backlight_handle_t;

typedef struct {
    bool pwm_control;
    bool output_invert;
    int gpio_num;
    
    int timer_idx;
    int channel_idx;
}st7789_backlight_config_t;

st7789_backlight_handle_t pwm_backlight_create(const st7789_backlight_config_t *config);

void pwm_backlight_set(st7789_backlight_handle_t bl, int brightness_percent);

void pwm_backlight_delete(st7789_backlight_handle_t bl);


#endif
