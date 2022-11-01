#include "axp173.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "esp_system.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "axp173_reg.h"
#include "i2c_bus.h"
#include "esp_log.h"

#define TAG "axp173"


#define AXP173_CHECK(a, str, ret) if(!(a)) { \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        return (ret); \
    }

static axp173_dev_t axp; //隐藏设备


static esp_err_t axp_read_bytes(uint8_t reg, uint8_t *data, size_t len);
static esp_err_t axp_read_byte(uint8_t reg, uint8_t *data);
// static esp_err_t axp_read_bit(uint8_t reg, uint8_t bit_num, uint8_t *data);
static esp_err_t axp_write_byte(uint8_t reg, const uint8_t data);
static esp_err_t axp_write_bit(uint8_t reg, uint8_t bit_num, uint8_t data);
static esp_err_t axp_write_bits(uint8_t reg, uint8_t bit_start, uint8_t bit_len, uint8_t data);

/* calculate func */
static uint8_t voltToBin_25mV(float volt);
static uint8_t voltToBin_100mV(float volt);




/* =========================================================== */
/* -------------------- public func ----------------------- */


/* ===================
        初始化
====================== */

/**
 * @brief 初始化axp设备
 * 
 * @param port I2C port端口号
 * @param addr axp173设备地址
 * @return esp_err_t 
 */
esp_err_t axp_init(i2c_port_t port, uint8_t addr){
    ESP_LOGI(TAG, "Init axp173...");
    axp.addr = addr;
    axp.port = port;

    /* default setting */
    apx173_defaultSetting();

    ESP_LOGI(TAG, "axp173 init over");
    return ESP_OK;
}

// /**
//  * @brief 创建axp173设备，返回句柄
//  * 
//  * @param port axp173 所在i2c总线端口
//  * @param addr axp173 设备地址
//  * @return axp173_handle_t 
//  */
// axp173_handle_t axp173_create(i2c_port_t port, uint8_t addr){
//     ESP_LOGI(TAG, "Create AXP173 dev ...");
//     axp173_dev_t *axp = (axp173_dev_t *)calloc(1, sizeof(axp173_dev_t));
//     if(axp == NULL){
//         ESP_LOGE(TAG, "calloc axp173 failed");
//         return NULL;
//     }
//     axp->addr = addr;
//     axp->port = port;
//     ESP_LOGI(TAG, "AXP173 Create over");
//     return (axp173_handle_t) axp;
// }

// /**
//  * @brief 释放axp173内存资源
//  * 
//  * @param axp173 句柄
//  * @return esp_err_t 
//  */
// esp_err_t axp173_delete(axp173_handle_t *axp173){
//     if(*axp173 == NULL){
//         return ESP_OK;
//     }
//     axp173_dev_t *_axp = (axp173_dev_t *)(*axp173);
//     free(_axp);
//     *axp173 = NULL;
//     return ESP_OK;
// }


/* ========================================================= */

/* ---------------------- app --------------------------- */

/**
 * @brief 默认设置
 * 
 * 1.打开 DCDC1和LDO4通道，并设置为 3.3V 输出，关闭LDO2、LDO3、DCDC2通道
 * 2.设置关机电压Voff 2.9V
 * 3.打开VBUS限压功能，并设置VBUS限压4.4V
 * 4.启动充电功能
 * 5.设置充电电流789mA和充电结束电流（小于78.9mA）
 * 6.设置ADC采样速率25Hz
 * 7.使能电池、VBUS 的ADC
 * @return esp_err_t 
 */
