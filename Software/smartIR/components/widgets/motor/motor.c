#include "motor.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_log.h"

#define TAG "motor"

static TimerHandle_t motor_timer = NULL;


// static uint32_t motorWorkTime;

static void motor_timer_cb(TimerHandle_t xTimer){
    gpio_set_level(PIN_MOTOR, 0);
}

esp_err_t motor_init(gpio_num_t pin, uint32_t level){
    gpio_config_t conf = {
        .intr_type = GPIO_PIN_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pin_bit_mask = (1ULL << pin),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    esp_err_t ret = gpio_config(&conf);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "motor configurate failed!");
        return ESP_FAIL;
    }

    ret = gpio_set_level(pin, level);
    if(ret!=ESP_OK){
        ESP_LOGE(TAG, "motor set failed!");
        return ESP_FAIL;
    }
    
    // 创建freertos定时器,未激活
    motor_timer = xTimerCreate("motor_click",(pdMS_TO_TICKS(DEFAULT_MOTOR_CLICK_WORKTIME)), pdFALSE, NULL, motor_timer_cb);

    return ESP_OK;
}

void motor_set(gpio_num_t pin, uint32_t level){
    gpio_set_level(pin, level);
}

void motor_reverse(gpio_num_t pin){
    uint8_t level = gpio_get_level(pin);
    gpio_set_level(pin, !level);
}

void motor_on(gpio_num_t pin){
    gpio_set_level(pin, 1);
}
void motor_off(gpio_num_t pin){
    gpio_set_level(pin, 0);
}

/**
 * @brief 
 * 
 * @param motorWorkTime 马达震动时长,单位ms
 */
void motor_click(uint32_t motorWorkTime){
    if(!xTimerIsTimerActive(motor_timer)){
        motor_on(PIN_MOTOR);
        xTimerChangePeriod(motor_timer, (pdMS_TO_TICKS(motorWorkTime)), 0);
        xTimerStart(motor_timer, 0);
    }
}
