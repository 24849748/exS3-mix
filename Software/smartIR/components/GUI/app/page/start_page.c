#include "start_page.h"
#include "main_page.h"

#include "lv_common.h"

/* #define */

#define COORD_GIF_X 15
#define COORD_GIF_Y -35


LV_IMG_DECLARE(loadimg1);
LV_IMG_DECLARE(loadimg2);
LV_IMG_DECLARE(loadimg3);
LV_IMG_DECLARE(loadimg4);
const lv_img_dsc_t *dsc_imgs_load[4] = {
    &loadimg4,
    &loadimg3,
    &loadimg2,
    &loadimg1,
};


/* static lv_obj_t *obj */
// static lv_obj_t * startPageBG;  //加载页面背景（主对象）
lv_obj_t * loadBar;
lv_obj_t * loadLabel; 
lv_obj_t * animing_load; 


/* 回调函数 */
static void setBarValue(void * obj, int32_t value){
    lv_bar_set_value(obj, value, LV_ANIM_ON);
}

static void bar_event_cb(lv_event_t * e){
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
    if(dsc->part != LV_PART_INDICATOR) return;

    lv_obj_t * obj = lv_event_get_target(e);
    
    lv_coord_t coord_x = (lv_bar_get_value(obj)*2);
    lv_obj_align(animing_load, LV_ALIGN_LEFT_MID, coord_x, COORD_GIF_Y);
    // lv_obj_align(lv_obj_get_child(startPageBG,0), LV_ALIGN_LEFT_MID, coord_x, COORD_GIF_Y);
    if(lv_bar_get_value(obj) == 95){
        lv_obj_add_flag(loadBar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(loadLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(animing_load, LV_OBJ_FLAG_HIDDEN);
        main_page_anim_in(200);
    }
}


/*************************
 *      绘制页面内容
 *************************/

void start_page(void){
    
    // lv_obj_t * loadBar;
    // lv_obj_t * loadLabel; 
    // lv_obj_t * animing_load; 

    /* gif 动图 */
    animing_load = lv_animimg_create(bg_screen);
    // lv_obj_t * animing_load = lv_animimg_create(startPageBG);
    lv_obj_align(animing_load, LV_ALIGN_LEFT_MID, COORD_GIF_X, COORD_GIF_Y);
    lv_animimg_set_src(animing_load,  (lv_img_dsc_t**)dsc_imgs_load, 4);
    lv_animimg_set_duration(animing_load, 250);
    lv_animimg_set_repeat_count(animing_load, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(animing_load);

    /* Loading文本 */
    loadLabel = lv_label_create(bg_screen);
    // lv_obj_t * loadLabel = lv_label_create(startPageBG);
    lv_obj_add_style(loadLabel, &style_font, 0);
    lv_label_set_text(loadLabel, "Loading...");
    lv_obj_align(loadLabel, LV_ALIGN_CENTER, 0, 20);

    /* 进度条 */
    // lv_obj_t * loadBar = lv_bar_create(startPageBG);
    loadBar = lv_bar_create(bg_screen);
    lv_obj_set_size(loadBar, 200, 7);       // 进度条大小
    lv_obj_center(loadBar);                 // 进度条位置
    lv_bar_set_range(loadBar, 5, 95);      // 进度条范围    
    lv_obj_set_style_bg_color(loadBar, lv_color_hex(0xfdd86d), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(loadBar, lv_color_hex(0xa9edb9), LV_PART_MAIN);
    lv_obj_add_event_cb(loadBar, bar_event_cb, LV_EVENT_DRAW_PART_END, NULL);

    /* 进度条动画 */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, loadBar);
    lv_anim_set_exec_cb(&a, setBarValue);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_values(&a, 5, 95);
    lv_anim_set_delay(&a, 500);
    lv_anim_start(&a);
    // lv_anim_set_get_value_cb();
    // printf("animing_load id: %d\n",lv_obj_get_child_id(animing_load));
    // printf("loadLabel id: %d\n",lv_obj_get_child_id(loadLabel));
    // printf("loadBar id: %d\n",lv_obj_get_child_id(loadBar));
}