esp_err_t apx173_defaultSetting(void){
    float bat_volt = 0;
    float VBUS_volt = 0;

    //设置通道电压
    axp_set_DC1_volt(3.3);
    axp_set_LDO4_volt(3.3);

    //打开通道输出 打开DCDC1，LDO4，关闭LDO23、DCDC2
    axp_output_channel_enable(OUTPUT_CH_DC1, 1);
    axp_output_channel_enable(OUTPUT_CH_LDO4, 1);
    axp_output_channel_enable(OUTPUT_CH_DC2, 0);
    axp_output_channel_enable(OUTPUT_CH_LDO2, 0);
    axp_output_channel_enable(OUTPUT_CH_LDO3, 0);
    
    //设置关机电压
    axp_set_volt_VOFF(VOFF_VOLT_2V9);

    // 长按时间2秒，长按自动关机，长按时间4秒
    axp_write_byte(AXP173_PEK, 0b01101100);

    //VBUS限压控制
    axp_VHOLD_enable(1);
    axp_set_volt_VHOLD(VHOLD_VOLT_4V4);
    
    // axp_autoShutdown_enable(true);
    // axp_set_time_shutdown(SHUTDOWN_TIME_4S); //有问题
    // axp_set_time_longPress(LONGPRESS_TIME_2S);

    
    //301525电池默认设置
    axp173_defaultSetting_301525();

    //设置adc采样率 25Hz
    axp_adc_set_sampling_freq(ADC_FREQ_25Hz);

    //电池、VBUS adc使能
    axp_adc_enable(ADC_ENABLE_BIT_BAT_VOLT, 1); 
    axp_adc_enable(ADC_ENABLE_BIT_VBUS_VOLT, 1); 
    axp_adc_enable(ADC_ENABLE_BIT_BAT_CURRENT, 1);
    axp_adc_enable(ADC_ENABLE_BIT_VBUS_CURRENT, 1);
    axp_adc_enable(ADC_ENABLE_BIT_TS, 1);

    //TS 引脚
    // axp_set_TS_PIN_output_Current(TS_CURRENT_20uA);
    // axp_set_TS_PIN_Current_output_way(C_OUTPUT_WAY_SAMPLING);
    // axp_select_TS_PIN_function(1);


    //读取一次电压
    // axp_get_bat_volt(&bat_volt);
    // axp_get_VBUS_volt(&VBUS_volt);
    bat_volt = axp_get_bat_volt();
    VBUS_volt = axp_get_VBUS_volt();

    ESP_LOGI(TAG, "AXP173 init over!");
    ESP_LOGI(TAG, "BAT volt: %.2f  VBUS volt: %.2f",bat_volt, VBUS_volt);


    return ESP_OK;
}


/**
 * @brief exS3使用的301525电池默认设置
 * 
 * @return esp_err_t 
 */
esp_err_t axp173_defaultSetting_301525(void){
    // axp_set_VOFF_volt(VOFF_VOLT_2V7);
    axp_charge_enable(1);
    axp_set_volt_chargeTarget(VOLT_CHARGE_TARGET_4V2);
    axp_set_current_charge(CHARGE_CURRENT_mA_100);
    axp_set_current_chargeEnd(0);
    return ESP_OK;
/* 
型号:301525
容量：100MAH
尺寸：厚3MM宽15MM长25MM
额定电压：3.7V
充电电压：4.2V
充电温度：0°C ~ 45°C
放电温度： -20°C ~ +60°C
存储温度：-20°C ~ +35°C
充电电流：标准充电：0.5C、快速充电：1.0C
标准充电方法：0.5C CC（恒流）充电至4.25V，再CV（恒压4.2V）充电直至充电电流≤0.05C
最大充电电流：1C
最大放电电流：1C
保护IC参数: 过充电保护电压4.20V±0.05V ，过放电保护电压2.7V±0.1V,充许最大电流2A。
 */
}





/* ===================
        ADC相关
====================== */



/**
 * @brief adc使能1控制
 * 
 * @param adc_bit   使能哪个adc, 详见macro：ADC_ENABLE_BIT...
 * @param enable    是否使能 1/0
 * @return esp_err_t 
 */
esp_err_t axp_adc_enable(adc_enable_t adc_channel, uint8_t enable){
    return axp_write_bit(AXP173_ADC_ENABLE_1, adc_channel, enable);
}

/**
 * @brief ADC 采样速率设置(√)
 * 
 * @param freq_select  采样速率n（25*2^n）
 *                   - 0 : 25Hz， 
 *                   - 1 : 50Hz，
 *                   - 2 : 100Hz，
 *                   - 3 : 200Hz，
 * @return esp_err_t 
 */
esp_err_t axp_adc_set_sampling_freq(adc_freq_t freq_select){
    return axp_write_bits(AXP173_ADC_RATE_TS_PIN, 6, 2, freq_select);
}


/* ===================
电池相关、状态读取相关
====================== */


/**
 * @brief 读取电池电压(√)
 * 
 * @return float 电压返回值,单位V
 */
