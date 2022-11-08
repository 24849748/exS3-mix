#include "encoder.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "driver/pcnt.h"
#include "rot_enc.h"
#include "encoder_button.h"
#include "pwm_bl.h"

#include "widgets_conf.h"
#include "sdkconfig.h"

#define TAG "encoder"

#define ECD_DEBUG 1     //开启debug 输出


static int32_t last_count;  /* 最后一次计数 */
static int32_t encoder_diff = 0;    /* 编码器转动方向 1-cw, -1-ccw, 0-default */
static SemaphoreHandle_t encoder_mutex;     //encoder读写保护

int8_t last_brightness = 0;
int8_t brightness = 50;

encoder_handle_t ecd;   /* encoder 设备句柄 */

static pwm_bl_handle_t pwm_bl_handle;



void encoder_init(void){    
    // uint32_t pcnt_unit = 0;
    encoder_mutex = xSemaphoreCreateMutex();    //创建mutex;

    //编码器按键初始化
    encoder_btn_init();

    //创建编码器句柄
    ecd = encoder_create(ECD_PCNT_UNIT_NUM, PIN_ECD_A, PIN_ECD_B);
    
    //设置去抖值，1us
    encoder_set_glitch_fliter(ecd, 1);          //设置过滤
    encoder_start(ecd);         //开启encoder pcnt

    last_count = encoder_get_counter_value(ecd);    //获取初值

    pwm_bl_config_t bl_conf = {
        .gpio_num = CONFIG_PIN_ST7789_BLK,
        .output_invert = false,
        .pwm_control = true,
        .timer_idx = 0,
        .channel_idx = 0,
    };
    pwm_bl_handle = pwm_bl_create(&bl_conf);
    pwm_bl_set(pwm_bl_handle, brightness);
}


static void encoder_task(void *pvParameter){

    // ecd = (encoder_handle_t)pvParameter;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(100));

        // if(pdTRUE == xSemaphoreTake(encoder_mutex, portMAX_DELAY)){
            
            encoder_btn_scan();

            // int32_t dir = 0;
            int32_t count = encoder_get_counter_value(ecd);
            // ESP_LOGI(TAG, "count:%d",count);
            // if(count > 100) count = 100;
            // if(count < 0) count = 0;

            if(count - last_count < 0){
                encoder_diff = -1;
                brightness -= 5;
            }else if(count - last_count > 0){
                encoder_diff = 1;
                brightness += 5;
            }else if(count == last_count){
                encoder_diff = 0;
            }
            last_count = count;
            ESP_LOGI(TAG, "diff:%d", encoder_diff);


            if(brightness > 100) brightness = 100;
            if(brightness < 5) brightness = 5;

            if(brightness != last_brightness){
                pwm_bl_set(pwm_bl_handle, brightness);
                last_brightness = brightness;
            }

            

        // }
        // xSemaphoreGive(encoder_mutex);

    }
    // encoder_delete(ecd);
}

void create_encoder_task(void){
    xTaskCreatePinnedToCore(encoder_task, "encoderTask", 2048, NULL, 4, NULL, 1);
}

