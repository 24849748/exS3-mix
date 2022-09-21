#include "fan_page.h"
#include "main_page.h"

#include "lv_common.h"
#include <stdio.h>

LV_FONT_DECLARE(font_LMYY);
// LV_IMG_DECLARE(arrow_up);
// LV_IMG_DECLARE(arrow_down);
LV_IMG_DECLARE(logo_kaiguan);
// LV_IMG_DECLARE(logo_swing);
// LV_IMG_DECLARE(logo_class);
// LV_IMG_DECLARE(logo_wet);

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


/**
 * @brief 移除风扇页面对象
 * 
 */
static void remove_fan_page_obj(void){
    lv_obj_del(fan_title);
    // lv_obj_del(fan_up);
    // lv_obj_del(fan_down);
    lv_obj_del(fan_return);
    lv_obj_del(fan_swing);
    lv_obj_del(fan_switch);
    lv_obj_del(fan_wet);
    lv_obj_del(fan_class);
    lv_obj_del(fan_speed);
    lv_obj_del(bg_arc);
}


/* 按钮回调函数 */
static void return_mainpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        printf("return main page\n");
        remove_fan_page_obj();
        show_main_page();
    }
}
static void fan_switch_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(fanswitch){
            printf("fan: open\n");
        }else{
            printf("fan: close\n");
        }
        fanswitch = !fanswitch;
    }
}
#if 0
static void fan_up_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
    printf("fan: up\n");
    }
}
static void fan_down_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("fan: down\n");
    }
}
#endif


