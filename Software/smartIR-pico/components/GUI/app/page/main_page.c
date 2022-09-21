#include "main_page.h"
#include "fan_page.h"
#include "setting_page.h"
#include "ac_page.h"

#include "lv_common.h"
#include <stdio.h>

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


// /* wifi动图集 */
// const lv_img_dsc_t *wifi_imgs[5] = {
//     &logo_wifi_off,
//     &logo_wifi_0,
//     &logo_wifi_1,
//     &logo_wifi_2,
//     &logo_wifi_full,
// };

// /* 电池动图集 */
// const lv_img_dsc_t *bat_imgs[8] = {
//     &logo_bat_chargefailed,
//     &logo_bat_warning,
//     &logo_bat_charging,
//     &logo_bat_0,
//     &logo_bat_1,
//     &logo_bat_2,
//     &logo_bat_3,
//     &logo_bat_4,
// };

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

/**
 * @brief 移除主页对象
 *
 */
static void remove_main_page_obj(void){
    lv_obj_del(btn_ac);
    lv_obj_del(btn_fan);
    lv_obj_del(astronaut);
    lv_obj_del(btn_setting);
    // lv_obj_del(top_info_bar);
}


/* 动画animing */
// void anim_mainpage_in(uint32_t delay){
//     // obj_set_anim(btn_fan, -80, 5, 1000, delay + 500, anim_x_cb, NULL, lv_anim_path_bounce);
//     // obj_set_anim(btn_ac, 80, -5, 1000, delay + 500, anim_x_cb, NULL, lv_anim_path_bounce);
//     // obj_set_anim(astronaut, -80, 5, 1000, delay + 500, anim_x_cb, NULL, lv_anim_path_bounce);
// }
// void anim_mainpage_out(uint32_t delay){
//     obj_set_anim(btn_fan, 240, lv_obj_get_x(btn_fan)+50, 5000, delay, anim_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
//     obj_set_anim(btn_ac, lv_obj_get_x(btn_ac), 0, 500, delay, anim_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
//     obj_set_anim(astronaut, 0, lv_obj_get_x(astronaut)+50, 500, delay, anim_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_overshoot);
// }


/* 按钮事件 */
static void btn_fan_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        printf("fan click\n");
        remove_main_page_obj();
        show_fan_page();
        // anim_mainpage_in(0);
    }
}
static void btn_ac_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        remove_main_page_obj();
        printf("ac click\n");
        show_ac_page();
    }
}
static void btn_setting_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        printf("setting click\n");
        remove_main_page_obj();
        show_setting_page();
    }
}


/* 宇航员动图 */
static void astronaut_anim(void){
    lv_obj_t *animing_astronaut = lv_animimg_create(astronaut);
    lv_obj_center(animing_astronaut);
    lv_animimg_set_src(animing_astronaut,  (lv_img_dsc_t**)astronaut_imgs, 7);
    lv_animimg_set_duration(animing_astronaut, 600);
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


/* 顶部状态栏 */

// void show_top_info_bar(void){
//     top_info_bar = lv_obj_create(bg_screen);

//     lv_obj_remove_style_all(top_info_bar);
//     lv_obj_set_size(top_info_bar,240,20);
//     // lv_style_set_color_filter_opa()
//     lv_obj_set_style_bg_opa(top_info_bar, LV_OPA_0,0);
//     // lv_obj_set_style_bg_color(top_info_bar,lv_color_white(),0);//lv_color_make(209, 154, 102)

//     //bar底部隔离条
//     lv_obj_set_style_border_color(top_info_bar,lv_color_make(136, 178, 251),0);
//     lv_obj_get_style_border_opa(top_info_bar,LV_OPA_50);
//     lv_obj_set_style_border_width(top_info_bar,1,0);
//     lv_obj_set_style_border_side(top_info_bar,LV_BORDER_SIDE_BOTTOM,0);

//     lv_obj_align(top_info_bar,LV_ALIGN_TOP_MID, 0,0);

//     show_test();
//     show_wifi_logo();
//     show_bat_logo();
// }



void create_main_page_btn(void){

    /* 空调按键 */
    btn_ac = lv_img_create(bg_screen);
    lv_obj_add_style(btn_ac, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn_ac, &style_outline_focused, LV_STATE_FOCUSED);
    // lv_obj_set_style_transform_zoom(btn_ac, 230, 0);
    lv_obj_add_flag(btn_ac, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_ac, btn_ac_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(btn_ac, &logo_ac);
    lv_obj_align(btn_ac, LV_ALIGN_BOTTOM_MID, 0,-70);
    // lv_obj_align_to(btn_ac, btn_setting, LV_ALIGN_CENTER, -70, 0);
    // lv_obj_align(btn_ac, LV_ALIGN_BOTTOM_MID, -50,-70);

    /* 设置按键 */
    btn_setting = lv_img_create(bg_screen);
    lv_obj_add_style(btn_setting, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn_setting, &style_outline_focused, LV_STATE_FOCUSED);
    // lv_obj_set_style_transform_zoom(btn_setting, 220, 0);
    lv_obj_add_flag(btn_setting, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_setting, btn_setting_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(btn_setting, &logo_setting);
    // lv_obj_align(btn_setting, LV_ALIGN_BOTTOM_MID, 0,-70);
    lv_obj_align_to(btn_setting, btn_ac, LV_ALIGN_CENTER, -70, 0);

    /* 风扇按键 */
    btn_fan = lv_img_create(bg_screen);
    lv_obj_add_style(btn_fan, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn_fan, &style_outline_focused, LV_STATE_FOCUSED);
    lv_obj_set_style_transform_zoom(btn_fan, 220, 0);
    lv_obj_add_flag(btn_fan, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_fan, btn_fan_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(btn_fan, &logo_fan);
    lv_obj_align_to(btn_fan, btn_ac, LV_ALIGN_CENTER, 70, 0);
    // lv_obj_align(btn_fan, LV_ALIGN_BOTTOM_MID, 50, -70);

    

}


// void show_main_page(void){
//     bg_page();
//     // lv_init_btn_style();
//     // create_main_page_btn();
//     gif_astronaut();        //太空人动图
// }

void show_main_page(void){
    lv_obj_t * hellotext = lv_label_create(lv_scr_act());
    lv_label_set_text(hellotext, "12345");
    lv_obj_center(hellotext);
}