float axp_get_bat_volt(void){
    uint8_t tmp[2];
    float sen, volt;

    /* 1.1mV per LSB */
    sen = 1.1 / 1000;
    esp_err_t ret = axp_read_bytes(AXP173_BAT_VOLTAGE, tmp, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    volt = (((tmp[0] << 4) + tmp[1]) * sen);
    // ESP_LOGI(TAG, "Read bat volt raw: %d", ((tmp[0] << 8) + tmp[1]));

    ESP_LOGI(TAG, "Read bat volt: %.2f V", volt);
    return volt;
}


/**
 * @brief 读取电池放电电流(√)
 * 
 * @return float 电流，单位mA
 */
float axp_get_bat_discharge_current(void){
    uint8_t data[2];
    float step, current;

    step = 0.5;      //0.5mA per step

    esp_err_t ret = axp_read_bytes(AXP173_DISCHARGE_COULOMB, data, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    current = (((data[0] << 5) | data[1]) * step);
    ESP_LOGI(TAG, "Bat discharge current: %.1f mA", current);
    return current;
}

/**
 * @brief 读取电池充电电流(√)
 * 
 * @return float 电流，单位mA
 */
float axp_get_bat_charge_current(void){
    uint8_t data[2];
    float step, current;
    step = 0.5;      //0.5mA per step

    esp_err_t ret = axp_read_bytes(AXP173_CHARGE_CURREN, data, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    current = (((data[0] << 5) | data[1]) * step);
    ESP_LOGI(TAG, "Bat charge current: %.1f mA", current);
    return current;
}


/**
 * @brief 获取电池百分比
 * 
 * @return int32_t 
 */
int32_t axp_get_bat_pct(void){
    // int32_t ret_pct;
    // float volt;
    // volt = axp_get_bat_volt();

    return (int32_t)((4.2/axp_get_bat_volt())*100);
}


/**
 * @brief 读取有关电池的状态，存储在 axp173_bat_info_t 结构体中(√)
 * 
 * @param bat_info 
 * @return esp_err_t 
 */
esp_err_t axp_get_bat_info(axp173_bat_info_t *bat_info){
    esp_err_t ret;
    uint8_t data;

    ret = axp_read_byte(AXP173_POWER_STATUS, &data);
    AXP173_CHECK(ret == ESP_OK, "read byte failed", ESP_FAIL);
    bat_info->currentPath = (data & (1<<2)) ? true : false;

    ret = axp_read_byte(AXP173_CHARGE_STATUS, &data);
    AXP173_CHECK(ret == ESP_OK, "read byte failed", ESP_FAIL);
    bat_info->overTemperature = (data & (1<<7)) ? true : false;
    bat_info->isCharging = (data & (1<<6)) ? true : false;
    bat_info->isExist = (data & (1<<5)) ? true : false;

    ESP_LOGI(TAG, "battery info: charge[%d], currentPath[%d], exist[%d], temperature[%d]", bat_info->isCharging, bat_info->currentPath, bat_info->isExist, bat_info->overTemperature);

    return ret;
}


/**
 * @brief 读取VBUS电压(√)
 * 
 * @return float 电压返回值，单位V
 */
float axp_get_VBUS_volt(void){
    uint8_t tmp[2];
    float sen,volt;

    sen = 1.7 / 1000;
    esp_err_t ret = axp_read_bytes(AXP173_VBUS_VOLTAGE, tmp, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    volt = (((tmp[0] << 4) + tmp[1]) * sen);

    ESP_LOGI(TAG, "Read VBUS volt: %.2f V", volt);
    return volt;
}

/**
 * @brief 读取VBUS电流(√)
 * 
 * @return float 电流，单位mA
 */
float axp_get_VBUS_current(void){
    uint8_t data[2];
    float step, current;
    step = 0.375;      //0.375mA per step

    esp_err_t ret = axp_read_bytes(AXP173_VBUS_CURRENT, data, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    current = (((data[0] << 4) | data[1]) * step);
    ESP_LOGI(TAG, "VBUS current: %.2f mA", current);
    return current;
}


/* ===================
        充电控制
====================== */
/**
 * @brief 充电功能使能控制，默认1(√)
 * 
 * @param enable    1->打开充电，0->关闭充电
 * @return esp_err_t 
 */
esp_err_t axp_charge_enable(uint8_t enable){
    return axp_write_bit(AXP173_CHARGE_CONTROL_1, 7, enable);
}

/**
 * @brief 设置axp173充电目标电压，默认4.2V(√)
 * 
 * @param volt_select 见VOLT_CHARGE_TARGET_...
 * @return esp_err_t 
 */
esp_err_t axp_set_volt_chargeTarget(volt_charge_target_t volt_select){
    return axp_write_bits(AXP173_CHARGE_CONTROL_1, 5, 2, volt_select);
}


/**
 * @brief 设置axp173充电结束电流，默认0
 * 
 * @param end_current 
 *                  - 0 -> 充电电流小于 10%设置值时结束充电
 *                  - 1 -> 充电电流小于 15%设置值时结束充电
 *                  
 * @return esp_err_t 
 */
esp_err_t axp_set_current_chargeEnd(uint8_t end_current){
    return axp_write_bit(AXP173_CHARGE_CONTROL_1, 4, end_current);
}

/**
 * @brief 设置axp173充电电流，默认0
 * 
 * @param current   充电电流，见enum: CHARGE_CURRENT_mA_..
 *                  
 * @return esp_err_t 
 */
esp_err_t axp_set_current_charge(charge_current_t current){
    return axp_write_bits(AXP173_CHARGE_CONTROL_1, 0, 4, current);
}




/* ===================
    电源通道设置
====================== */

/**
 * @brief 电源输出通道使能
 * 
 * @param ctrl_bit      电源输出通道使能， 详见macro：OUTPUT_CH_..
 * @param enable        true->打开，false->关闭
 * @return esp_err_t 
 */
esp_err_t axp_output_channel_enable(output_channel_t channel, bool enable){
    return axp_write_bit(AXP173_DC1_LDO234_SW, channel, enable);
}

/**
 * @brief 
 * 
 * @param volt 
 * @return esp_err_t 
 */
esp_err_t axp_set_channel_volt(output_channel_t channel, float volt){
    esp_err_t ret = ESP_OK;
    switch (channel) {
    case OUTPUT_CH_DC1:
        // if(volt < 0.7 || volt > 3.5 ){
        // ESP_LOGE(TAG, "invalid range, DCDC1 suppost voltage range: (0.7, 3.5)");
        // break;
        // }
        // axp_write_byte(AXP173_DC1_VOLT, voltToBin_25mV(volt));
        // break;
        ret = axp_set_DC1_volt(volt);
        break;
    case OUTPUT_CH_DC2:
        ret = axp_set_DC2_volt(volt);
        break;
    case OUTPUT_CH_LDO2:
        ret = axp_set_LDO2_volt(volt);
        break;
    case OUTPUT_CH_LDO3:
        ret = axp_set_LDO3_volt(volt);
        break;
    case OUTPUT_CH_LDO4:
        ret = axp_set_LDO4_volt(volt);
        break;
    case OUTPUT_CH_EXTEN:
        ret = ESP_OK;
        break;
    }
    return ret;
}

/**
 * @brief 设置 DCDC2 输出电压，实际电压与设置电压可能有偏差
 * 
 * @param volt  范围: 0.7 ~ 2.275V
 * @return esp_err_t 
 */
esp_err_t axp_set_DC2_volt(float volt){
    if(volt < 0.7 || volt > 2.275 ){
        ESP_LOGE(TAG, "invalid range, DCDC2 suppost voltage range: (0.7, 2.275)");
        return ESP_FAIL;
    }
    return axp_write_byte(AXP173_DC2_VOLT, voltToBin_25mV(volt));
}

/**
 * @brief 设置 DCDC1 输出电压
 * 
 * @param volt  范围: 0.7 ~ 3.5V，25mV / Step
 * @return esp_err_t 
 */
esp_err_t axp_set_DC1_volt(float volt){
    if(volt < 0.7 || volt > 3.5 ){
        ESP_LOGE(TAG, "invalid range, DCDC1 suppost voltage range: (0.7, 3.5)");
        return ESP_FAIL;
    }
    return axp_write_byte(AXP173_DC1_VOLT, voltToBin_25mV(volt));
}


/**
 * @brief 设置 LDO4 输出电压, 实际电压与设置电压可能有偏差
 * 
 * @param volt  范围: 0.7 ~ 3.5V，25mV / Step
 * @return esp_err_t 
 */
esp_err_t axp_set_LDO4_volt(float volt){
    if(volt < 0.7 || volt > 3.5 ){
        ESP_LOGE(TAG, "invalid range, LDO4 suppost voltage range: (0.7, 3.5)");
        return ESP_FAIL;
    }
    return axp_write_byte(AXP173_LDO4_VOLT, voltToBin_25mV(volt));
}

/**
 * @brief 设置 LDO2 输出电压，实际电压与设置电压可能有偏差
 * 
 * @param volt  范围: 1.8 ~ 3.3V
 * @return esp_err_t 
 */
esp_err_t axp_set_LDO2_volt(float volt){
    // AXP173_CHECK_VOLT_RANGE(volt, 1.8, 3.3);
    if(volt < 1.8 || volt > 3.3 ){
        ESP_LOGE(TAG, "invalid range, LDO2 suppost voltage range: (1.8, 3.3)");
        return ESP_FAIL;
    }

    uint8_t data, merge_data;
    esp_err_t ret = axp_read_byte(AXP173_LDO23_VOLT, &data);//读取原来的设置
    if(ret != ESP_OK){
        return ret;
    }

    data &= 0b00001111; //取原来的低4位（LDO3的） 
    merge_data = ((voltToBin_100mV(volt) << 4) | data);  //转换后的值左移4位，与原来LDO3的合并

    // length = 4, bitstart = 4
    // uint8_t mask = ((1 << 4) - 1) << (4 - 4 + 1);
    // data <<= (bit_start - length + 1); // shift data into correct position
    // data &= mask;                      // zero all non-important bits in data
    // byte &= ~(mask);                  // zero all important bits in existing byte
    // byte |= data;                     // combine data with existing byte
    // return i2c_bus_write_byte(dev_handle, mem_address, byte);
    return axp_write_byte(AXP173_LDO23_VOLT, merge_data);
}

/**
 * @brief 设置 LDO3 输出电压，实际电压与设置电压可能有偏差
 *      低四位
 * 
 * @param volt  范围: 1.8 ~ 3.3V
 * @return esp_err_t 
 */
esp_err_t axp_set_LDO3_volt(float volt){
    // AXP173_CHECK_VOLT_RANGE(volt, 1.8, 3.3);
    if(volt < 1.8 || volt > 3.3 ){
        ESP_LOGE(TAG, "invalid range, LDO3 suppost voltage range: (1.8, 3.3)");
        return ESP_FAIL;
    }

    uint8_t data, merge_data;
    uint8_t tmp = voltToBin_100mV(volt);
    esp_err_t ret = axp_read_byte(AXP173_LDO23_VOLT, &data); //读取原来的设置
    if(ret != ESP_OK){
        return ret;
    }

    data &= 0b11110000; //取原来的高4位（LDO2的） 
    merge_data = data | tmp;  //转换后的值与原来LDO2的值合并

    return axp_write_byte(AXP173_LDO23_VOLT, merge_data);
}



/* ===================
        VHOLD
====================== */

/* ================ 以下代码未测试 ==================== */
/**
 * @brief VBUS VHOLD限压控制
 * 
 * @param enable    1->限压，0->不限压
 * @return esp_err_t 
 */
esp_err_t axp_VHOLD_enable(uint8_t enable){
    return axp_write_bit(AXP173_VBUS_TO_IPSOUT, 6, enable);
}

/**
 * @brief 设置VHOLD限压电压，默认4.4V
 * 
 * @param volt_select  限压电压，见VHOLD_VOLT_...
 * @return esp_err_t 
 */
esp_err_t axp_set_volt_VHOLD(volt_vhold_t volt_select){
    return axp_write_bits(AXP173_VBUS_TO_IPSOUT, 3, 3, volt_select);
}

/**
 * @brief 设置关机电压，默认2.9V
 * 
 * @param volt_select   关机电压，见VOFF_VOLT_...
 * @return esp_err_t 
 */
esp_err_t axp_set_volt_VOFF(volt_voff_t volt_select){
    return axp_write_byte(AXP173_SHUTDOWN_VOLT, volt_select);
}


/* ===================
        PEK按键
====================== */

/**
 * @brief 【FBI warning】关机控制
 * 
 * @param shutdown  1->关机，0->不关机
 * @return esp_err_t 
 */
esp_err_t axp_shutdown(uint8_t shutdown){
    return axp_write_bit(AXP173_SHUTDOWN_BAT_CHGLED, 7, shutdown);
}

esp_err_t axp_autoShutdown_enable(bool enable){
    return axp_write_bit(AXP173_PEK, 3, enable);
}

/**
 * @brief 设置关机时间，长按PEK键时间，大于该值将关机
 * 
 * @param axp173 
 * @param time 
 * @return esp_err_t 
 */
esp_err_t axp_set_time_shutdown(shutdown_time_t time){
    return axp_write_bits(AXP173_PEK, 0, 2, time);
}

/**
 * @brief 设置长按键时间
 * 
 * @param axp173 
 * @param time 
 * @return esp_err_t 
 */
esp_err_t axp_set_time_longPress(longPress_time_t time){
    return axp_write_bits(AXP173_PEK, 4, 2, time);
}





/* ===================
        内部温度
====================== */
/**
 * @brief AXP173 内部温度监测 ADC 使能(√)
 * 
 * @param enable    1->打开，0->关闭
 * @return esp_err_t 
 */
esp_err_t axp_internal_temperature_monitor_enable(uint8_t enable){
    return axp_write_bit(AXP173_ADC_ENABLE_2, 7, enable);
}

/**
 * @brief 读取芯片内部温度(√)
 * 
 * @return float 芯片内部温度
 */
float axp_get_internal_temperature(void){
    uint8_t tmp[2];
    float ret_data;
    float step = 0.1;

    esp_err_t ret = axp_read_bytes(AXP173_TEMP, tmp, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    ret_data = ((((tmp[0] << 4) + tmp[1]) * step)- 144.7);
    // ESP_LOGI(TAG, "Read bat volt raw: %d", ((tmp[0] << 8) + tmp[1]));

    ESP_LOGI(TAG, "Read inter temp: %.1f *C", ret_data);
    return ret_data;
}





/* ===================
        TS引脚
====================== */
/**
 * @brief TS 管脚输出电流设置
 * 
 * @param Current_select    输出电流，见TS_CURRENT_...
 *                      (0 : 20uA； 1 : 40uA； 2 : 60uA； 3 : 80uA)
 *                  
 * @return esp_err_t 
 */
esp_err_t axp_set_current_TSoutput(TS_output_current_t current_select){
    return axp_write_bits(AXP173_ADC_RATE_TS_PIN, 4, 2, current_select);
}

/**
 * @brief TS 管脚电流输出方式设置
 * 
 * @param way_select 电流输出方式，见C_OUTPUT_WAY_...
 *          (0:关闭;  1:充电时输出电流;  2:ADC 采样时输入可以省电;  3:一直打开)
 * @return esp_err_t 
 */
esp_err_t axp_set_currentOutputWay_TS(TS_current_output_way_t way_select){
    return axp_write_bits(AXP173_ADC_RATE_TS_PIN, 0, 2, way_select);
}

/**
 * @brief TS 管脚功能选择
 * 
 * @param func_select   0:电池温度监测功能，1:外部独立的 ADC 输入通路
 * @return esp_err_t 
 */
esp_err_t axp_set_function_TS(uint8_t func_select){
    return axp_write_bit(AXP173_ADC_RATE_TS_PIN, 2, func_select);
}

/**
 * @brief 读取TS引脚的ADC数据(需要根据热敏电阻进一步计算)
 * 
 * @return float 
 */
float axp_get_adcData_TS(void){
    uint8_t tmp[2];
    float step, ret_data;

    /* 0.8mV per LSB */
    step = 0.8;
    esp_err_t ret = axp_read_bytes(AXP173_TS_INPUT, tmp, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    ret_data = (((tmp[0] << 4) + tmp[1]) * step);
    // ESP_LOGI(TAG, "Read bat volt raw: %d", ((tmp[0] << 8) + tmp[1]));

    ESP_LOGI(TAG, "Read TS data: %.1f mV ", ret_data);
    return ret_data;
}





/* ===================
        库仑计
====================== */
/**
 * @brief 库仑计开关控制
 * 
 * @param enable    1->open; 0->close; 默认0
 * @return esp_err_t 
 */
esp_err_t axp_coulomb_enable(uint8_t enable){
    return axp_write_bit(AXP173_COULOMB_COUNTER, 7, enable);
}

/**
 * @brief 暂停库仑计控制，该寄存器位会自清零
 * 
 * @return esp_err_t 
 */
esp_err_t axp_coulomb_counter_pause(){
    return axp_write_bit(AXP173_COULOMB_COUNTER, 6, 1);
}

/**
 * @brief 清除库仑计控制，该寄存器位会自清零
 * 
 * @return esp_err_t 
 */
esp_err_t axp_coulomb_counter_clear(){
    return axp_write_bit(AXP173_COULOMB_COUNTER, 5, 1);
}

/**
 * @brief 获取 电池 充电库仑计 数据
 * 
 * @param charge_count  充电计数
 * @return esp_err_t 
 */
esp_err_t axp_get_coulombCount_charge(int32_t *charge_count){
    uint8_t data[4];
    esp_err_t ret = axp_read_bytes(AXP173_CHARGE_COULOMB, data, 4);
    *charge_count = (data[3]<<24 | data[2]<< 16 | data[1] << 8 | data[0]);
    
    return ret;
}


/**
 * @brief 获取 电池 放电库仑计 数据
 * 
 * @param discharge_count 放电计数
 * @return esp_err_t 
 */
esp_err_t axp_get_coulombCount_discharge(int32_t *discharge_count){
    uint8_t data[4];
    esp_err_t ret = axp_read_bytes(AXP173_DISCHARGE_COULOMB, data, 4);
    *discharge_count = (data[3]<<24 | data[2]<< 16 | data[1] << 8 | data[0]);

    return ret;
} 




/* IRQ 函数，未做测试 */

static void axpScanTask(void *pvParameters);
/**
 * @brief axp IRQ引脚中断处理函数，通知对应处理任务
 * 
 * @param arg 
 */
void IRAM_ATTR axp_isr_handler(void *arg){
    //notify task to read
    // uint32_t a = 0;
    xTaskNotifyFromISR(axpScanTask, 0, eNoAction, pdFALSE);
}


/**
 * @brief 打印IRQ信息
 * 
 * @param reg       寄存器地址
 * @param bitNum    bit位
 */
void axp_print_IRQ_status(uint8_t reg, uint8_t bitNum){
    switch (reg) {
    case AXP173_IRQ_STATUS_1:
        switch (bitNum) {
        case 7:
            ESP_LOGI(TAG, "IRQ:ACIN 过压");
            break;
        case 6:
            ESP_LOGI(TAG, "IRQ:ACIN 接入");
            break;
        case 5:
            ESP_LOGI(TAG, "IRQ:ACIN 移出");
            break;
        case 4:
            ESP_LOGI(TAG, "IRQ:VBUS 过压");
            break;
        case 3:
            ESP_LOGI(TAG, "IRQ:VBUS 接入");
            break;
        case 2:
            ESP_LOGI(TAG, "IRQ:VBUS 移出");
            break;
        case 1:
            ESP_LOGI(TAG, "IRQ:VBUS 可用但小于 VHOLD");
            break;
        case 0:
            ESP_LOGI(TAG, "保留");
            break;
        }
        break;
    case AXP173_IRQ_STATUS_2:
        switch (bitNum) {
        case 7:
            ESP_LOGI(TAG, "IRQ:电池接入");
            break;
        case 6:
            ESP_LOGI(TAG, "IRQ:电池移出");
            break;
        case 5:
            ESP_LOGI(TAG, "IRQ:电池激活模式");
            break;
        case 4:
            ESP_LOGI(TAG, "IRQ:退出电池激活模式");
            break;
        case 3:
            ESP_LOGI(TAG, "IRQ:正在充电");
            break;
        case 2:
            ESP_LOGI(TAG, "IRQ:充电完成");
            break;
        case 1:
            ESP_LOGI(TAG, "IRQ:电池过温");
            break;
        case 0:
            ESP_LOGI(TAG, "IRQ:电池低温");
            break;
        }
        break;
    case AXP173_IRQ_STATUS_3:
        switch (bitNum) {
        case 7:
            ESP_LOGI(TAG, "IRQ:AXP173 内部过温 ");
            break;
        case 6:
            ESP_LOGI(TAG, "IRQ:充电电流小于设置电流");
            break;
        case 5:
            ESP_LOGI(TAG, "IRQ:DC-DC1 输出电压小于设置值");
            break;
        case 4:
            ESP_LOGI(TAG, "IRQ:DC-DC2 输出电压小于设置值");
            break;
        case 3:
            ESP_LOGI(TAG, "IRQ:LDO4 输出电压小于设置值 ");
            break;
        case 2:
            ESP_LOGI(TAG, "保留");
            break;
        case 1:
            ESP_LOGI(TAG, "IRQ:短按键");
            break;
        case 0:
            ESP_LOGI(TAG, "长按键");
            break;
        }
        break;
    case AXP173_IRQ_STATUS_4:
        switch (bitNum) {
        case 7:
            ESP_LOGI(TAG, "保留");
            break;
        case 6:
            ESP_LOGI(TAG, "保留");
            break;
        case 5:
            ESP_LOGI(TAG, "IRQ:VBUS 有效");
            break;
        case 4:
            ESP_LOGI(TAG, "IRQ:VBUS 无效");
            break;
        case 3:
            ESP_LOGI(TAG, "IRQ:VBUS Session A/B");
            break;
        case 2:
            ESP_LOGI(TAG, "IRQ:VBUS Session End");
            break;
        case 1:
            ESP_LOGI(TAG, "保留");
            break;
        case 0:
            ESP_LOGI(TAG, "APS 低压 IRQ 状态, APS 电压低于 Warning Leve2 后置位, 超过 Warning Level1后将清 0");
            break;
        }
        break;
    }
}


/**
 * @brief 一次扫描4个IRQ状态寄存器，并打印输出
 * 
 * @return esp_err_t 
 */
void axp_IRQ_scan_status(void){
    uint8_t read_data[4];
    axp_read_bytes(AXP173_IRQ_STATUS_1, read_data, 4);
    
    for(int i=0; i<=3; i++){
        if(read_data[i] != 0){
            for(int j = 0; j<=7; j++){
                if(read_data[i]&(1<<j)){
                    // def_reg = def_reg + i;
                    // def_bitNum = j;
                    // ret_code = ((read_data[i]<<8)|j);
                    axp_print_IRQ_status(AXP173_IRQ_STATUS_1 + i, j);
                }
            }
        }
    }
} //获取中断状态（读取的byte与默认byte对比，不一样再比bit，返回对应bit位）

/**
 * @brief IRQ使能
 * 
 * @param IRQcode   "AXP173_intr.h"中定义的宏，包含寄存器地址和对应bit位，见IRQ_...
 * @param enable    是否使能
 * @return esp_err_t 
 */
esp_err_t axp_IRQ_enable(uint16_t IRQcode, bool enable){
    uint8_t bit_num = (IRQcode & 0xff);
    uint8_t reg = ((IRQcode&0xff00) >> 8);
    return axp_write_bit(reg, bit_num, enable);
}     //启动或屏蔽相应中断

/**
 * @brief 配置相应引脚，并开启中断
 * 
 * @param pin_IRQ IRQ引脚
 * @return esp_err_t 
 */
void axp_isr_init(gpio_num_t pin_IRQ){
    gpio_config_t conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << pin_IRQ),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin_IRQ, axp_isr_handler, NULL);
    
    // 使用默认使能状态
    // axp_IRQ_enable();
}

/**
 * @brief axp IRQ 状态扫描函数
 * 
 * @param pvParameters 
 */
static void axpScanTask(void *pvParameters){
    // 不在该任务内init
    // axp_isr_init(AXP173_PIN_IRQ);
    
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    axp_IRQ_scan_status();
}

void create_axp_IRQ_task(void){
    xTaskCreatePinnedToCore(axpScanTask,"axpScanTask",4096,NULL,10,NULL, 0);
}



/* -------------------- private func ----------------------- */
/* axp173 i2c read */
static esp_err_t axp_read_bytes(uint8_t reg, uint8_t *data, size_t len){
    return i2c_bus_read_bytes(axp.port, axp.addr, reg, data, len);
}
static esp_err_t axp_read_byte(uint8_t reg, uint8_t *data){
    return i2c_bus_read_byte(axp.port, axp.addr, reg, data);
}
// static esp_err_t axp_read_bit(uint8_t reg, uint8_t bit_num, uint8_t *data){
    // return i2c_bus_read_bit(axp.port, axp.addr, reg, bit_num, data);
// }

/* axp173 i2c write */
static esp_err_t axp_write_byte(uint8_t reg, const uint8_t data){
    return i2c_bus_write_byte(axp.port, axp.addr, reg, data);
}
static esp_err_t axp_write_bit(uint8_t reg, uint8_t bit_num, uint8_t data){
    return i2c_bus_write_bit(axp.port, axp.addr, reg, bit_num, data);
}
static esp_err_t axp_write_bits(uint8_t reg, uint8_t bit_start, uint8_t bit_len, uint8_t data){
    return i2c_bus_write_bits(axp.port, axp.addr, reg, bit_start, bit_len, data);
}

/* calculate func */
/**
 * @brief 将电压转换对应可调范围的二进制数 精度：25mV/step
 * 
 * @param volt      DCDC1或LDO4范围：0.7-3.5V，
 *                  DCDC2范围：0.7-2.275V；
 *                  实际电压可能与设置电压有差
 * @return uint8_t 
 */
uint8_t voltToBin_25mV(float volt){
    // uint8_t bin = (uint8_t)(((volt*1000)-700)/25);
    
    // char a[40];
    // itoa(bin, a, 2);
    // ESP_LOGI(TAG, "volt bin: %s", a);
    return (uint8_t)(((volt*1000)-700)/25);
    // return bin;
}

/**
 * @brief 将电压转换对应可调范围的二进制数 精度：100mV/step
 *          用于调整LDO2时需要左移4位
 * @param volt  范围：1.8-3.3V；实际电压可能与设置电压有差
 * @return uint8_t 
 */
uint8_t voltToBin_100mV(float volt){
    return (uint8_t)(((volt*1000)-1800)/100);
}


/**
 * @todo
 *      测试coulomb相关函数
 *      IRQ
 */


/**
 * 在某些特定事件发生时，AXP173 通过拉低 IRQ 的中断机制来提醒 Host，
 * 并将中断状态保存在中断状态寄存器中(参见寄存器 REG44H、寄存器 REG45H、
 * 寄存器 REG46H、寄存器 REG47H)，向相应的状态寄存器位写 1 则清除相应的
 * 中断，当无中断事件时，IRQ 输出拉高(通过外部上拉 51K 电阻)。
 * 每个中断都可以通过中断控制寄存器来屏蔽(参见寄存器 REG40H、
 * 寄存器 REG41H、寄存器 REG42H、寄存器REG43H)。
 * 
 *  1mA = 3.6coulomb
 */


/** 暂不实现的功能
 * VBUS限流使能、设置限流电流
 * Sleep 模式下 PWRON 短按唤醒功能使能设置
 * CHGLED 管脚功能设置、CHGLED 管脚控制设置
 * REG 36H: PEK 按键参数设置
 * REG 37H: DC-DC 工作频率设置
 * REG 38H: VLTF-charge 电池充电低温门限设置
 * REG 39H: VHTF-charge 电池充电高温门限设置
 * REG 3AH: APS 低电级别 1
 * REG 3BH: APS 低电级别 2
 * REG 3CH: VLTF-discharge 电池放电低温门限设置
 * REG 3DH: VHTF-discharge 电池放电高温门限设置
 * REG 80H: DC-DC 工作模式选择
 * REG 8AH: 定时器控制
 * REG 8BH: VBUS 管脚监测 SRP 功能控制
 * REG 8FH: 过温关机等功能设置
 */

// threshold  sampling