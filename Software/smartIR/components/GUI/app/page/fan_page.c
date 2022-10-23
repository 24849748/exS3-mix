// #include <stdio.h>
#include "fan_page.h"
#include "main_page.h"
#include "anim.h"
#include "lv_common.h"

#include "esp_log.h"

#define TAG "fan_page"


LV_FONT_DECLARE(font_LMYY);
LV_IMG_DECLARE(logo_kaiguan);


//user data
bool fanswitch = false;     //后面需要给其他任务extern

static lv_obj_t * fan_title;
static lv_obj_t * fan_speed;
static lv_obj_t * fan_switch;
static lv_obj_t * fan_return;
static lv_obj_t * fan_swing;
static lv_obj_t * fan_wet;
static lv_obj_t * fan_class;
static lv_obj_t * bg_arc;



void fan_page_anim_in(uint32_t delay){
    anim_y_fade_in(fan_title,-50, 10, delay,NULL);
    anim_y_fade_in(fan_return,-50, 10, delay, NULL);

    anim_y_fade_in(fan_switch,-50, 0, delay, NULL);

    anim_step_in(bg_arc, 200);
    anim_step_in(fan_wet, 200);
    anim_step_in(fan_class, 200);
    anim_step_in(fan_swing, 200);
    anim_step_in(fan_speed, 200);
}


void fan_page_anim_out(uint32_t delay){
    anim_y_fade_out(fan_title,lv_obj_get_y(fan_title), -50, delay,lv_obj_del_anim_ready_cb);
    anim_y_fade_out(fan_return,lv_obj_get_y(fan_return), -50, delay,lv_obj_del_anim_ready_cb);

    anim_y_fade_out(fan_switch, 0, -50, delay,lv_obj_del_anim_ready_cb);

    anim_step_out(bg_arc, 200);
    anim_step_out(fan_wet, 200);
    anim_step_out(fan_class, 200);
    anim_step_out(fan_swing, 200);
    anim_step_out(fan_speed, 200);

}


/* 按钮回调函数 */
static void return_mainpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        ESP_LOGI(TAG, "return main page");
        fan_page_anim_out(200);
        show_main_page();
    }
}
static void fan_switch_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(fanswitch){
            ESP_LOGI(TAG, "fan: open");
        }else{
            ESP_LOGI(TAG, "fan: close");
        }
        fanswitch = !fanswitch;
    }
}


static void fan_class_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "fan: class");
    }
}
static void fan_wet_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "fan: wet");
    }
}
static void fan_swing_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "fan: swing");
    }
}
static void fan_speed_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "fan: speed");
    }
}



/* 显示风扇文字标题 */
void show_fan_title(void){
    fan_title = lv_label_create(bg_screen);
    lv_label_set_recolor(fan_title, true);
    lv_label_set_text(fan_title, "#88B2FB 风扇");
    lv_obj_set_style_text_font(fan_title,&font_LMYY,0);
    lv_obj_align(fan_title,LV_ALIGN_TOP_MID,0,10);  //位置
}

