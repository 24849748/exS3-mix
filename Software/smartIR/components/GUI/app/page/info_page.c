#include "info_page.h"
#include "main_page.h"
#include "anim.h"
#include "lv_common.h"
#include "esp_log.h"

#define TAG "info_page"

/* 坐标信息,对齐左上角LV_ALIGN_TOP_LEFT */
#define INFO_RETURN_Y       15   

#define INFO_LINE_X         25     //info line x坐标
#define FirstLineINFO_Y     60     //第一行 info y坐标
#define LastLineINFO_Y      200    //最后一行 info y坐标

static lv_obj_t * info_return;      //左上角返回键
static lv_obj_t * info_line;        //信息栏左边
static lv_obj_t * axp_info;         //axp173相关信息
// static lv_obj_t * axp_info1;         //axp173相关信息
// static lv_obj_t * axp_info2;         //axp173相关信息
static lv_obj_t * sys_info;         //系统相关信息



/* 按钮回调函数 */
static void return_mainpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "return main page");
        info_page_anim_out(200);
        main_page_anim_in(200);
    }
}


/*************************
 *      绘制页面内容
 *************************/

/* info 页面返回按键 */
void create_info_return_btn(void){
    /* 返回键 */
    info_return = lv_img_create(bg_screen);
    lv_obj_set_style_translate_y(info_return, 5, LV_STATE_PRESSED);
    lv_obj_align(info_return, LV_ALIGN_TOP_LEFT, 10, INFO_RETURN_Y);
    lv_obj_add_flag(info_return, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(info_return, return_mainpage_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_ext_click_area(info_return, 15);
    LV_IMG_DECLARE(logo_return);        //使用返回图片
    lv_img_set_src(info_return, &logo_return); 
}

/* info左边线条 */
void create_info_line(void){

    static lv_point_t points[] = { 
        {INFO_LINE_X, FirstLineINFO_Y},     //line起点
        {INFO_LINE_X, LastLineINFO_Y} };    //line终点
    static lv_style_t styleline;
    lv_style_init(&styleline);
    lv_style_set_line_color(&styleline, lv_color_white());
    lv_style_set_line_width(&styleline, 3);

    info_line = lv_line_create(bg_screen);
    lv_line_set_points(info_line, points, 2);
    lv_obj_add_style(info_line, &styleline, 0);
    lv_obj_align(info_line, LV_ALIGN_TOP_LEFT, 0, 0);
}


/**
 * @brief 快速创建一个info label对象，并设置文字内容、对齐左上角，内容需要重新设定
 */
lv_obj_t * lv_create_info_label(lv_obj_t *parent, char *text, lv_coord_t x, lv_coord_t y){
    lv_obj_t * obj = lv_label_create(parent);
    lv_label_set_text(obj, text);
    lv_obj_align(obj, LV_ALIGN_TOP_LEFT, x, y);
    return obj;
}

/**
 * @brief 创建 axp info 文字对象
 *      包括： 充电状态 电池电压/百分比 当前功率
 */
void create_axp_info(void){
    axp_info = lv_label_create(bg_screen);
    lv_label_set_recolor(axp_info, true);
    // lv_label_set_text(axp_info, "#ffffff axp173 : charging\n#ffffff Volt: 3.7V\n#ffffff Percent: 40%\n#ffffff Power: 0.14w ");
    lv_label_set_text(axp_info, "#ffffff axp173 : charging");
    lv_obj_align(axp_info, LV_ALIGN_TOP_LEFT, INFO_LINE_X + 10, FirstLineINFO_Y);
    
    // axp_info1 = lv_create_info_label(bg_screen, "Volt: 3.7V", INFO_LINE_X+10, FirstLineINFO_Y+20);
    // axp_info2 = lv_create_info_label(bg_screen, "Power:0.14w",INFO_LINE_X+10, FirstLineINFO_Y+40);
}

/**
 * @brief 创建 sys info 文字对象
 *      包括： cpu型号  flash大小  剩余堆容量  mac地址
 */
void create_sys_info(void){
    sys_info = lv_label_create(bg_screen);
    lv_label_set_recolor(sys_info, true);
    lv_label_set_text(sys_info, "#ffffff system : esp32S3\n#ffffff flash: 123 Mb\n#ffffff free heap size: 51 bytes");
    lv_obj_align(sys_info, LV_ALIGN_TOP_LEFT, INFO_LINE_X + 10, 160);
}


/*************************
 *        页面动画
 *************************/

/* info页面动画 */
void info_page_anim_in(uint32_t delay){
    anim_y_fade_in(info_return, -50, INFO_RETURN_Y, delay);
    anim_y_fade_in(info_line, -100, 0, delay);
    anim_x_fade_in(axp_info, -50, 40, delay);
    anim_x_fade_in(sys_info, -50, 40, delay);
}
void info_page_anim_out(uint32_t delay){
    anim_y_fade_out(info_return, INFO_RETURN_Y, -50, delay);
    anim_y_fade_out(info_line, 0, 100, delay);
    anim_x_fade_out(axp_info, 40, 100, delay);
    anim_x_fade_out(sys_info, 40, 100, delay);
    
}

/*************************
 *        页面API
 *************************/

void info_page_hide_obj(void){
    lv_obj_add_flag(info_return, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(info_line, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(axp_info, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(sys_info, LV_OBJ_FLAG_HIDDEN);
}

void info_page_create_obj(void){
    create_info_return_btn();
    create_info_line();
    create_axp_info();
    create_sys_info();

    info_page_hide_obj();
}


void show_info_page(void){
    create_info_return_btn();
    create_info_line();
    create_axp_info();
    create_sys_info();
    info_page_anim_in(200);     //动画进场
}



/* 
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void app_main(void)
{
    printf("Hello world!\n");

    //Print chip information 
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

 */
