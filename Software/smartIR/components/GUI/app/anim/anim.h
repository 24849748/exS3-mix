#ifndef _ANIM_H_
#define _ANIM_H_


#include "lvgl.h"


void anim_x_cb(void * var, int32_t v);
void anim_y_cb(void * var, int32_t v);

void obj_set_anim(lv_obj_t *obj, int32_t start, int32_t end, uint32_t time, uint32_t delay, lv_anim_exec_xcb_t exec_cb, lv_anim_ready_cb_t ready_cb, lv_anim_path_cb_t path_cb);


void anim_x_fade_in(lv_obj_t *obj, int32_t start, int32_t end, uint32_t delay, lv_anim_ready_cb_t ready_cb);
void anim_x_fade_out(lv_obj_t *obj, int32_t start, int32_t end, uint32_t delay, lv_anim_ready_cb_t ready_cb);

void anim_y_fade_in(lv_obj_t *obj, int32_t start, int32_t end, uint32_t delay, lv_anim_ready_cb_t ready_cb);
void anim_y_fade_out(lv_obj_t *obj, int32_t start, int32_t end, uint32_t delay, lv_anim_ready_cb_t ready_cb);

void anim_step_in(lv_obj_t *obj, uint32_t delay);
void anim_step_out(lv_obj_t *obj, uint32_t delay);

#endif
