#include "lv_common.h"
// #include "bg_page.h"

//--------------------background-------------------------//
lv_obj_t *bg_screen;

void bg_page(void){
    lv_obj_t *scr = lv_scr_act();

    bg_screen = lv_obj_create(scr);
    lv_obj_remove_style_all(bg_screen);
    lv_obj_set_size(bg_screen, 240, 320);
    lv_obj_set_style_radius(bg_screen,2,0); //圆角设置
    lv_obj_set_style_bg_opa(bg_screen,LV_OPA_COVER,0);
    lv_obj_set_style_bg_color(bg_screen, BG_COLOR,0);
    lv_obj_center(bg_screen);
}

//----------------------style----------------------------//
lv_style_t style_btn_pressed;
lv_style_t style_return_btn_default;
lv_style_t style_outline_focused;

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

//----------------------func-----------------------------//
/* 创建图像按钮 */
lv_obj_t * create_click_imgbtn(lv_obj_t * parent, lv_event_cb_t event_cb, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs){
    lv_obj_t * obj = lv_img_create(parent);
    lv_obj_add_style(obj, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(obj, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(obj, align, x_ofs,y_ofs);
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

    // lv_obj_t * text_speed = lv_label_create(ac_speed);
    // lv_label_set_recolor(text_speed, true);
    // lv_label_set_text(text_speed, "#88B2FB 风速");
    // lv_obj_set_style_text_font(text_speed, &font_LMYY, 0);

    return obj;
}

//----------------------anim-----------------------------//

/**
 * @brief 动画在x轴上运动
 * 
 * @param var 
 * @param v 
 */
void anim_x_cb(void * var, int32_t v){
    lv_obj_set_x(var, v);
}
/**
 * @brief 动画在y轴上运动
 * 
 * @param var 
 * @param v 
 */
void anim_y_cb(void * var, int32_t v){
    lv_obj_set_y(var, v);
}

/**
 * @brief 设置对象动画
 * 
 * @param obj       需要添加动画的对象
 * @param start     开始值
 * @param end       结束值
 * @param duration  动画时长
 * @param delay     开始执行动画前的等待时长
 * @param exec_cb   anim_x_cb 或 anim_y_cb，执行动画运动的轴
 * @param ready_cb  动画ready完的回调函数
 * @param path_cb   内置路径动画函数
 *                  - lv_anim_path_linear/ease_in/ease_out/ease_in_out/overshoot/bounce/step
 * 
 */
void obj_set_anim(lv_obj_t *obj, 
                int32_t start, 
                int32_t end, 
                uint32_t duration, 
                uint32_t delay, 
                lv_anim_exec_xcb_t exec_cb, 
                lv_anim_ready_cb_t ready_cb, 
                lv_anim_path_cb_t path_cb){
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, start, end);
    lv_anim_set_time(&a, duration);
    lv_anim_set_delay(&a, delay);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)exec_cb);
    lv_anim_set_ready_cb(&a, ready_cb);
    lv_anim_set_path_cb(&a, path_cb);
    lv_anim_start(&a);
}



