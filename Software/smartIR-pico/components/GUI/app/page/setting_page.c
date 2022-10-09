#include "setting_page.h"
// #include "bg_page.h"
#include "main_page.h"
#include "lv_common.h"

#include "esp_log.h"
#define TAG "setting_page"

#include <stdio.h>

LV_FONT_DECLARE(font_LMYY);

static lv_obj_t * btn_return;
static lv_obj_t * hello_text;
static lv_obj_t * title_setting;

void remove_setting_page_obj(void){
    lv_obj_del(btn_return);
    lv_obj_del(hello_text);
    lv_obj_del(title_setting);
}


/* 按钮回调函数 */
static void return_mainpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        //led_blink(PIN_LED);
        // anim_mainpage_out(0);
        ESP_LOGI(TAG, "return main page");
        remove_setting_page_obj();
        show_main_page();
    }
}

/* 显示风扇文字标题 */
void show_title_setting(void){
    title_setting = lv_label_create(bg_screen);
    lv_label_set_recolor(title_setting, true);
    lv_label_set_text(title_setting, "#88B2FB 设置");
    lv_obj_set_style_text_font(title_setting,&font_LMYY,0);
    lv_obj_align(title_setting,LV_ALIGN_TOP_MID,0,10);  //位置
}


/* 滚动文本 */
void show_test(void){
    hello_text = lv_label_create(bg_screen);
    lv_label_set_long_mode(hello_text, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(hello_text, 50);
    lv_label_set_recolor(hello_text, true);
    lv_label_set_text(hello_text, "   #fcfcfc 我 爱 林 彦 柠 !#");  //设置颜色
    lv_obj_align(hello_text,LV_ALIGN_BOTTOM_MID,0,0);  //位置

    LV_FONT_DECLARE(font_LMYY);
    lv_obj_set_style_text_font(hello_text,&font_LMYY,0);
}

void create_setting_btn(void){
        /* 返回按钮 */
    // btn_return = lv_btn_create(bg_screen);
    // lv_obj_remove_style_all(btn_return);
    // lv_obj_add_style(btn_return, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(btn_return,lv_color_hex(0x3d3037),LV_STATE_DEFAULT);
    // lv_obj_align(btn_return,LV_ALIGN_TOP_LEFT,0,0);
    // lv_obj_set_size(btn_return, 40,30);
    // lv_obj_add_flag(btn_return, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_event_cb(btn_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    // lv_obj_t * return_text = lv_label_create(btn_return);
    // lv_label_set_text(return_text," < ");
    // lv_obj_center(return_text);

    btn_return = create_text_btn(bg_screen);
    lv_obj_add_style(btn_return, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_align(btn_return, LV_ALIGN_TOP_LEFT,10,10);
    lv_obj_add_event_cb(btn_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(btn_return, 40, 36);
    lv_obj_set_ext_click_area(btn_return, 30);
    lv_obj_t * text_return = lv_label_create(btn_return);
    lv_label_set_recolor(text_return, true);
    lv_label_set_text(text_return, "#88B2FB <");
    lv_obj_set_style_text_font(text_return, &font_LMYY, 0);
    lv_obj_center(text_return);

    // btn_return = lv_label_create(bg_screen);
    // lv_obj_add_style(btn_return, &style_btn_pressed, LV_STATE_PRESSED);
    // lv_obj_set_style_bg_color(btn_return,lv_color_hex(0x21252b),LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(btn_return,LV_OPA_COVER,LV_STATE_DEFAULT);
    // lv_obj_set_style_radius(btn_return, 5, LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_color(btn_return,lv_color_hex(0x202020),LV_STATE_PRESSED);
    // lv_obj_set_style_bg_opa(btn_return,LV_OPA_COVER,LV_STATE_PRESSED);
    // lv_obj_set_style_radius(btn_return, 5, LV_STATE_PRESSED);

    // lv_obj_align(btn_return, LV_ALIGN_TOP_LEFT,5,5);
    // lv_obj_set_size(btn_return, 36,30);
    // lv_obj_add_flag(btn_return, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_event_cb(btn_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    // lv_label_set_recolor(btn_return,true);
    // lv_label_set_text(btn_return,"#88B2FB   < ");
    // lv_obj_set_style_text_font(btn_return, &font_LMYY, 0);

}

void show_setting_page(void){
    show_title_setting();
    show_test();
    create_setting_btn();
}