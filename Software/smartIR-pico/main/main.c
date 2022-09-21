#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

// #include "esp_system.h"
#include "esp_log.h"

// #include "i2c_app.h"
#include "sd_spi_fatfs.h"
// #include "driver/spi_common.h"
// #include "lv_task.h"
#include "ir_nvs.h"
#include "ir_rmt.h"


#define TAG "main"


static bool on = true;
/* 
static void btn_single_click_cb(void *arg){
    btn_get_event(arg);
    on = !on;
    ESP_LOGI(TAG, "on: %d", on);
    if(on){
        ac.status.ac_power = AC_POWER_ON;
        execute_ac();
    }else{
        ac.status.ac_power = AC_POWER_OFF;
        execute_ac();
    }
    
} */

void rmt_test(){
    sdmmc_card_t * card = sd_spi_fatfs_init();
    if(card == NULL){
        ESP_LOGE(TAG, "sd card init failed!");
        return;
    }
    ir_nvs_init();
    IR_init();

    for(int i=0; i <=8; i++){
        ac.bin_num = i;
        on = !on;
        ESP_LOGI(TAG, "on: %d", on);
        if(on) {
        ac.status.ac_power = AC_POWER_ON;
        execute_ac();
        } else {
        ac.status.ac_power = AC_POWER_OFF;
        execute_ac();
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    while(1){
        ESP_LOGI(TAG,".");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



void app_main(void)
{   
    rmt_test();


    // xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 1, NULL, 0);
    // xTaskCreate(sd_task, "sd_card_task", 4096, NULL, 3, NULL);
    // i2c_app_create_task();
}



/* 
.\..\..\..\..\esp-idf\export.ps1

. C:\Users\bosco\esp32\esp-idf\export.ps1
*/