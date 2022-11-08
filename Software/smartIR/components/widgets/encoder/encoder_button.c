#include "encoder_button.h"

#include "widgets_conf.h"

#include "esp_timer.h"
#include "esp_log.h"


#define TAG "ENCODER BUTTON"

#define ACTIVE_LEVEL    0   // 按键按下的电平


typedef struct {
    gpio_num_t pin;
    bool isPressing;            // 经过消抖判断后的状态
    
    int previousSteadyState;    // the previous steady state from the input pin, used to detect pressed and released event
	int lastSteadyState;        // the last steady state from the input pin
	int lastFlickerableState;   // the last flickerable state from the input pin
	int currentState;           // the current reading from the input pin


    int64_t debounceTime;       // 消抖时间
    int64_t lastDebounceTime;   // the last time the output pin was toggled

    void (*shortPress_cb)(void);    //
    void (*longPress_cb)(void);     //

    bool isShort;
    bool isLong;
    int64_t pressedTime;
    int64_t releasedTime;
} encoder_button_t;
encoder_button_t ecd_btn;


// bool isPressing = false;
// bool isLongDetected = false;
// int64_t pressedTime = 0;
// int64_t releasedTime = 0;


void encoder_btn_set_debounceTime(int64_t debounceTime){
    ecd_btn.debounceTime = debounceTime;
}

void encoder_btn_set_event_cb(encoder_btn_event_t event, encoder_btn_event_cb_t event_cb){
    if(event == ECD_BTN_EVENT_SHORTPRESS){
        ecd_btn.shortPress_cb = event_cb;
    }else if(event == ECD_BTN_EVENT_LONGPRESS){
        ecd_btn.longPress_cb = event_cb;
    }
}

int encoder_btn_get_state(void){
    return ecd_btn.lastSteadyState;
}

int encoder_btn_get_state_raw(void){
    return gpio_get_level(ecd_btn.pin);
}

bool encoder_btn_isPressing(void){
    return ecd_btn.isPressing;
}

bool encoder_btn_isPressed(void){
    if(ecd_btn.previousSteadyState == 1 && ecd_btn.lastSteadyState == 0){
        return true;
    }else {
        return false;
    }
}

bool encoder_btn_isReleased(void){
    if(ecd_btn.previousSteadyState == 0 && ecd_btn.lastSteadyState == 1){
        return true;
    }else {
        return false;
    }
}

// void encoder_btn_set_shortPress_event_cb(encoder_btn_event_cb_t event_cb){
//     ecd_btn.shortPress_cb = event_cb;
// }
// void encoder_btn_set_longPress_event_cb(encoder_btn_event_cb_t event_cb){
//     ecd_btn.longPress_cb = event_cb;
// }




/**
 * @brief 按键扫描，需要在循环中调用
 * 
 */
void encoder_btn_scan(void){
    ecd_btn.currentState = gpio_get_level(ecd_btn.pin);

    // 状态变化，更新状态、更新时间
    if(ecd_btn.currentState != ecd_btn.lastFlickerableState){       //如果currentState不等于lastFlickerableState
        ecd_btn.lastDebounceTime = esp_timer_get_time();            //更新上次状态变化时间，用于判断是否大于消抖间
        ecd_btn.lastFlickerableState = ecd_btn.currentState;        //更新currentState
    }

    // 消抖判断
    if((esp_timer_get_time() - ecd_btn.lastDebounceTime) >= ecd_btn.debounceTime){ //如果间隔时间大于消抖时间
        if(gpio_get_level(ecd_btn.pin) == ACTIVE_LEVEL){
            ecd_btn.isPressing = true;
        }else{
            ecd_btn.isPressing = false;
        }
        ecd_btn.previousSteadyState = ecd_btn.lastSteadyState; 
        ecd_btn.lastSteadyState = ecd_btn.currentState;             // 最后稳定状态 等于 现在的状态
        
    }

    if(ecd_btn.previousSteadyState == 1 && ecd_btn.lastSteadyState == 0){   // isPressed()
        ecd_btn.pressedTime = esp_timer_get_time();
        ecd_btn.isShort = true;
        ecd_btn.isLong = false;
    }
    if(ecd_btn.previousSteadyState == 0 && ecd_btn.lastSteadyState == 1){
        ecd_btn.isShort = false;
        ecd_btn.releasedTime = esp_timer_get_time();
        if((ecd_btn.releasedTime - ecd_btn.pressedTime) < SHORT_TIME){
            // ESP_LOGI(TAG, "short");
            ecd_btn.shortPress_cb();
        }
    }
    if(ecd_btn.isPressing == true && ecd_btn.isLong == false){
        if((esp_timer_get_time() - ecd_btn.pressedTime) > LONG_TIME){
            ecd_btn.isLong = true;
            // ESP_LOGI(TAG, "long");
            ecd_btn.longPress_cb();
        }
    }
}

// custom
void short_press_cb(void){
    ESP_LOGI(TAG, "short");
}
void long_press_cb(void){
    ESP_LOGI(TAG, "long");
}


/**
 * @brief 按键初始化
 * 
 */
void encoder_btn_init(){
    gpio_config_t config = {
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << PIN_ECD_BTN),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&config);

    
    ecd_btn.pin = PIN_ECD_BTN;
    ecd_btn.debounceTime = DEBOUNCE_TIME;
    
    ecd_btn.isPressing = false;
    ecd_btn.previousSteadyState = gpio_get_level(PIN_ECD_BTN);
    ecd_btn.lastSteadyState = gpio_get_level(PIN_ECD_BTN);
    ecd_btn.lastFlickerableState = gpio_get_level(PIN_ECD_BTN);

    ecd_btn.lastDebounceTime = 0;

    ecd_btn.isShort = false;
    ecd_btn.isLong = false;
    ecd_btn.pressedTime = 0;
    ecd_btn.releasedTime = 0;
    // ecd_btn.shortPress_cb = &short_press_cb;
    // encoder_btn_set_shortPress_event_cb(&short_press_cb);
    // ecd_btn.longPress_cb = &long_press_cb;
    // encoder_btn_set_longPress_event_cb(&long_press_cb);
    encoder_btn_set_event_cb(ECD_BTN_EVENT_SHORTPRESS, &short_press_cb);
    encoder_btn_set_event_cb(ECD_BTN_EVENT_LONGPRESS, &long_press_cb);
    

    ESP_LOGI(TAG, "Encoder Button init over");
}



/* 测试用例 */
static void encoder_btn_task(void *pvParameter){
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(100));
        encoder_btn_scan();
        // ESP_LOGI(TAG, "btn state:%d", encoder_btn_isPressing());
        // ESP_LOGI(TAG, "isReleased:%d", encoder_btn_isReleased());
    }
}

void create_encoder_button_task(void){
    xTaskCreatePinnedToCore(encoder_btn_task, "encoder btn task", 2048, NULL, 4, NULL, 1);
}