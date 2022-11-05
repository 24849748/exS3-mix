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

#include "wifi.h"
#include "time_ntp.h"

#include "axp173.h"
#include "motor.h"

#include "esp_log.h"
#define TAG "main"

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
    i2c_bus_init(I2C_NUM_0);


    led_init(PIN_LED, 0);
    motor_init(PIN_MOTOR, 0);


    axp_init(I2C_NUM_0, AXP173_I2C_ADDR);


    wifi_init();
    wifi_start();
    // time_ntp_init();
    time_ntp_get_time();
    

    lv_create_task();


    motor_on(PIN_MOTOR);
    vTaskDelay(pdMS_TO_TICKS(200));
    motor_off(PIN_MOTOR);

    rtos_debug();
}


/**
 *  WiFi ：wifi:state wrong txa_flags=9
 *  ntp get不到正确时间
 */


/** 启动流程 
 *  1.初始化总线（spi、i2c）
 *  2.初始化GPIO（led、motor）
 *  3.初始化存储设备（nvs）
 *  4.连接wifi
 *  5.初始化 lv_port_disp、lv_port_indev（屏幕、输入设备）
 *  6.lvgl_init
 *  7.创建lvgl任务
 *  8.创建其他任务
 *  9.
 *  10.
 */


/**
 * @todo
 *      
 */


/* 快速运行IDF环境命令
.\..\..\..\..\esp-idf\export.ps1

. C:\Users\bosco\esp32\esp-idf\export.ps1
*/