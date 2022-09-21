#include <stdio.h>
#include <stdbool.h>
#include "esp_system.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "i2c_bus.h"
#include "axp173.h"

#define TAG "axp173"

//axp173 信息
typedef struct{
    bool is_VBUS_exist;      //1->VBUS is existed, 0->VBUS isn't existed
    bool is_ACIN_exist;
    bool bat_current_path;  //电池电流方向，1->in, 0->out
    
    bool is_BAT_exist;
    bool is_BAT_charging;    //1->charging; 0->discharging or finished
    bool is_over_temp;      //1->is over; 0->isn't over
    uint16_t BAT_volt;
}axp173_info_t;

//存axp173的地址、i2c总线句柄
typedef struct{
    i2c_bus_device_handle_t i2c_dev;    //i2c总线句柄
    uint8_t dev_addr;                   //axp173地址
    axp173_info_t *info;
} axp173_dev_t;


//low dirver
axp173_handle_t axp173_create(i2c_bus_handle_t bus, uint8_t dev_addr){
    if(bus == NULL){
        return NULL;
    }
    axp173_dev_t *axp = (axp173_dev_t *)calloc(1,sizeof(axp173_dev_t));
    axp->i2c_dev = i2c_bus_device_create(bus, dev_addr, i2c_bus_get_current_clk_speed(bus));
    if(axp->i2c_dev == NULL){
        free(axp);
        return NULL;
    }
    axp->dev_addr = dev_addr;
    axp->info = (axp173_info_t *)calloc(1,sizeof(axp173_info_t));
    return (axp173_handle_t)axp;
}

esp_err_t axp173_delete(axp173_handle_t *axp){
    if(*axp == NULL){
        return ESP_OK;
    }
    axp173_dev_t *temp_axp = (axp173_dev_t *)(*axp);
    i2c_bus_device_delete(&temp_axp->i2c_dev);
    free(temp_axp->info);
    free(temp_axp);
    *axp = NULL;
    return ESP_OK;
}


esp_err_t axp173_get_power_status(axp173_handle_t axp){
    axp173_dev_t *taxp = (axp173_dev_t *)axp;
    esp_err_t ret;
    uint8_t tmp = 0;
    ret = i2c_bus_read_byte(taxp->i2c_dev, AXP173_POWER_STATUS, &tmp);
    
    uint8_t data1 = 0;  //临时做 位与操作 的变量
    //VBUS
    
    data1 = tmp & (1<<5);  //  AXP173_POWER_STATUS寄存器第5位置1，和读取的tmp做与操作，置赋予data1
    taxp->info->is_VBUS_exist = (data1 != 0) ? 1 : 0; //判断读取的bit值是否不等于0，是is_VBUS_exist为true，否则为false
    

    taxp->info->is_ACIN_exist = ((tmp & (1<<7)) != 0) ? 1 : 0;


    ESP_LOGI(TAG, "is_VBUS_exist: %d, is_ACIN_exist: %d",taxp->info->is_VBUS_exist, taxp->info->is_ACIN_exist);
    
    return ret;
}

esp_err_t axp173_get_bat_status(axp173_handle_t axp){
    axp173_dev_t *taxp = (axp173_dev_t *)axp;
    esp_err_t ret;
    uint8_t tmp = 0;
    ret = i2c_bus_read_byte(taxp->i2c_dev, AXP173_CHARGE_STATUS, &tmp);
    
    taxp->info->is_over_temp = ((tmp & (1<<7)) != 0) ? 1 : 0;
    taxp->info->is_BAT_charging = ((tmp & (1<<6)) != 0) ? 1 : 0;
    taxp->info->is_BAT_exist = ((tmp & (1<<5)) != 0) ? 1 : 0; //判断读取的bit值是否不等于0，是is_VBUS_exist为true，否则为false

    ESP_LOGI(TAG, "BAT: %d, temp: %d, charging:%d",taxp->info->is_BAT_exist, taxp->info->is_over_temp, taxp->info->is_BAT_charging);
    
    return ret;
}

esp_err_t axp173_get_bat_volt(axp173_handle_t axp){
    axp173_dev_t *taxp = (axp173_dev_t *)axp;
    esp_err_t ret;
    uint8_t tmpH = 0;
    uint8_t tmpL = 0;
    ret = i2c_bus_read_byte(taxp->i2c_dev, AXP173_BAT_VOLTAGE, &tmpH);
    ret = i2c_bus_read_byte(taxp->i2c_dev, 0x79, &tmpL);

    ESP_LOGI(TAG, "L: %d  H: %d",tmpL, tmpH);

    taxp->info->BAT_volt = ((tmpL << 8) | tmpH);

    ESP_LOGI(TAG, "BAT volt: %d",taxp->info->BAT_volt);
    
    return ret;
}


/* ---------------------- app --------------------------- */

