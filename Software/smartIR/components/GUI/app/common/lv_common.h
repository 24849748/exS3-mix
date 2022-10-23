/**
 * @file lv_common.h
 * @author bosco (1270112821@qq.com)
 * @brief 项目共同使用的部分都放在这，如背景、风格、动画、函数等
 * @version 0.1
 * @date 2022-09-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __LV_COMMON_H_
#define __LV_COMMON_H_

#include "lvgl.h"

//--------------------background-------------------------//

#define BG_COLOR lv_color_hex(0x2c313c) //vscode atom theme background
// #define BG_COLOR lv_color_hex(0xffcc3e)
#define TEXT_COLOR lv_color_hex(0x21252b)
#define BTNBG_COLOR lv_color_hex(0x181821)

extern lv_obj_t *bg_screen;

void bg_page(void);

//----------------------style----------------------------//
extern lv_style_t style_btn_pressed;
extern lv_style_t style_return_btn_default;
extern lv_style_t style_outline_focused;

extern lv_style_t style_btn;
extern lv_style_t style_btn_pr;


void lv_init_btn_style(void);
void init_custom_btn_style(void);


//----------------------func-----------------------------//
lv_obj_t * create_click_imgbtn(lv_obj_t * parent, lv_event_cb_t event_cb, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

lv_obj_t * create_text_btn(lv_obj_t * parent);

lv_obj_t * lv_create_button_c(lv_obj_t * parent, lv_coord_t size_w, lv_coord_t size_h, lv_align_t align, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const char *text);


#endif
