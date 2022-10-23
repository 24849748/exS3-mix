#include "encoder.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/pcnt.h"

#include "widgets_conf.h"

#define TAG "encoder"

static int32_t test_count;
// static int32_t last_count;  /* 最后一次计数 */
static int32_t encoder_diff = 0;    /* 编码器转动方向 1-cw, -1-ccw, 0-default */
static lv_indev_state_t encoder_state;  /* encoder 按键状态 */

encoder_handle_t ecd;   /* encoder 设备句柄 */

void encoder_init(void){

    ecd = encoder_create(ECD_PCNT_UNIT_NUM, PIN_ECD_A, PIN_ECD_B);
    // encoder_set_glitch_fliter(ecd, 1);
    // encoder_start(ecd);

    // last_count = encoder_get_counter_value(ecd);


}

void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data){
    data->enc_diff = encoder_diff;
    data->state = encoder_state;
}


// static void encoder_handler(void){
//     encoder_diff += 0;
//     encoder_state = LV_INDEV_STATE_REL;
// }

void encoder_test(void){
    encoder_init();
    encoder_start(ecd);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        test_count = encoder_get_counter_value(ecd);
        ESP_LOGI(TAG, "test_count: %d", test_count);
    }
    
}