// #include <stdio.h>
#include "ac_page.h"

#include "main_page.h"
#include "lv_common.h"
#include "anim.h"

#include "esp_log.h"

#define TAG "ac_page"

LV_FONT_DECLARE(font_LMYY);
LV_FONT_DECLARE(HmOS_35px);
LV_IMG_DECLARE(logo_kaiguan);

//user data
bool acswitch = false;     //后面需要给其他任务extern
int16_t acTemperature = 26;


static lv_obj_t * ac_return;    //返回键
static lv_obj_t * ac_switch;    //开关键
static lv_obj_t * ac_arc;       //圆弧条
static lv_obj_t * ac_add;       //+
static lv_obj_t * ac_cut;       //+
static lv_obj_t * text_temp;      //温度显示
static lv_obj_t * ac_speed;     //风速按钮
static lv_obj_t * ac_timing;    //定时按钮



/* 按钮回调函数 */
static void return_mainpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "return main page");
        ac_page_anim_out(200);
        main_page_anim_in(200);
    }
}
static void ac_switch_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(acswitch){
            ESP_LOGI(TAG, "ac: open");
        }else{
            ESP_LOGI(TAG, "ac: close");
        }
        acswitch = !acswitch;
    }
}
static void ac_arc_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED){
        acTemperature = lv_arc_get_value(ac_arc);
        lv_label_set_text_fmt(text_temp, "#88B2FB %d°", acTemperature);
        ESP_LOGI(TAG, "ac: %d", lv_arc_get_value(ac_arc));
    }
}
static void ac_add_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        acTemperature = lv_arc_get_value(ac_arc) + 1;
        if(acTemperature >32){
            acTemperature = 32;
        }
        lv_arc_set_value(ac_arc, acTemperature);
        lv_label_set_text_fmt(text_temp, "#88B2FB %d°", acTemperature);
        ESP_LOGI(TAG, "ac temp add: %d",lv_arc_get_value(ac_arc));
    }
}
static void ac_cut_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED){
        acTemperature = lv_arc_get_value(ac_arc) - 1;
        if(acTemperature < 16){
            acTemperature = 16;
        }
        lv_arc_set_value(ac_arc, acTemperature);
        lv_label_set_text_fmt(text_temp, "#88B2FB %d°", acTemperature);
        ESP_LOGI(TAG, "ac temp cut: %d",lv_arc_get_value(ac_arc));
    }
}



/*************************
 *      绘制页面内容
 *************************/

/* 显示风扇文字标题 */
// void show_title_ac(void){
//     ac_title = lv_label_create(bg_screen);
//     lv_label_set_recolor(ac_title, true);
//     lv_label_set_text(ac_title, "#88B2FB 空调");
//     lv_obj_set_style_text_font(ac_title, &font_LMYY,0);
//     lv_obj_align(ac_title, LV_ALIGN_TOP_MID, 0, 10);  //位置
// }

