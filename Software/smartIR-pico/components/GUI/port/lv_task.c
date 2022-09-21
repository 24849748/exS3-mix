#include "lv_task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_freertos_hooks.h"
#include "lvgl.h"
#include "lv_port_disp.h"

#include "main_page.h"


#define TAG "lv_task"
#define LV_TICK_PERIOD_MS 1

static SemaphoreHandle_t xGuiMutex;

static void lv_tick_task(void *arg){
    (void)arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void guiTask(void *arg){
    (void)arg;
    xGuiMutex = xSemaphoreCreateMutex();
    lv_init();

    lv_port_disp_init();

    //todo: lv indev init

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui",
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    //lvgl demo
    show_main_page();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        if(pdTRUE ==  xSemaphoreTake(xGuiMutex, portMAX_DELAY)){
            lv_task_handler();
            xSemaphoreGive(xGuiMutex);
        }
    }
    vTaskDelete(NULL);
}

