#ifndef _AXP173_H_
#define _AXP173_H_

#include "driver/i2c.h"
#include "axp173_type.h"

/* ↓ custom define */
#define AXP173_I2C_PORT         I2C_NUM_0
/* ↑ custom define */

#define AXP173_I2C_ADDR         (0x34)      //axp173 i2c address

#define AXP173_PIN_IRQ          39

/* ================================================================= */


// axp173 设备信息：addr port
typedef struct{
    uint8_t addr;               //axp173地址
    i2c_port_t port;            //axp173所在i2c总线端口
} axp173_dev_t;
// typedef void* axp173_handle_t;


/* 储存电池信息的结构体 */
typedef struct{
    bool currentPath;       //电池电流方向，1->in, 0->out
    bool isExist;             //1->电池存在，0->电池拔出
    bool isCharging;            //1->battery is charging; 0->discharging or finished
    bool overTemperature;       //1->over temperature; 0->isn't over temperature
}axp173_bat_info_t;


/* ===================
        初始化
====================== */

esp_err_t axp_init(i2c_port_t port, uint8_t addr);
// axp173_handle_t axp173_create(i2c_port_t port, uint8_t addr);
// esp_err_t axp173_delete(axp173_handle_t *axp173);

//预设置
esp_err_t apx173_defaultSetting(void);
esp_err_t axp173_defaultSetting_301525(void);



/* ===================
        控制操作
====================== */


// ADC相关
esp_err_t axp_adc_enable(adc_enable_t adc_channel, uint8_t enable);
esp_err_t axp_adc_set_sampling_freq(adc_freq_t freq_select);



// 电池相关、状态读取相关
float axp_get_bat_volt(void);
float axp_get_bat_discharge_current(void);
float axp_get_bat_charge_current(void);
int32_t axp_get_bat_pct(void);
esp_err_t axp_get_bat_info(axp173_bat_info_t *bat_info);      //电池状态

float axp_get_VBUS_volt(void);
float axp_get_VBUS_current(void);


// 充电控制
esp_err_t axp_charge_enable(uint8_t enable);
esp_err_t axp_set_volt_chargeTarget(volt_charge_target_t volt_select);
esp_err_t axp_set_current_charge(charge_current_t current);
esp_err_t axp_set_current_chargeEnd(uint8_t end_current);


// 电源通道参数设置
esp_err_t axp_output_channel_enable(output_channel_t channel, bool enable);
esp_err_t axp_set_channel_volt(output_channel_t channel, float volt);
esp_err_t axp_set_DC2_volt(float volt);
esp_err_t axp_set_DC1_volt(float volt);
esp_err_t axp_set_LDO4_volt(float volt);
esp_err_t axp_set_LDO2_volt(float volt);
esp_err_t axp_set_LDO3_volt(float volt);


/* ================以下未测试==================== */

esp_err_t axp_VHOLD_enable(uint8_t enable);
esp_err_t axp_set_volt_VHOLD(volt_vhold_t volt_select);
esp_err_t axp_set_volt_VOFF(volt_voff_t volt_select);

// PEK按键功能相关
esp_err_t axp_shutdown(uint8_t shutdown);
esp_err_t axp_autoShutdown_enable(bool enable);
esp_err_t axp_set_time_shutdown(shutdown_time_t time);
esp_err_t axp_set_time_longPress(longPress_time_t time);


// 其他温度相关
esp_err_t axp_internal_temperature_monitor_enable(uint8_t enable);
float axp_get_internal_temperature(void);


// TS pin引脚设置
esp_err_t axp_set_current_TSoutput(TS_output_current_t current_select);
esp_err_t axp_set_currentOutputWay_TS(TS_current_output_way_t way_select);
esp_err_t axp_set_function_TS(uint8_t func_select);
float axp_get_adcData_TS(void);


// 库仑计相关
esp_err_t axp_coulomb_enable(uint8_t enable);
esp_err_t axp_coulomb_counter_pause();
esp_err_t axp_coulomb_counter_clear();
esp_err_t axp_get_coulombCount_charge(int32_t *charge_count);
esp_err_t axp_get_coulombCount_discharge(int32_t *discharge_count);


// IRQ 未测试
esp_err_t axp_isr_init(gpio_num_t pin_IRQ);
esp_err_t axp_IRQ_enable(uint16_t IRQcode, bool enable);
void axp_IRQ_scan_status(void);
void create_axp_IRQ_task(void);

#endif
