#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led.h"
#include "i2c_bus.h"
#include "lv_task.h"


#include "axp173.h"
#include "vmotor.h"

#include "esp_log.h"
#define TAG "main"

// axp173_handle_t axp;

/**
 * @brief   使用 vTaskList() debug 系统所有任务信息运行状态
 *      State说明
 *          -R：Ready状态
 *          -B：Blocked状态
 *          -X：Execute（调用 vTaskList() 的任务）
 *          -S：Suspended，or Blocked without a timeout
 *          -D：Delete
 * 
 */
void rtos_debug(void){
    static char pcWriterBuffer[512] = {0};
    while (1)
    {
        vTaskList(pcWriterBuffer);
        printf("-------------------------------------------------\n");
        printf("Name\t\tState\tPrio\tStack\tNum\tCoreID\n");
        printf("%s\n", pcWriterBuffer);

        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}


void app_main(void)
{   
    // int count;
    led_init(1, 1);
    vmotor_init(38, 0);

    i2c_bus_init(I2C_NUM_0);
    
    // axp = axp173_create(I2C_NUM_0, AXP173_I2C_ADDR);

    lv_create_task();
    

    vmotor_on(38);
    vTaskDelay(pdMS_TO_TICKS(500));
    vmotor_off(38);


    rtos_debug();
}




/**
 * @todo
 *      使用 任务通知 使用LED和motor， motor要提示 充电完成、电池剩余电量不足等。。
 *      以GoButton写一个c的驱动
 *      
 */


/* 快速运行IDF环境命令
.\..\..\..\..\esp-idf\export.ps1

. C:\Users\bosco\esp32\esp-idf\export.ps1
*/