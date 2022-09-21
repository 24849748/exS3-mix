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

#include "../../lvgl/lvgl.h"

//--------------------background-------------------------//

#define BG_COLOR lv_color_hex(0x2c313c) //vscode atom theme background
#define TEXT_COLOR lv_color_hex(0x21252b)


extern lv_obj_t *bg_screen;

void bg_page(void);

//----------------------style----------------------------//
extern lv_style_t style_btn_pressed;
extern lv_style_t style_return_btn_default;
extern lv_style_t style_outline_focused;
void lv_init_btn_style(void);


//----------------------func-----------------------------//
lv_obj_t * create_click_imgbtn(lv_obj_t * parent, lv_event_cb_t event_cb, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

lv_obj_t * create_text_btn(lv_obj_t * parent);

//----------------------anim-----------------------------//
void anim_x_cb(void * var, int32_t v);
void anim_y_cb(void * var, int32_t v);
void obj_set_anim(lv_obj_t *obj, 
                int32_t start, 
                int32_t end, 
                uint32_t time, 
                uint32_t delay, 
                lv_anim_exec_xcb_t exec_cb, 
                lv_anim_ready_cb_t ready_cb, 
                lv_anim_path_cb_t path_cb);






#endif
