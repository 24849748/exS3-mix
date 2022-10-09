/**
 * @file disp_driver.h
 * @author bosco (1270112821@qq.com)
 * @brief 显示器硬件驱动，连接lvgl，可能不需要
 * @version 0.1
 * @date 2022-09-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DISP_DRIVER_H
#define DISP_DRIVER_H

#include "lvgl.h"

/* Initialize display */
void driver_disp_init(void);
// void *disp_driver_init(void);


/* Display flush callback */
void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);

void tft_backlight_init(void);

#endif
