#include "lv_common.h"

//--------------------background-------------------------//
lv_obj_t *bg_screen;

void bg_page(void){
    lv_obj_t *scr = lv_scr_act();
    bg_screen = lv_obj_create(scr);
    lv_obj_remove_style_all(bg_screen);
    lv_obj_set_size(bg_screen, 240, 320);
    lv_obj_set_style_radius(bg_screen,2,0); //圆角设置
    lv_obj_set_style_bg_opa(bg_screen,LV_OPA_COVER,0);
    lv_obj_set_style_bg_color(bg_screen, BG_COLOR, 0);
    lv_obj_center(bg_screen);
}

//----------------------style----------------------------//
lv_style_t style_btn_pressed;
lv_style_t style_return_btn_default;
lv_style_t style_outline_focused;

lv_style_t style_btn;       //btn default style
lv_style_t style_btn_pr;    //pressed style

lv_style_t style_font;      //font style


void lv_init_btn_style(void){
    /* border,按下效果 */
    lv_style_init(&style_btn_pressed);
    lv_style_set_radius(&style_btn_pressed,7);
    lv_style_set_bg_color(&style_btn_pressed,lv_color_black());
    lv_style_set_bg_opa(&style_btn_pressed,LV_OPA_COVER);

    lv_style_set_border_color(&style_btn_pressed, lv_color_white());
    lv_style_set_border_opa(&style_btn_pressed, LV_OPA_COVER);
    // lv_style_set_border_side(&style_btn_border, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);
    lv_style_set_border_side(&style_btn_pressed, LV_BORDER_SIDE_FULL);
    lv_style_set_border_width(&style_btn_pressed, 2);



    /* default style */
    lv_style_init(&style_return_btn_default);
    lv_style_set_bg_color(&style_return_btn_default,lv_color_hex(0x21252b));
    lv_style_set_bg_opa(&style_return_btn_default, LV_OPA_COVER);
    lv_style_set_radius(&style_return_btn_default,7);



    /* focused style */
    lv_style_init(&style_outline_focused);
    lv_style_set_outline_color(&style_outline_focused, lv_color_hex(0xd19a66));
    lv_style_set_outline_width(&style_outline_focused, 2);
    lv_style_set_radius(&style_outline_focused, 7);
    lv_style_set_outline_pad(&style_outline_focused, 5);
}


/* 初始化自定义button style */
void init_custom_btn_style(void){
    /* default style */
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 7);             //背景圆角
    lv_style_set_bg_color(&style_btn, BTNBG_COLOR); //按钮背景颜色
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);  //无不透明

    lv_style_set_border_width(&style_btn, 0);       //无边框
    lv_style_set_outline_width(&style_btn, 0);      //无外框
    lv_style_set_shadow_width(&style_btn, 0);       //无阴影

    /* pressed style */
    lv_style_init(&style_btn_pr);
    lv_style_set_bg_color(&style_btn_pr, lv_color_black());     //纯黑背景

    /* font style */
    lv_style_init(&style_font);
    lv_style_set_text_color(&style_font, lv_color_white());
}


//----------------------func-----------------------------//
/* 创建图像按钮 */
lv_obj_t * create_click_imgbtn(lv_obj_t * parent, lv_event_cb_t event_cb, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs){
    lv_obj_t * obj = lv_img_create(parent);
    lv_obj_add_style(obj, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(obj, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(obj, align, x_ofs, y_ofs);
    return obj;
}

/**
 * @brief 创建文本按钮
 * @todo 设置位置、文本（颜色、字体）、回调函数
 * @param parent 父对象
 * @return 对象
 */
lv_obj_t * create_text_btn(lv_obj_t * parent){
    lv_obj_t * obj = lv_btn_create(parent);

    lv_obj_remove_style(obj, NULL, LV_PART_MAIN);
    //按下风格
    lv_obj_add_style(obj, &style_btn_pressed, LV_STATE_PRESSED);

    //bg 颜色
    lv_obj_set_style_bg_color(obj, TEXT_COLOR, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(obj, 60, 40);

    return obj;
}

/**
 * @brief 创建一个自定义风格的按钮, text需要自带颜色
 * 
 */
lv_obj_t * lv_create_button_c(lv_obj_t * parent, lv_coord_t size_w, lv_coord_t size_h, lv_align_t align, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const char *text){
    lv_obj_t * btn_obj = lv_btn_create(parent);
    lv_obj_remove_style_all(btn_obj);

    lv_obj_add_style(btn_obj, &style_btn, LV_STATE_DEFAULT);
    lv_obj_add_style(btn_obj, &style_btn_pr, LV_STATE_PRESSED);

    lv_obj_set_size(btn_obj, size_w, size_h);
    lv_obj_align(btn_obj, align, x, y);

    lv_obj_add_event_cb(btn_obj, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * info_label = lv_label_create(btn_obj);
    lv_label_set_recolor(info_label, true);
    lv_label_set_text(info_label, text);
    lv_obj_align(info_label, LV_ALIGN_TOP_LEFT, 5, 5);
    return btn_obj;
}

