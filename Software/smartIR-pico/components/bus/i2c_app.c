/**
 * @file i2c_app.c
 * @author bosco (1270112821@qq.com)
 * @brief axp173 mpu6050应用层
 * 
 */
#include "i2c_app.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "freertos/queue.h"

#include "esp_system.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "i2c_bus.h"
#include "mpu6050.h"
#include "axp173.h"

#define TAG "i2c_app"

static i2c_bus_handle_t i2c_bus = NULL;
static mpu6050_handle_t mpu6050 = NULL;
static axp173_handle_t axp173 = NULL;


static void i2c_app_init(){
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ,
    };
    i2c_bus = i2c_bus_create(I2C_PORT, &conf);
    mpu6050 = mpu6050_create(i2c_bus, MPU6050_I2C_ADDR);
    axp173 = axp173_create(i2c_bus, AXP173_I2C_ADDR);
}

static void i2c_app_deinit(){
    mpu6050_delete(&mpu6050);
    axp173_delete(&axp173);
    i2c_bus_delete(&i2c_bus);
}

static void mpu6050_get_data(){
    uint8_t mpu6050_deviceid;
    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;
    complimentary_angle_t angle;

    i2c_app_init();

    mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);
    printf("mpu6050 device ID is: 0x%02x\n", mpu6050_deviceid);
    mpu6050_wake_up(mpu6050);
    mpu6050_set_acce_fs(mpu6050, ACCE_FS_8G);
    mpu6050_set_gyro_fs(mpu6050, GYRO_FS_1000DPS);

    while (1) {
        mpu6050_get_acce(mpu6050, &acce);
        mpu6050_get_gyro(mpu6050, &gyro);
        mpu6050_complimentory_filter(mpu6050, &acce, &gyro, &angle);
        // printf("acce_x:%.2f, acce_y:%.2f, acce_z:%.2f\t", acce.acce_x, acce.acce_y, acce.acce_z);
        // printf("gyro_x:%.2f, gyro_y:%.2f, gyro_z:%.2f\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
        ESP_LOGI(TAG, "roll:%.2f, pitch:%.2f", -angle.roll, angle.pitch);
        vTaskDelay(200 / portTICK_RATE_MS);

        // axp173_get_power_status(axp173);
        // axp173_get_bat_status(axp173);
        axp173_get_bat_volt(axp173);
    }

    i2c_app_deinit();
}
/* 
static void axp173_get_status(){
    
    while (1)
    {
        // axp173_get_power_status(axp173);
        // axp173_get_bat_status(axp173);
        axp173_get_bat_volt(axp173);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
} */

void i2c_app_create_task(){
    xTaskCreate(mpu6050_get_data, "mpu6050", 2048, NULL, 2, NULL);
    // xTaskCreate(axp173_get_status, "axp173", 2048, NULL, 2, NULL);
}