static void fan_class_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("fan: class\n");
    }
}
static void fan_wet_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("fan: wet\n");
    }
}
static void fan_swing_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("fan: swing\n");
    }
}
static void fan_speed_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("fan: speed\n");
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

    /* 返回按键 */
    fan_return = lv_label_create(bg_screen);
    lv_obj_add_style(fan_return, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(fan_return, &style_return_btn_default, LV_STATE_DEFAULT);
    lv_obj_align(fan_return, LV_ALIGN_TOP_LEFT,5,5);
    lv_obj_set_size(fan_return, 36,30);
    lv_obj_add_flag(fan_return, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_recolor(fan_return,true);
    lv_label_set_text(fan_return,"#88B2FB   < ");
    lv_obj_set_style_text_font(fan_return, &font_LMYY, 0);


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

#if 0
/* 文字按钮 */
void create_fan_text_btn(void){

    fan_switch = create_click_imgbtn(bg_screen, fan_switch_cb, LV_ALIGN_TOP_MID, 0, 45);
    lv_img_set_src(fan_switch, &logo_kaiguan);

    // /* 加 */
    // fan_up = lv_img_create(bg_screen);
    // lv_obj_add_style(fan_up, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_transform_zoom(fan_up, 200, 0);
    // lv_obj_add_flag(fan_up, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_event_cb(fan_up, fan_up_cb, LV_EVENT_CLICKED, NULL);
    // lv_img_set_src(fan_up, &arrow_up);
    // lv_obj_align(fan_up, LV_ALIGN_RIGHT_MID, -30, -40);

    // /* 减 */
    // fan_down = lv_img_create(bg_screen);
    // lv_obj_add_style(fan_down, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_transform_zoom(fan_down, 200, 0);
    // lv_obj_add_flag(fan_down, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_event_cb(fan_down, fan_down_cb, LV_EVENT_CLICKED, NULL);
    // lv_img_set_src(fan_down, &arrow_down);
    // lv_obj_align(fan_down, LV_ALIGN_RIGHT_MID, -30, 80);

    /* 返回按键 */
    fan_return = lv_label_create(bg_screen);
    lv_obj_add_style(fan_return, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(fan_return, &style_return_btn_default, LV_STATE_DEFAULT);
    lv_obj_align(fan_return, LV_ALIGN_TOP_LEFT,5,5);
    lv_obj_set_size(fan_return, 36,30);
    lv_obj_add_flag(fan_return, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_recolor(fan_return,true);
    lv_label_set_text(fan_return,"#88B2FB   < ");
    lv_obj_set_style_text_font(fan_return, &font_LMYY, 0);



    /* 风类按钮 */
    fan_class = lv_btn_create(bg_screen);
    lv_obj_remove_style(fan_class, NULL, LV_PART_MAIN);
    lv_obj_add_style(fan_class, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(fan_class, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_class, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_class, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_class, 60, 40);
    lv_obj_align(fan_class, LV_ALIGN_LEFT_MID, 30, 20);
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
    lv_obj_set_style_bg_color(fan_swing, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_swing, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_swing, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_swing, 60, 40);
    lv_obj_align_to(fan_swing, fan_class, LV_ALIGN_CENTER, 0, -60);
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
    lv_obj_set_style_bg_color(fan_wet, lv_color_hex(0x21252b), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(fan_wet, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(fan_wet, 5, LV_STATE_DEFAULT);
    lv_obj_set_size(fan_wet, 60, 40);
    lv_obj_align_to(fan_wet, fan_class, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(fan_wet, fan_wet_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * text_wet = lv_label_create(fan_wet);
    lv_label_set_recolor(text_wet, true);
    lv_label_set_text(text_wet, "#88B2FB 加湿");
    lv_obj_set_style_text_font(text_wet, &font_LMYY, 0);
    lv_obj_center(text_wet);

}

/* 按钮集合 */
void create_fan_btn(void){

    /* 风扇开关 */
    // fan_switch = lv_img_create(bg_screen);
    // lv_obj_add_style(fan_switch, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_add_flag(fan_switch, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_event_cb(fan_switch, fan_switch_cb, LV_EVENT_CLICKED, NULL);
    // lv_img_set_src(fan_switch, &logo_kaiguan);
    // lv_obj_align(fan_switch, LV_ALIGN_TOP_MID, 0,50);
    /* 风扇开关 */
    fan_switch = create_click_imgbtn(bg_screen, fan_switch_cb, LV_ALIGN_TOP_MID, 0, 35);
    lv_img_set_src(fan_switch, &logo_kaiguan);

    /* 加 */
    fan_up = lv_img_create(bg_screen);
    lv_obj_add_style(fan_up, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(fan_up, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_up, fan_up_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(fan_up, &arrow_up);
    lv_obj_align(fan_up, LV_ALIGN_RIGHT_MID, -35, -40);

    /* 减 */
    fan_down = lv_img_create(bg_screen);
    lv_obj_add_style(fan_down, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(fan_down, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_down, fan_down_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(fan_down, &arrow_down);
    lv_obj_align(fan_down, LV_ALIGN_RIGHT_MID, -35, 80);


    /* 返回按键 */
    fan_return = lv_label_create(bg_screen);
    lv_obj_add_style(fan_return, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(fan_return, &style_return_btn_default, LV_STATE_DEFAULT);
    lv_obj_align(fan_return, LV_ALIGN_TOP_LEFT,5,5);
    lv_obj_set_size(fan_return, 36,30);
    lv_obj_add_flag(fan_return, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_recolor(fan_return,true);
    lv_label_set_text(fan_return,"#88B2FB   < ");
    lv_obj_set_style_text_font(fan_return, &font_LMYY, 0);


    /* 风类按钮 */
    fan_class = lv_img_create(bg_screen);
    lv_obj_add_style(fan_class, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_transform_zoom(fan_class, 200, 0);
    lv_obj_add_flag(fan_class, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_class, fan_class_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(fan_class, &logo_class);
    lv_obj_align(fan_class, LV_ALIGN_LEFT_MID, 30, 20);
    // lv_obj_align_to(fan_class, fan_swing, LV_ALIGN_CENTER, 0, 60);

    /* 摆风按钮 */
    fan_swing = lv_img_create(bg_screen);
    lv_obj_add_style(fan_swing, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(fan_swing, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_swing, fan_swing_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(fan_swing, &logo_swing);
    // lv_obj_align(fan_swing, LV_ALIGN_LEFT_MID, 30, -50);
    lv_obj_align_to(fan_swing, fan_class, LV_ALIGN_CENTER, 0, -60);

    /* 加湿按钮 */
    fan_wet = lv_img_create(bg_screen);
    lv_obj_add_style(fan_wet, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_transform_zoom(fan_wet, 200, 0);
    lv_obj_add_flag(fan_wet, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(fan_wet, fan_wet_cb, LV_EVENT_CLICKED, NULL);
    lv_img_set_src(fan_wet, &logo_wet);
    lv_obj_align_to(fan_wet, fan_class, LV_ALIGN_CENTER, 0, 60);

}

#endif


void show_fan_page(void){
    lv_init_btn_style();
    create_bg_circle();
    show_fan_title();
    // create_fan_btn();
    // create_fan_text_btn();
    create_fan_text_btn2();
}