/* 温度圆弧 */
void create_ac_arc(void){
    ac_arc = lv_arc_create(bg_screen);
    lv_obj_set_size(ac_arc, 150, 150);
    lv_obj_align(ac_arc, LV_ALIGN_CENTER, 0, -30);
    // lv_obj_align_to(ac_arc, ac_switch, LV_ALIGN_CENTER, 0, 20);


    lv_obj_remove_style(ac_arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(ac_arc, lv_color_hex(0x88B2FB), LV_PART_INDICATOR);   //圆弧前景颜色
    lv_obj_set_style_arc_color(ac_arc, lv_color_hex(0xf34c44), LV_PART_MAIN);        //圆弧背景颜色

    lv_arc_set_rotation(ac_arc, 160);       //到0°的偏移量
    lv_arc_set_bg_angles(ac_arc, 0, 220);
    // lv_arc_set_angles();
    lv_arc_set_value(ac_arc, acTemperature);       //得到空调值 保存到nvs
    lv_arc_set_range(ac_arc, 16, 32);
    // lv_obj_remove_style(ac_arc, NULL, LV_PART_KNOB);
    lv_obj_add_event_cb(ac_arc, ac_arc_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_clear_flag(ac_arc, LV_OBJ_FLAG_CLICKABLE);

    //圆弧温度
    text_temp = lv_label_create(ac_arc);
    lv_label_set_recolor(text_temp, true);
    // lv_label_set_text(ac_temp, "#88B2FB 26°");
    lv_label_set_text_fmt(text_temp, "#88B2FB %d°", acTemperature);
    lv_obj_set_style_text_font(text_temp, &font_LMYY, 0);
    lv_obj_align_to(text_temp, ac_arc, LV_ALIGN_BOTTOM_MID, 10, 0);
    // lv_obj_align(ac_temp,LV_ALIGN_BOTTOM_MID,0,70);

}

void create_ac_btn(void){
    /* 返回键 */
    ac_return = lv_img_create(bg_screen);
    // lv_obj_set_style_bg_opa(ac_return, LV_OPA_0, LV_STATE_DEFAULT);
    // lv_obj_set_style_radius(ac_return, 7, LV_STATE_PRESSED);

    // lv_obj_set_style_bg_color(ac_return, lv_color_black(),LV_STATE_PRESSED);
    // lv_obj_set_style_bg_opa(ac_return, LV_OPA_100, LV_STATE_PRESSED);
    lv_obj_set_style_translate_y(ac_return, 5, LV_STATE_PRESSED);

    lv_obj_align(ac_return, LV_ALIGN_TOP_LEFT, 10, 15);
    // lv_obj_set_size(ac_return, 43, 20);
    // lv_obj_set_size(ac_return, 60, 21);
    lv_obj_add_flag(ac_return, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ac_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_ext_click_area(ac_return, 20);
    LV_IMG_DECLARE(logo_return);
    lv_img_set_src(ac_return, &logo_return); 


    /* 风扇开关 */
    // ac_switch = create_click_imgbtn(bg_screen, ac_switch_cb, LV_ALIGN_CENTER, 0, 60);
    ac_switch = lv_img_create(bg_screen);
    lv_obj_add_style(ac_switch, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(ac_switch, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ac_switch, ac_switch_cb, LV_EVENT_CLICKED, NULL);
    // lv_obj_align_to(ac_switch, ac_arc, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(ac_switch, LV_ALIGN_CENTER, 0, -30);

    lv_img_set_src(ac_switch, &logo_kaiguan);


    /* 风速按钮 */
    ac_speed = lv_btn_create(bg_screen);
    lv_obj_remove_style(ac_speed, NULL, LV_PART_MAIN);
    lv_obj_add_style(ac_speed, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ac_speed, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ac_speed, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ac_speed, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(ac_speed, 60, 40);
    lv_obj_align(ac_speed, LV_ALIGN_BOTTOM_MID, 50, -40);
    // lv_obj_add_event_cb(ac_speed, return_mainpage_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_speed = lv_label_create(ac_speed);
    lv_label_set_recolor(text_speed, true);
    lv_label_set_text(text_speed, "#88B2FB 风速");
    lv_obj_set_style_text_font(text_speed, &font_LMYY, 0);
    // lv_label_set_text(text_speed, "#88B2FB Speed");
    // lv_obj_set_style_text_font(text_speed, &HmOS_35px, 0);
    lv_obj_center(text_speed);


    /* 定时按钮 */
    ac_timing = lv_btn_create(bg_screen);
    lv_obj_remove_style(ac_timing, NULL, LV_PART_MAIN);
    lv_obj_add_style(ac_timing, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ac_timing, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ac_timing, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ac_timing, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(ac_timing, 60, 40);
    lv_obj_align(ac_timing, LV_ALIGN_BOTTOM_MID, -50, -40);
    // lv_obj_add_event_cb(ac_speed, return_mainpage_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_timing = lv_label_create(ac_timing);
    lv_label_set_recolor(text_timing, true);
    lv_label_set_text(text_timing, "#88B2FB 定时");
    lv_obj_set_style_text_font(text_timing, &font_LMYY, 0);
    // lv_label_set_text(text_timing, "#88B2FB Timing");
    // lv_obj_set_style_text_font(text_timing, &HmOS_35px, 0);
    lv_obj_center(text_timing);
    

    /* + button */
    ac_add = create_text_btn(bg_screen);
    lv_obj_align(ac_add, LV_ALIGN_CENTER, 80, 35);
    lv_obj_add_event_cb(ac_add, ac_add_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(ac_add, 30, 30);
    lv_obj_t * text_add = lv_label_create(ac_add);
    lv_label_set_recolor(text_add, true);
    lv_label_set_text(text_add, "#88B2FB +");
    lv_obj_set_style_text_font(text_add, &font_LMYY, 0);
    // lv_obj_set_style_text_font(text_add, &HmOS_35px, 0);
    
    lv_obj_center(text_add);
    

    /* - button */
    ac_cut = create_text_btn(bg_screen);
    lv_obj_align(ac_cut, LV_ALIGN_CENTER, -80, 35);
    lv_obj_add_event_cb(ac_cut, ac_cut_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(ac_cut, 30, 30);
    lv_obj_t * text_cut = lv_label_create(ac_cut);
    lv_label_set_recolor(text_cut, true);
    lv_label_set_text(text_cut, "#88B2FB -");
    lv_obj_set_style_text_font(text_cut, &font_LMYY, 0);
    // lv_obj_set_style_text_font(text_cut, &HmOS_35px, 0);
    lv_obj_center(text_cut);
}



/*************************
 *        页面动画
 *************************/

void ac_page_anim_in(uint32_t delay){
    // anim_y_fade_in(ac_title,-50, 10, delay,NULL);
    anim_y_fade_in(ac_return,-50, 15, delay);
    anim_y_fade_in(ac_switch,-100, -30, delay);
    anim_step_in(ac_arc, 200);
    anim_step_in(ac_add, 200);
    anim_step_in(ac_cut, 200);
    anim_step_in(text_temp, 200);

    anim_x_fade_in(ac_timing, -100, -50, delay);
    anim_x_fade_in(ac_speed, 100, 50, delay);

}

void ac_page_anim_out(uint32_t delay){
    // anim_y_fade_out(ac_title, lv_obj_get_y(ac_title), -50, delay,lv_obj_del_anim_ready_cb);
    anim_y_fade_out(ac_return, 15, -50, delay);

    anim_y_fade_out(ac_switch, -30, -100, delay);

    anim_step_out(ac_arc, 200);
    anim_step_out(ac_add, 200);
    anim_step_out(ac_cut, 200);
    anim_step_out(text_temp, 200);

    anim_x_fade_out(ac_timing, -50, -100, delay);
    anim_x_fade_out(ac_speed, 50, 100, delay);
}



/*************************
 *        页面API
 *************************/

void ac_page_hide_obj(void){
    lv_obj_add_flag(ac_return, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ac_switch, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ac_timing, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ac_speed, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ac_arc, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ac_add, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ac_cut, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(text_temp, LV_OBJ_FLAG_HIDDEN);
}

void ac_page_create_obj(void){
    create_ac_arc();
    create_ac_btn();

    ac_page_hide_obj();
}