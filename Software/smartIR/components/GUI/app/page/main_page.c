#include "main_page.h"
#include "fan_page.h"
#include "setting_page.h"
#include "ac_page.h"
#include "anim.h"
#include "lv_common.h"
#include <stdio.h>


#include "esp_log.h"
#define TAG "main_page"


LV_IMG_DECLARE(logo_setting);
LV_IMG_DECLARE(logo_fan);
LV_IMG_DECLARE(logo_ac);
LV_IMG_DECLARE(astronaut1);
// LV_IMG_DECLARE(astronaut2);
LV_IMG_DECLARE(astronaut3);
// LV_IMG_DECLARE(astronaut4);
LV_IMG_DECLARE(astronaut5);
// LV_IMG_DECLARE(astronaut6);
LV_IMG_DECLARE(astronaut7);
// LV_IMG_DECLARE(astronaut8);
LV_IMG_DECLARE(astronaut9);
// LV_IMG_DECLARE(astronaut10);
LV_IMG_DECLARE(astronaut11);
// LV_IMG_DECLARE(astronaut12);
LV_IMG_DECLARE(astronaut13);
// LV_IMG_DECLARE(astronaut14);


static lv_obj_t * astronaut;        //太空人动图 obj
static lv_obj_t * btn_fan;          //风扇界面按钮 obj
static lv_obj_t * btn_ac;           //空调界面按钮 obj
static lv_obj_t * btn_setting;      //设置界面按钮 obj


/* 太空人动图集 */
const lv_img_dsc_t *astronaut_imgs[7] = {
    &astronaut1,
    // &astronaut2,
    &astronaut3,
    // &astronaut4,
    &astronaut5,
    // &astronaut6,
    &astronaut7,
    // &astronaut8,
    &astronaut9,
    // &astronaut10,
    &astronaut11,
    // &astronaut12,
    &astronaut13,
    // &astronaut14,
};

/* 动画animing */
void main_page_anim_in(uint32_t delay){
    anim_y_fade_in(astronaut,-100, lv_obj_get_y(astronaut), delay, NULL);
    anim_y_fade_in(btn_ac, 100,-70, delay, NULL);
    anim_y_fade_in(btn_fan, 100,-70, delay, NULL);
    anim_y_fade_in(btn_setting, 100,-70, delay, NULL);
}

void main_page_anim_out(uint32_t delay){
    anim_y_fade_out(astronaut, lv_obj_get_y(astronaut),-100, delay, lv_obj_del_anim_ready_cb);
    anim_y_fade_out(btn_ac, -70, 100, delay, lv_obj_del_anim_ready_cb);
    anim_y_fade_out(btn_fan, -70, 100, delay, lv_obj_del_anim_ready_cb);
    anim_y_fade_out(btn_setting, -70, 100, delay, lv_obj_del_anim_ready_cb);
}



/* 按钮事件 */
static void btn_fan_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        ESP_LOGI(TAG, "enter fan page");
        main_page_anim_out(200);
        show_fan_page();
        // anim_mainpage_in(0);
    }
}
static void btn_ac_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        ESP_LOGI(TAG, "enter ac page");
        main_page_anim_out(200);
        show_ac_page();
    }
}
static void btn_setting_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        ESP_LOGI(TAG, "enter setting page");
        main_page_anim_out(200);
        show_setting_page();
    }
}


/* 宇航员动图 */
static void astronaut_anim(void){
    lv_obj_t *animing_astronaut = lv_animimg_create(astronaut);
    lv_obj_center(animing_astronaut);
    lv_animimg_set_src(animing_astronaut,  (lv_img_dsc_t**)astronaut_imgs, 7);
    lv_animimg_set_duration(animing_astronaut, 300);
    lv_animimg_set_repeat_count(animing_astronaut, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(animing_astronaut);
}
void gif_astronaut(void){
    astronaut = lv_obj_create(bg_screen);
    lv_obj_remove_style_all(astronaut);
    lv_obj_set_style_radius(astronaut,75,0);
    lv_obj_set_style_bg_color(astronaut,lv_color_black(),0);
    lv_obj_set_style_bg_opa(astronaut,LV_OPA_0,0);//opa 透明度
    lv_obj_align(astronaut,LV_ALIGN_TOP_MID,0,20);
    astronaut_anim();

    lv_obj_t *animing_astronaut = lv_animimg_create(astronaut);
    lv_obj_center(animing_astronaut);
}



void create_main_page_btn(void){

    /* 空调按键 */
    btn_ac = lv_img_create(bg_screen);
    lv_obj_add_style(btn_ac, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn_ac, &style_outline_focused, LV_STATE_FOCUSED);
    lv_obj_add_flag(btn_ac, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_ac, btn_ac_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(btn_ac, &logo_ac);
    lv_obj_align(btn_ac, LV_ALIGN_BOTTOM_MID, 0,-70);

    /* 设置按键 */
    btn_setting = lv_img_create(bg_screen);
    lv_obj_add_style(btn_setting, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn_setting, &style_outline_focused, LV_STATE_FOCUSED);
    lv_obj_add_flag(btn_setting, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_setting, btn_setting_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(btn_setting, &logo_setting);
    lv_obj_align(btn_setting, LV_ALIGN_BOTTOM_MID, -70, -70);

    /* 风扇按键 */
    btn_fan = lv_img_create(bg_screen);
    lv_obj_add_style(btn_fan, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn_fan, &style_outline_focused, LV_STATE_FOCUSED);
    lv_obj_add_flag(btn_fan, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_fan, btn_fan_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(btn_fan, &logo_fan);
    lv_obj_align(btn_fan, LV_ALIGN_BOTTOM_MID, 70,-70);
}


void show_main_page(void){
    lv_init_btn_style();
    create_main_page_btn();
    gif_astronaut();        //太空人动图
    main_page_anim_in(200);
}