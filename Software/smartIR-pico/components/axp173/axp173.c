#include "axp173.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "esp_system.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "i2c_bus.h"

#define TAG "axp173"


#define AXP173_CHECK(a, str, ret) if(!(a)) { \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        return (ret); \
    }

/* -------------------- private func ----------------------- */
/* axp173 i2c read */
static esp_err_t axp173_read_bytes(axp173_handle_t axp173, uint8_t reg, uint8_t *data, size_t len){
    axp173_dev_t *_axp = (axp173_dev_t *)axp173;
    return i2c_bus_read_bytes(_axp->port, _axp->addr, reg, data, len);
}
static esp_err_t axp173_read_byte(axp173_handle_t axp173, uint8_t reg, uint8_t *data){
    axp173_dev_t *_axp = (axp173_dev_t *)axp173;
    return i2c_bus_read_byte(_axp->port, _axp->addr, reg, data);
}
// static esp_err_t axp173_read_bit(axp173_handle_t axp173, uint8_t reg, uint8_t bit_num, uint8_t *data){
//     axp173_dev_t *_axp = (axp173_dev_t *)axp173;
//     return i2c_bus_read_bit(_axp->port, _axp->addr, reg, bit_num, data);
// }

/* axp173 i2c write */
static esp_err_t axp173_write_byte(axp173_handle_t axp173, uint8_t reg, const uint8_t data){
    axp173_dev_t *_axp = (axp173_dev_t *)axp173;
    return i2c_bus_write_byte(_axp->port, _axp->addr, reg, data);
}
static esp_err_t axp173_write_bit(axp173_handle_t axp173, uint8_t reg, uint8_t bit_num, uint8_t data){
    axp173_dev_t *_axp = (axp173_dev_t *)axp173;
    return i2c_bus_write_bit(_axp->port, _axp->addr, reg, bit_num, data);
}
static esp_err_t axp173_write_bits(axp173_handle_t axp173, uint8_t reg, uint8_t bit_start, uint8_t bit_len, uint8_t data){
    axp173_dev_t *_axp = (axp173_dev_t *)axp173;
    return i2c_bus_write_bits(_axp->port, _axp->addr, reg, bit_start, bit_len, data);
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




/* =========================================================== */
/* -------------------- public func ----------------------- */


/**
 * @brief 创建axp173设备，返回句柄
 * 
 * @param port axp173 所在i2c总线端口
 * @param addr axp173 设备地址
 * @return axp173_handle_t 
 */
axp173_handle_t axp173_create(i2c_port_t port, uint8_t addr){
    ESP_LOGI(TAG, "Create AXP173 dev ...");
    axp173_dev_t *axp = (axp173_dev_t *)calloc(1, sizeof(axp173_dev_t));
    if(axp == NULL){
        ESP_LOGE(TAG, "calloc axp173 failed");
        return NULL;
    }
    axp->addr = addr;
    axp->port = port;
    ESP_LOGI(TAG, "AXP173 Create over");
    return (axp173_handle_t) axp;
}

/**
 * @brief 释放axp173内存资源
 * 
 * @param axp173 句柄
 * @return esp_err_t 
 */
esp_err_t axp173_delete(axp173_handle_t *axp173){
    if(*axp173 == NULL){
        return ESP_OK;
    }

    axp173_dev_t *_axp = (axp173_dev_t *)(*axp173);
    free(_axp);
    *axp173 = NULL;
    return ESP_OK;
}


/* ========================================================= */

/* ---------------------- app --------------------------- */


esp_err_t apx173_init(axp173_handle_t axp173){
    float bat_volt = 0;
    float VBUS_volt = 0;

    //设置通道电压
    axp173_set_DC1_volt(axp173, 3.3);
    axp173_set_LDO4_volt(axp173, 3.3);
    //打开通道输出 打开DC1和LDO4，LDO23、DC2关闭
    axp173_power_output_ctrl(axp173, OUTPUT_SW_DC1, 1);
    axp173_power_output_ctrl(axp173, OUTPUT_SW_LDO4, 1);
    axp173_power_output_ctrl(axp173, OUTPUT_SW_DC2, 0);
    axp173_power_output_ctrl(axp173, OUTPUT_SW_LDO2, 0);
    axp173_power_output_ctrl(axp173, OUTPUT_SW_LDO3, 0);
    
    //VBUS限压控制
    axp173_set_VHOLD_volt(axp173, VHOLD_VOLT_4V4);
    axp173_VHOLD_enable(axp173, 1);
    //设置关机电压
    axp173_set_VOFF_volt(axp173, VOFF_VOLT_2V9);
    //启动充电功能
    axp173_charge_enable(axp173, 1);
    //设置电池充电电流和结束电流
    axp173_set_charge_current(axp173, CHARGE_CURRENT_mA_780);
    axp173_set_charge_end_current(axp173, 0);
    //设置adc采样率 25Hz
    axp173_set_ADC_sampling_freq(axp173, 0);
    //电池、VBUS adc使能
    axp173_enable_adc(axp173, ADC_ENABLE_BIT_BAT_VOLT, 1); 
    axp173_enable_adc(axp173, ADC_ENABLE_BIT_VBUS_VOLT, 1); 
    //读取一次电压
    axp173_get_bat_volt(axp173, &bat_volt);
    axp173_get_VBUS_volt(axp173,&VBUS_volt);

    ESP_LOGI(TAG, "AXP173 init over!");
    ESP_LOGI(TAG, "BAT volt: %.2f  VBUS volt: %.2f",bat_volt, VBUS_volt);

    return ESP_OK;
}


/**
 * @brief adc使能1控制
 * 
 * @param axp173    axp173设备句柄
 * @param adc_bit   使能哪个adc, 详见macro：ADC_ENABLE_BIT...
 * @param enable    是否使能 1/0
 * @return esp_err_t 
 */
esp_err_t axp173_enable_adc(axp173_handle_t axp173, uint8_t adc_bit, uint8_t enable){
    // axp173_dev_t *_axp = (axp173_dev_t *)axp173;
    return axp173_write_bit(axp173, AXP173_ADC_ENABLE_1, adc_bit, enable);
}

/**
 * @brief 读取电池电压
 * 
 * @param axp173    axp173设备句柄
 * @param volt 电压返回值
 * @return esp_err_t 
 */
esp_err_t axp173_get_bat_volt(axp173_handle_t axp173, float *volt){
    uint8_t tmp[2];
    float sen;
    esp_err_t ret;

    /* 1.1mV per LSB */
    sen = 1.1 / 1000;
    ret = axp173_read_bytes(axp173, AXP173_BAT_VOLTAGE, tmp, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    *volt = (((tmp[0] << 4) + tmp[1]) * sen);
    // ESP_LOGI(TAG, "Read bat volt raw: %d", ((tmp[0] << 8) + tmp[1]));

    ESP_LOGI(TAG, "Read bat volt: %.2f V", *volt);
    return ret;
}

/**
 * @brief 读取VBUS电压
 * 
 * @param axp173    axp173设备句柄
 * @param volt 电压返回值
 * @return esp_err_t 
 */
esp_err_t axp173_get_VBUS_volt(axp173_handle_t axp173, float *volt){
    uint8_t tmp[2];
    float sen;
    esp_err_t ret;

    sen = 1.7 / 1000;
    ret = axp173_read_bytes(axp173, AXP173_VBUS_VOLTAGE, tmp, 2);
    AXP173_CHECK(ret == ESP_OK, "bus read bytes error", ESP_FAIL);

    *volt = (((tmp[0] << 4) + tmp[1]) * sen);

    // *volt = (((tmp[0] << 16) + (tmp[1] << 8) + tmp[2]) * sen);
    // ESP_LOGI(TAG, "Read VBUS volt raw: %d", ((tmp[0] << 8) + tmp[1]));
    ESP_LOGI(TAG, "Read VBUS volt: %.2f V", *volt);
    return ret;
}

/**
 * @brief 读取有关电池的状态，存储在 axp173_bat_info_t 结构体中
 * 
 * @param axp173    axp173设备句柄
 * @param bat_info 
 * @return esp_err_t 
 */
esp_err_t axp173_get_bat_status(axp173_handle_t axp173, axp173_bat_info_t *bat_info){
    esp_err_t ret;
    uint8_t data;

    ret = axp173_read_byte(axp173, AXP173_POWER_STATUS, &data);
    AXP173_CHECK(ret == ESP_OK, "read byte failed", ESP_FAIL);
    bat_info->currentPath = (data & (1<<2)) ? true : false;

    ret = axp173_read_byte(axp173, AXP173_CHARGE_STATUS, &data);
    AXP173_CHECK(ret == ESP_OK, "read byte failed", ESP_FAIL);
    bat_info->temperature = (data & (1<<7)) ? true : false;
    bat_info->charge = (data & (1<<6)) ? true : false;
    bat_info->exist = (data & (1<<5)) ? true : false;

    ESP_LOGI(TAG, "battery info: charge[%d], currentPath[%d], exist[%d], temperature[%d]", bat_info->charge, bat_info->currentPath, bat_info->exist, bat_info->temperature);

    return ret;
}


/**
 * @brief DCDC2开关控制
 * 
 * @param axp173    axp173设备句柄
 * @param ctrl_bit      控制哪个电源输出开关， 详见macro：OUTPUT_SW_..
 * @param enable        1->打开，0->关闭
 * @return esp_err_t 
 */
esp_err_t axp173_power_output_ctrl(axp173_handle_t axp173, uint8_t ctrl_bit, uint8_t enable){
    return axp173_write_bit(axp173, AXP173_DC1_LDO234_SW, ctrl_bit, enable);
}

/**
 * @brief 设置 DCDC2 输出电压，实际电压与设置电压可能有偏差
 * 
 * @param axp173    axp173设备句柄
 * @param volt  范围: 0.7 ~ 2.275V
 * @return esp_err_t 
 */
esp_err_t axp173_set_DC2_volt(axp173_handle_t axp173, float volt){
    if(volt < 0.7 || volt > 2.275 ){
        ESP_LOGE(TAG, "invalid range, DCDC2 suppost voltage range: (0.7, 2.275)");
        return ESP_FAIL;
    }
    return axp173_write_byte(axp173, AXP173_DC2_VOLT, voltToBin_25mV(volt));
}

/**
 * @brief 设置 DCDC1 输出电压
 * 
 * @param axp173    axp173设备句柄
 * @param volt  范围: 0.7 ~ 3.5V
 * @return esp_err_t 
 */
esp_err_t axp173_set_DC1_volt(axp173_handle_t axp173, float volt){
    if(volt < 0.7 || volt > 3.5 ){
        ESP_LOGE(TAG, "invalid range, DCDC1 suppost voltage range: (0.7, 3.5)");
        return ESP_FAIL;
    }
    return axp173_write_byte(axp173, AXP173_DC1_VOLT, voltToBin_25mV(volt));
}


/**
 * @brief 设置 LDO4 输出电压, 实际电压与设置电压可能有偏差
 * 
 * @param axp173    axp173设备句柄
 * @param volt  范围: 0.7 ~ 3.5V
 * @return esp_err_t 
 */
esp_err_t axp173_set_LDO4_volt(axp173_handle_t axp173, float volt){
    if(volt < 0.7 || volt > 3.5 ){
        ESP_LOGE(TAG, "invalid range, LDO4 suppost voltage range: (0.7, 3.5)");
        return ESP_FAIL;
    }
    return axp173_write_byte(axp173, AXP173_LDO4_VOLT, voltToBin_25mV(volt));
}

/**
 * @brief 设置 LDO2 输出电压，实际电压与设置电压可能有偏差
 * 
 * @param axp173    axp173设备句柄
 * @param volt  范围: 1.8 ~ 3.3V
 * @return esp_err_t 
 */
esp_err_t axp173_set_LDO2_volt(axp173_handle_t axp173, float volt){
    // AXP173_CHECK_VOLT_RANGE(volt, 1.8, 3.3);
    if(volt < 1.8 || volt > 3.3 ){
        ESP_LOGE(TAG, "invalid range, LDO2 suppost voltage range: (1.8, 3.3)");
        return ESP_FAIL;
    }

    uint8_t data, merge_data;
    esp_err_t ret = axp173_read_byte(axp173, AXP173_LDO23_VOLT, &data);//读取原来的设置
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
    return axp173_write_byte(axp173, AXP173_LDO23_VOLT, merge_data);
}

/**
 * @brief 设置 LDO3 输出电压，实际电压与设置电压可能有偏差
 *      低四位
 * 
 * @param axp173    axp173设备句柄
 * @param volt  范围: 1.8 ~ 3.3V
 * @return esp_err_t 
 */
esp_err_t axp173_set_LDO3_volt(axp173_handle_t axp173, float volt){
    // AXP173_CHECK_VOLT_RANGE(volt, 1.8, 3.3);
    if(volt < 1.8 || volt > 3.3 ){
        ESP_LOGE(TAG, "invalid range, LDO3 suppost voltage range: (1.8, 3.3)");
        return ESP_FAIL;
    }

    uint8_t data, merge_data;
    uint8_t tmp = voltToBin_100mV(volt);
    esp_err_t ret = axp173_read_byte(axp173, AXP173_LDO23_VOLT, &data); //读取原来的设置
    if(ret != ESP_OK){
        return ret;
    }

    data &= 0b11110000; //取原来的高4位（LDO2的） 
    merge_data = data | tmp;  //转换后的值与原来LDO2的值合并

    return axp173_write_byte(axp173, AXP173_LDO23_VOLT, merge_data);
}



/* ================ 以下代码未测试 ==================== */
/**
 * @brief VBUS VHOLD限压控制
 * 
 * @param axp173    axp173设备句柄
 * @param enable    1->限压，0->不限压
 * @return esp_err_t 
 */
esp_err_t axp173_VHOLD_enable(axp173_handle_t axp173, uint8_t enable){
    return axp173_write_bit(axp173, AXP173_VBUS_TO_IPSOUT, 6, enable);
}

/**
 * @brief 设置VHOLD限压电压，默认4.4V
 * 
 * @param axp173    axp173设备句柄
 * @param volt_select  限压电压，见VHOLD_VOLT_...
 * @return esp_err_t 
 */
esp_err_t axp173_set_VHOLD_volt(axp173_handle_t axp173, uint8_t volt_select){
    return axp173_write_bits(axp173, AXP173_VBUS_TO_IPSOUT, 3, 3, volt_select);
}

/**
 * @brief 设置关机电压，默认2.9V
 * 
 * @param axp173    axp173设备句柄
 * @param volt_select   关机电压，见VOFF_VOLT_...
 * @return esp_err_t 
 */
esp_err_t axp173_set_VOFF_volt(axp173_handle_t axp173, uint8_t volt_select){
    return axp173_write_byte(axp173, AXP173_SHUTDOWN_VOLT, volt_select);
}


/**
 * @brief 【FBI warning】关机控制
 * 
 * @param axp173    axp173设备句柄
 * @param shutdown  1->关机，0->不关机
 * @return esp_err_t 
 */
esp_err_t axp173_shutdown(axp173_handle_t axp173, uint8_t shutdown){
    return axp173_write_bit(axp173, AXP173_SHUTDOWN_BAT_CHGLED, 7, shutdown);
}

//充电控制1
/**
 * @brief 充电功能使能控制，默认1
 * 
 * @param axp173    axp173设备句柄
 * @param enable    1->打开充电，0->关闭充电
 * @return esp_err_t 
 */
esp_err_t axp173_charge_enable(axp173_handle_t axp173, uint8_t enable){
    axp173_write_bit(axp173, AXP173_CHARGE_CONTROL_1, 7, enable);
    return ESP_OK;
}

/**
 * @brief 设置axp173充电目标电压，默认4.2V
 * 
 * @param axp173    axp173设备句柄
 * @param volt_select 
 *                  - 0 -> 4.1V
 *                  - 1 -> 4.15V
 *                  - 2 -> 4.2V
 *                  - 3 -> 4.36V
 * @return esp_err_t 
 */
esp_err_t axp173_set_charge_target_volt(axp173_handle_t axp173, uint8_t volt_select){
    return axp173_write_bits(axp173, AXP173_CHARGE_CONTROL_1, 5, 2, volt_select);
}


/**
 * @brief 设置axp173充电结束电流，默认0
 * 
 * @param axp173    axp173设备句柄
 * @param end_current 
 *                  - 0 -> 充电电流小于 10%设置值时结束充电
 *                  - 1 -> 充电电流小于 15%设置值时结束充电
 *                  
 * @return esp_err_t 
 */
esp_err_t axp173_set_charge_end_current(axp173_handle_t axp173, uint8_t end_current){
    return axp173_write_bit(axp173, AXP173_CHARGE_CONTROL_1, 4, end_current);
}

/**
 * @brief 设置axp173充电电流，默认0
 * 
 * @param axp173    axp173设备句柄
 * @param current   充电电流，见enum: CHARGE_CURRENT_mA_..
 *                  
 * @return esp_err_t 
 */
esp_err_t axp173_set_charge_current(axp173_handle_t axp173, uint8_t current){
    return axp173_write_bits(axp173, AXP173_CHARGE_CONTROL_1, 0, 4, current);
}




/**
 * @brief AXP173 内部温度监测 ADC 使能
 * 
 * @param axp173    axp173设备句柄
 * @param enable    1->打开，0->关闭
 * @return esp_err_t 
 */
esp_err_t axp173_internal_temperature_monitor_enable(axp173_handle_t axp173, uint8_t enable){
    return axp173_write_bit(axp173, AXP173_ADC_ENABLE_2, 7, enable);
}

/**
 * @brief ADC 采样速率设置
 * 
 * @param axp173    axp173设备句柄
 * @param freq_select  采样速率n（25*2^n）
 *                   - 0 : 25Hz， 
 *                   - 1 : 50Hz，
 *                   - 2 : 100Hz，
 *                   - 3 : 200Hz，
 * @return esp_err_t 
 */
esp_err_t axp173_set_ADC_sampling_freq(axp173_handle_t axp173, uint8_t freq_select){
    return axp173_write_bits(axp173, AXP173_ADC_RATE_TS_PIN, 6, 2, freq_select);
}

/**
 * @brief TS 管脚输出电流设置
 * 
 * @param axp173    axp173设备句柄
 * @param Current_select    输出电流
 *                      (0 : 20uA； 1 : 40uA； 2 : 60uA； 3 : 80uA)
 *                  
 * @return esp_err_t 
 */
esp_err_t axp173_set_TS_PIN_output_Current(axp173_handle_t axp173, uint8_t Current_select){
    return axp173_write_bits(axp173, AXP173_ADC_RATE_TS_PIN, 4, 2, Current_select);
}

/**
 * @brief TS 管脚电流输出方式设置
 * 
 * @param axp173    axp173设备句柄
 * @param way_select 电流输出方式
 *          (0:关闭;  1:充电时输出电流;  2:ADC 采样时输入可以省电;  3:一直打开)
 * @return esp_err_t 
 */
esp_err_t axp173_set_TS_PIN_Current_output_way(axp173_handle_t axp173, uint8_t way_select){
    return axp173_write_bits(axp173, AXP173_ADC_RATE_TS_PIN, 0, 2, way_select);
}

/**
 * @brief TS 管脚功能选择
 * 
 * @param axp173    axp173设备句柄
 * @param func_select   0:电池温度监测功能，1:外部独立的 ADC 输入通路
 * @return esp_err_t 
 */
esp_err_t axp173_select_TS_PIN_function(axp173_handle_t axp173, uint8_t func_select){
    return axp173_write_bit(axp173, AXP173_ADC_RATE_TS_PIN, 2, func_select);
}

/**
 * @brief 库仑计开关控制
 * 
 * @param axp173    axp173设备句柄
 * @param enable    1->open; 0->close; 默认0
 * @return esp_err_t 
 */
esp_err_t axp173_coulomb_switch(axp173_handle_t axp173, uint8_t enable){
    return axp173_write_bit(axp173, AXP173_COULOMB_COUNTER, 7, enable);
}

/**
 * @brief 库仑计暂停控制，该寄存器位会自清零
 * 
 * @param axp173    axp173设备句柄
 * @return esp_err_t 
 */
esp_err_t axp173_coulomb_counter_pause(axp173_handle_t axp173){
    return axp173_write_bit(axp173, AXP173_COULOMB_COUNTER, 6, 1);
}

/**
 * @brief 清除库仑计控制，该寄存器位会自清零
 * 
 * @param axp173    axp173设备句柄
 * @return esp_err_t 
 */
esp_err_t axp173_coulomb_counter_clear(axp173_handle_t axp173){
    return axp173_write_bit(axp173, AXP173_COULOMB_COUNTER, 5, 1);
}

/**
 * @brief 获取 电池 充电库仑计 数据
 * 
 * @param axp173    axp173设备句柄
 * @param charge_count  充电计数
 * @return esp_err_t 
 */
esp_err_t axp173_get_charge_coulomb_count(axp173_handle_t axp173, int32_t *charge_count){
    uint8_t data[4];
    esp_err_t ret = axp173_read_bytes(axp173, AXP173_CHARGE_COULOMB, data, 4);
    *charge_count = (data[3]<<24 | data[2]<< 16 | data[1] << 8 | data[0]);
    
    return ret;
}

/**
 * @brief 获取 电池 放电库仑计 数据
 * 
 * @param axp173    axp173设备句柄
 * @param discharge_count 放电计数
 * @return esp_err_t 
 */
esp_err_t axp173_get_discharge_coulomb_count(axp173_handle_t axp173, int32_t *discharge_count){
    uint8_t data[4];
    esp_err_t ret = axp173_read_bytes(axp173, AXP173_DISCHARGE_COULOMB, data, 4);
    *discharge_count = (data[3]<<24 | data[2]<< 16 | data[1] << 8 | data[0]);

    return ret;
} 


/**
 * @todo
 *      测试coulomb相关函数
 *      conf
 */


/** 暂不实现的功能
 * VBUS限流使能、设置限流电流
 * Sleep 模式下 PWRON 短按唤醒功能使能设置
 * CHGLED 管脚功能设置、CHGLED 管脚控制设置
 * REG 34H: 充电控制 2
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