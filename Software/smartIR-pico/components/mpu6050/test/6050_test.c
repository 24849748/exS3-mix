#include <stdio.h>
#include "unity.h"
#include "driver/i2c.h"
#include "i2c_bus.h"
#include "mpu6050.h"
// #include "esp_system.h"

#define I2C_PORT    I2C_NUM_0
#define I2C_SCL     21     
#define I2C_SDA     4
#define I2C_FREQ    100000

static i2c_bus_handle_t i2c_bus = NULL;
static mpu6050_handle_t mpu6050 = NULL;



static void mpu6050_init(){
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
}

static void mpu6050_deinit()
{
    mpu6050_delete(&mpu6050);
    i2c_bus_delete(&i2c_bus);
}


static void mpu6050_get_data()
{
    uint8_t mpu6050_deviceid;
    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;
    int cnt = 100;
    mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);
    printf("mpu6050 device ID is: 0x%02x\n", mpu6050_deviceid);
    mpu6050_wake_up(mpu6050);
    mpu6050_set_acce_fs(mpu6050, ACCE_FS_4G);
    mpu6050_set_gyro_fs(mpu6050, GYRO_FS_500DPS);

    while (cnt--) {
        printf("\n************* MPU6050 MOTION SENSOR ************\n");
        mpu6050_get_acce(mpu6050, &acce);
        printf("acce_x:%.2f, acce_y:%.2f, acce_z:%.2f\n", acce.acce_x, acce.acce_y, acce.acce_z);
        mpu6050_get_gyro(mpu6050, &gyro);
        printf("gyro_x:%.2f, gyro_y:%.2f, gyro_z:%.2f\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
        printf("**************************************************\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

TEST_CASE("mpu6050_test","[mpu6050]"){
    mpu6050_init();
    vTaskDelay(1000 / portTICK_RATE_MS);
    mpu6050_get_data();
    mpu6050_deinit();
}

