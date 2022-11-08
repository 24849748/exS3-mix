#ifndef _ENCODER_BUTTON_H_
#define _ENCODER_BUTTON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define DEBOUNCE_TIME   8*1000          //消抖时间8ms
#define SHORT_TIME      800*1000          //短按时间8ms
#define LONG_TIME       2000*1000       //长按事件1s

typedef enum {
    ECD_BTN_EVENT_SHORTPRESS,
    ECD_BTN_EVENT_LONGPRESS,
} encoder_btn_event_t;
typedef void * encoder_btn_event_cb_t;

void encoder_btn_init();
void encoder_btn_scan(void);

void encoder_btn_set_debounceTime(int64_t debounceTime);
void encoder_btn_set_event_cb(encoder_btn_event_t event, encoder_btn_event_cb_t event_cb);
int encoder_btn_get_state(void);
int encoder_btn_get_state_raw(void);
bool encoder_btn_isPressing(void);
// bool encoder_btn_isPressed(void);
// bool encoder_btn_isReleased(void);


void create_encoder_button_task(void);

#endif
