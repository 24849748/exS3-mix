#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "sdkconfig.h"

#include "led.h"
#include "i2c_bus.h"
#include "lv_task.h"

#include "encoder.h"
#include "axp173.h"
#include "vmotor.h"

#include "esp_log.h"
#define TAG "main"

// axp173_handle_t axp;

void app_main(void)
{   
    // int count;
    led_init(1, 1);
    vmotor_init(38,0);

    i2c_bus_init(I2C_NUM_0);
    
    // axp = axp173_create(I2C_NUM_0, AXP173_I2C_ADDR);


    lv_create_task();
    
    vmotor_on(38);
    vTaskDelay(pdMS_TO_TICKS(1000));
    vmotor_off(38);
    // encoder_create_task();
    // encoder_test();
}



/**
 * @todo
 *      lv显示axp173 信息界面
 *      lv动画
 *      lv 和 编码器
 */


/* 运行IDF环境快速命令
.\..\..\..\..\esp-idf\export.ps1

. C:\Users\bosco\esp32\esp-idf\export.ps1
*/