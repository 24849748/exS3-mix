#include "vmotor.h"
#include "esp_log.h"

#define TAG "vmotor"

esp_err_t vmotor_init(gpio_num_t pin, uint32_t level){
    gpio_config_t conf = {
        .intr_type = GPIO_PIN_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pin_bit_mask = (1ULL << pin),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    esp_err_t ret = gpio_config(&conf);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "vmotor configurate failed!");
        return ESP_FAIL;
    }

    ret = gpio_set_level(pin, level);
    if(ret!=ESP_OK){
        ESP_LOGE(TAG, "vmotor set failed!");
        return ESP_FAIL;
    }
    return ESP_OK;
}

void vmotor_blink(gpio_num_t pin){
    uint8_t level = gpio_get_level(pin);
    gpio_set_level(pin, !level);
}

void vmotor_on(gpio_num_t pin){
    gpio_set_level(pin, 1);
}
void vmotor_off(gpio_num_t pin){
    gpio_set_level(pin, 0);
}