void create_bg_circle(void){
    bg_arc = lv_arc_create(bg_screen);
    lv_arc_set_rotation(bg_arc, 0);
    lv_arc_set_bg_angles(bg_arc, 0, 360);
    lv_obj_remove_style(bg_arc, NULL, LV_PART_KNOB);
    lv_obj_remove_style(bg_arc, NULL, LV_PART_INDICATOR);
    lv_obj_clear_flag(bg_arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(bg_arc);

    lv_obj_set_size(bg_arc,210,210);
    lv_obj_set_style_arc_width(bg_arc, 60, LV_PART_MAIN);
    lv_obj_set_style_arc_color(bg_arc, lv_color_hex(0x242424), LV_PART_MAIN);
}


void create_fan_text_btn2(void){
    /* 开关按钮 */
    fan_switch = create_click_imgbtn(bg_screen, fan_switch_cb, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(fan_switch, &logo_kaiguan);

    fan_return = create_text_btn(bg_screen);
    lv_obj_add_style(fan_return, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_align(fan_return, LV_ALIGN_TOP_LEFT,10,10);
    lv_obj_add_event_cb(fan_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(fan_return, 40, 36);
    lv_obj_set_ext_click_area(fan_return, 30);
    lv_obj_t * text_return = lv_label_create(fan_return);
    lv_label_set_recolor(text_return, true);
    lv_label_set_text(text_return, "#88B2FB <");
    lv_obj_set_style_text_font(text_return, &font_LMYY, 0);
    lv_obj_center(text_return);

    /* 风速按钮 */
    fan_speed = lv_btn_create(bg_screen);
    lv_obj_remove_style(fan_speed, NULL, LV_PART_MAIN);
    lv_obj_add_style(fan_speed, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(fan_speed, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(fan_speed, lv_color_hex(0x242424), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_speed, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_speed, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_speed, 60, 40);
    // lv_obj_align(fan_speed, LV_ALIGN_LEFT_MID, 30, 20);
    lv_obj_align_to(fan_speed, fan_switch, LV_ALIGN_CENTER, 0, 70);
    lv_obj_add_event_cb(fan_speed, fan_speed_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_speed = lv_label_create(fan_speed);
    lv_label_set_recolor(text_speed, true);
    lv_label_set_text(text_speed, "#88B2FB 风速");
    lv_obj_set_style_text_font(text_speed, &font_LMYY, 0);
    lv_obj_center(text_speed);



    /* 风类按钮 */
    fan_class = lv_btn_create(bg_screen);
    lv_obj_remove_style(fan_class, NULL, LV_PART_MAIN);
    lv_obj_add_style(fan_class, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(fan_class, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(fan_class, lv_color_hex(0x242424), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_class, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_class, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_class, 60, 40);
    // lv_obj_align(fan_class, LV_ALIGN_LEFT_MID, 30, 20);
    lv_obj_align_to(fan_class, fan_switch, LV_ALIGN_CENTER,-75, 0);
    lv_obj_add_event_cb(fan_class, fan_class_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_class = lv_label_create(fan_class);
    lv_label_set_recolor(text_class, true);
    lv_label_set_text(text_class, "#88B2FB 风类");
    lv_obj_set_style_text_font(text_class, &font_LMYY, 0);
    lv_obj_center(text_class);



    /* 摆风按钮 */
    fan_swing = lv_btn_create(bg_screen);
    lv_obj_remove_style(fan_swing, NULL, LV_PART_MAIN);
    lv_obj_add_style(fan_swing, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(fan_swing, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(fan_swing, lv_color_hex(0x242424), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_swing, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_swing, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_swing, 60, 40);
    // lv_obj_align_to(fan_swing, fan_class, LV_ALIGN_CENTER, 0, -60);
    lv_obj_align_to(fan_swing, fan_switch, LV_ALIGN_CENTER, 75, 0);
    lv_obj_add_event_cb(fan_swing, fan_swing_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_swing = lv_label_create(fan_swing);
    lv_label_set_recolor(text_swing, true);
    lv_label_set_text(text_swing, "#88B2FB 摆风");
    lv_obj_set_style_text_font(text_swing, &font_LMYY, 0);
    lv_obj_center(text_swing);



    /* 加湿按钮 */
    fan_wet = lv_btn_create(bg_screen);
    lv_obj_remove_style(fan_wet, NULL, LV_PART_MAIN);
    lv_obj_add_style(fan_wet, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(fan_wet, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(fan_wet, lv_color_hex(0x242424), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_wet, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_wet, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_wet, 60, 40);
    // lv_obj_align_to(fan_wet, fan_class, LV_ALIGN_CENTER, 0, 60);
    lv_obj_align_to(fan_wet, fan_switch, LV_ALIGN_CENTER, 0, -70);
    lv_obj_add_event_cb(fan_wet, fan_wet_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_wet = lv_label_create(fan_wet);
    lv_label_set_recolor(text_wet, true);
    lv_label_set_text(text_wet, "#88B2FB 加湿");
    lv_obj_set_style_text_font(text_wet, &font_LMYY, 0);
    lv_obj_center(text_wet);
}


void show_fan_page(void){
    lv_init_btn_style();
    create_bg_circle();
    show_fan_title();
    create_fan_text_btn2();
    fan_page_anim_in(200);
}