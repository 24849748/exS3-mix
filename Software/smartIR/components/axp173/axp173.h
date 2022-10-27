#ifndef _AXP173_H_
#define _AXP173_H_

#include "driver/i2c.h"

/* ↓ custom define */
#define AXP173_I2C_PORT         I2C_NUM_0
/* ↑ custom define */

#define AXP173_I2C_ADDR         (0x34)      //axp173 i2c address
/* registers */
#define AXP173_POWER_STATUS     (0x00)
#define AXP173_CHARGE_STATUS    (0x01)
// #define AXP173_OTG_VBUS_STATUS  (0x04)


#define AXP173_EXTEN_DC2_SW     (0x10)  //EXTEN & DC-DC2 开关控制寄存器
#define AXP173_DC1_LDO234_SW    (0x12)  //DC-DC1/LDO4 & LDO2/3 开关控制寄存器

/* 输出电压设置 */
#define AXP173_DC2_VOLT         (0x23)  //DC-DC2 输出电压设置
#define AXP173_DC2_VOLT_SLOPE   (0x25)  //DC-DC2 动态电压调节参数设置

#define AXP173_DC1_VOLT         (0x26)  //DC-DC1 输出电压设置
#define AXP173_LDO4_VOLT        (0x27)  //LDO4 输出电压设置
#define AXP173_LDO23_VOLT       (0x28)  //LDO2/3 输出电压设置


#define AXP173_VBUS_TO_IPSOUT   (0x30)  //VBUS-IPSOUT 通路设置寄存器
#define AXP173_SHUTDOWN_VOLT    (0x31)  //VOFF 关机电压设置寄存器
#define AXP173_SHUTDOWN_BAT_CHGLED  (0x32)  //关机、电池检测、CHGLED 控制寄存器

#define AXP173_CHARGE_CONTROL_1 (0x33)  //充电控制寄存器 1
#define AXP173_CHARGE_CONTROL_2 (0x34)  //充电控制寄存器 2

#define AXP173_PEK              (0x36)  //PEK 参数设置寄存器
// #define AXP173_DCDC_FREQUENCY   (0x37)  //DCDC 转换器工作频率设置寄存器
// #define AXP173_BAT_CHG_L_TEMP   (0x38)  //电池充电低温报警设置寄存器
// #define AXP173_BAT_CHG_H_TEMP   (0x39)  //电池充电高温报警设置寄存器

// #define AXP173_APS_LOW_POWER1   (0x3a)  //APS 低电 Level1 设置寄存器
// #define AXP173_APS_LOW_POWER2   (0x3b)  //APS 低电 Level2 设置寄存器
// #define AXP173_BAT_DISCHG_L_TEMP    (0x3c)  //电池放电低温报警设置寄存器
// #define AXP173_BAT_DISCHG_H_TEMP    (0x3d)  //电池放电高温报警设置寄存器


#define AXP173_DCDC_MODE        (0x80)  //DCDC 工作模式设置寄存器
#define AXP173_ADC_ENABLE_1     (0x82)  //ADC 使能设置寄存器 1
#define AXP173_ADC_ENABLE_2     (0x83)  //ADC 使能设置寄存器 2
#define AXP173_ADC_RATE_TS_PIN  (0x84)  //ADC 采样率设置，TS pin 控制寄存器


// #define AXP173_TIMER_CONTROL            (0x8a)  //定时器控制寄存器
// #define AXP173_VBUS_MONITOR             (0x8b)  //VBUS 监测设置寄存器
// #define AXP173_TEMP_SHUTDOWN_CONTROL    (0x8f)  //过温关机控制寄存器


/* Interrupt control registers */
#define AXP173_IRQ_EN_CONTROL_1 (0x40)  //IRQ 使能控制寄存器 1
#define AXP173_IRQ_EN_CONTROL_2 (0x41)  //IRQ 使能控制寄存器 2
#define AXP173_IRQ_EN_CONTROL_3 (0x42)  //IRQ 使能控制寄存器 3
#define AXP173_IRQ_EN_CONTROL_4 (0x43)  //IRQ 使能控制寄存器 4

#define AXP173_IRQ_STATUS_1     (0x44)  //IRQ 状态寄存器 1
#define AXP173_IRQ_STATUS_2     (0x45)  //IRQ 状态寄存器 2
#define AXP173_IRQ_STATUS_3     (0x46)  //IRQ 状态寄存器 3
#define AXP173_IRQ_STATUS_4     (0x47)  //IRQ 状态寄存器 4


/* ADC data registers */
#define AXP173_ACIN_VOLTAGE     (0x56)  //ACIN 电压 ADC 数据高 8 位, 低4位在 (0x57)
#define AXP173_ACIN_CURRENT     (0x58)  //ACIN 电流 ADC 数据高 8 位, 低4位在 (0x59)
#define AXP173_VBUS_VOLTAGE     (0x5a)  //VBUS 电压 ADC 数据高 8 位, 低4位在 (0x5b)
#define AXP173_VBUS_CURRENT     (0x5c)  //VBUS 电流 ADC 数据高 8 位, 低4位在 (0x5d)

//温度相关
#define AXP173_TEMP             (0x5e)  //AXP173 内部温度监测 ADC 数据高 8 位, 低4位在 (0x5f)
#define AXP173_TS_INPUT         (0x62)  //TS 输入 ADC 数据高 8 位，默认监测电池温度,, 低4位在 (0x63)


#define AXP173_BAT_POWER                (0x70)  //电池瞬时功率高 8 位，中 8 位(0x71)， 高 8 位(0x72) 
#define AXP173_BAT_VOLTAGE              (0x78)  //电池电压高 8 位，低 4 位(0x79)
#define AXP173_CHARGE_CURREN            (0x7a)  //电池充电电流高 8 位, 低 5 位(0x7b)
#define AXP173_DISCHARGE_CURRENT        (0x7c)  //电池放电电流高 8 位, 低 5 位(0x7d)
#define AXP173_APS_VOLTAGE              (0x7e)  //APS 电压高 8 位, 低 4 位(0x7f)
#define AXP173_CHARGE_COULOMB           (0xb0)  //电池充电库仑计数据寄存器 3,2(0xb1),1(0xb2),0(0xb3)
#define AXP173_DISCHARGE_COULOMB        (0xb4)  //电池放电库仑计数据寄存器 3,2(0xb5),1(0xb6),0(oxb7)
#define AXP173_COULOMB_COUNTER_CONTROL  (0xb8)  //库仑计控制寄存器

/* Computed ADC */
#define AXP173_COULOMB_COUNTER          (0xff)


/* ================================================================= */

/* ADC使能 1, ADC channel bit, 用于控制开启对应ADC功能  reg: 0x82 */
typedef enum {
    ADC_ENABLE_BIT_TS = 0,
    ADC_ENABLE_BIT_APS_VOLT,
    ADC_ENABLE_BIT_VBUS_CURRENT,
    ADC_ENABLE_BIT_VBUS_VOLT,
    ADC_ENABLE_BIT_ACIN_CURRENT,
    ADC_ENABLE_BIT_ACIN_VOLT,
    ADC_ENABLE_BIT_BAT_CURRENT,
    ADC_ENABLE_BIT_BAT_VOLT,
}adc_enable_t;

/* ADC 采样速率 */
typedef enum {
    ADC_FREQ_25Hz,
    ADC_FREQ_50Hz,
    ADC_FREQ_100Hz,
    ADC_FREQ_200Hz,
} adc_freq_t;

/* 充电电流 */
typedef enum {
    CHARGE_CURRENT_mA_100 = 0b0000,
    CHARGE_CURRENT_mA_190,
    CHARGE_CURRENT_mA_280,
    CHARGE_CURRENT_mA_360,
    CHARGE_CURRENT_mA_450,
    CHARGE_CURRENT_mA_550,
    CHARGE_CURRENT_mA_630,
    CHARGE_CURRENT_mA_700,
    CHARGE_CURRENT_mA_780,
    CHARGE_CURRENT_mA_880,
    CHARGE_CURRENT_mA_960,
    CHARGE_CURRENT_mA_1000,
    CHARGE_CURRENT_mA_1080,
    CHARGE_CURRENT_mA_1160,
    CHARGE_CURRENT_mA_1240,
    CHARGE_CURRENT_mA_1320,
}charge_current_t;

/* 输出通道 */
typedef enum {
    OUTPUT_SW_DC1 = 0,
    OUTPUT_SW_LDO4 = 1,
    OUTPUT_SW_LDO2 = 2,
    OUTPUT_SW_LDO3 = 3,
    OUTPUT_SW_DC2 = 4,
    OUTPUT_SW_EXTEN = 6,
} output_switch_t;

/* VHOLD电压 */
typedef enum {
    VHOLD_VOLT_4V = 0,
    VHOLD_VOLT_4V1,
    VHOLD_VOLT_4V2,
    VHOLD_VOLT_4V3,
    VHOLD_VOLT_4V4,
    VHOLD_VOLT_4V5,
    VHOLD_VOLT_4V6,
    VHOLD_VOLT_4V7,
} volt_vhold_t;

/* Voff 关机电压 */
typedef enum {
    VOFF_VOLT_2V6 = 0,
    VOFF_VOLT_2V7,
    VOFF_VOLT_2V8,
    VOFF_VOLT_2V9,
    VOFF_VOLT_3V,
    VOFF_VOLT_3V1,
    VOFF_VOLT_3V2,
    VOFF_VOLT_3V3,
} volt_voff_t;

/* 充电目标电压 */
typedef enum {
    VOLT_CHARGE_TARGET_4V1,     // 4.1V
    VOLT_CHARGE_TARGET_4V15,    // 4.15V
    VOLT_CHARGE_TARGET_4V2,     // 4.2V
    VOLT_CHARGE_TARGET_4V36,    // 4.36V
} volt_charge_target_t;

// /* LDO2 LDO3输出电压 */
// typedef enum {
//     VOLT_LDO23_1V8,
//     VOLT_LDO23_1V9,
//     VOLT_LDO23_2V,
//     VOLT_LDO23_2V1,
//     VOLT_LDO23_2V2,
//     VOLT_LDO23_2V3,
//     VOLT_LDO23_2V4,
//     VOLT_LDO23_2V5,
//     VOLT_LDO23_2V6,
//     VOLT_LDO23_2V7,
//     VOLT_LDO23_2V8,
//     VOLT_LDO23_2V9,
//     VOLT_LDO23_3V,
//     VOLT_LDO23_3V1,
//     VOLT_LDO23_3V2,
//     VOLT_LDO23_3V3,
// } volt_ldo23_t;

/* 长按关机时间 */
typedef enum {
    SHUTDOWN_TIME_4S,
    SHUTDOWN_TIME_6S,
    SHUTDOWN_TIME_8S,
    SHUTDOWN_TIME_10S,
} shutdown_time_t;

/* PEK长按时间 */
typedef enum {
    LONGPRESS_TIME_1S,  //1s
    LONGPRESS_TIME_1S5, //1.5s
    LONGPRESS_TIME_2S,  //2s
    LONGPRESS_TIME_2S5, //2.5s
} longPress_time_t;



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


esp_err_t axp_init(i2c_port_t port, uint8_t addr);

// axp173_handle_t axp173_create(i2c_port_t port, uint8_t addr);

// esp_err_t axp173_delete(axp173_handle_t *axp173);

esp_err_t axp_enable_adc(adc_enable_t adc_bit, uint8_t enable);

esp_err_t axp_get_bat_volt(float *volt);

esp_err_t axp_get_VBUS_volt(float *volt);

esp_err_t axp_get_bat_status(axp173_bat_info_t *bat_info);

esp_err_t axp_power_output_ctrl(output_switch_t ctrl_bit, uint8_t enable);

esp_err_t axp_set_DC2_volt(float volt);

esp_err_t axp_set_DC1_volt(float volt);

esp_err_t axp_set_LDO4_volt(float volt);

esp_err_t axp_set_LDO2_volt(float volt);

esp_err_t axp_set_LDO3_volt(float volt);


/* ================以下未测试==================== */

esp_err_t axp_VHOLD_enable(uint8_t enable);

esp_err_t axp_set_VHOLD_volt(volt_vhold_t volt_select);

esp_err_t axp_set_VOFF_volt(volt_voff_t volt_select);

esp_err_t axp_shutdown(uint8_t shutdown);

esp_err_t axp_charge_enable(uint8_t enable);

esp_err_t axp_set_charge_target_volt(volt_charge_target_t volt_select);

esp_err_t axp_set_charge_end_current(uint8_t end_current);

esp_err_t axp_set_charge_current(charge_current_t current);

esp_err_t axp_internal_temperature_monitor_enable(uint8_t enable);

esp_err_t axp_set_ADC_sampling_freq(adc_freq_t freq_select);

esp_err_t axp_set_TS_PIN_output_Current(uint8_t Current_select);

esp_err_t axp_set_TS_PIN_Current_output_way(uint8_t way_select);

esp_err_t axp_select_TS_PIN_function(uint8_t func_select);

esp_err_t axp_set_shutdown_time(shutdown_time_t time);

esp_err_t axp_set_longPress_time(longPress_time_t time);


//库仑计相关
esp_err_t axp_coulomb_switch(uint8_t enable);

esp_err_t axp_coulomb_counter_pause();

esp_err_t axp_coulomb_counter_clear();

esp_err_t axp_get_charge_coulomb_count(int32_t *charge_count);

esp_err_t axp_get_discharge_coulomb_count(int32_t *discharge_count);


//预设置
esp_err_t apx173_default_setting(void);



#if 0
axp173_handle_t axp173_create(i2c_port_t port, uint8_t addr);

esp_err_t axp173_delete(axp173_handle_t *axp173);

esp_err_t axp173_enable_adc(axp173_handle_t axp173, uint8_t adc_bit, uint8_t enable);

esp_err_t axp173_get_bat_volt(axp173_handle_t axp173, float *volt);

esp_err_t axp173_get_VBUS_volt(axp173_handle_t axp173, float *volt);

esp_err_t axp173_get_bat_status(axp173_handle_t axp173, axp173_bat_info_t *bat_info);

esp_err_t axp173_power_output_ctrl(axp173_handle_t axp173, uint8_t ctrl_bit, uint8_t enable);

esp_err_t axp173_set_DC2_volt(axp173_handle_t axp173, float volt);

esp_err_t axp173_set_DC1_volt(axp173_handle_t axp173, float volt);

esp_err_t axp173_set_LDO4_volt(axp173_handle_t axp173, float volt);

esp_err_t axp173_set_LDO2_volt(axp173_handle_t axp173, float volt);

esp_err_t axp173_set_LDO3_volt(axp173_handle_t axp173, float volt);


/* ================以下未测试==================== */

esp_err_t axp173_VHOLD_enable(axp173_handle_t axp173, uint8_t enable);

esp_err_t axp173_set_VHOLD_volt(axp173_handle_t axp173, uint8_t volt_select);

esp_err_t axp173_set_VOFF_volt(axp173_handle_t axp173, uint8_t volt_select);

esp_err_t axp173_shutdown(axp173_handle_t axp173, uint8_t shutdown);

esp_err_t axp173_charge_enable(axp173_handle_t axp173, uint8_t enable);

esp_err_t axp173_set_charge_target_volt(axp173_handle_t axp173, uint8_t volt_select);

esp_err_t axp173_set_charge_end_current(axp173_handle_t axp173, uint8_t end_current);

esp_err_t axp173_set_charge_current(axp173_handle_t axp173, uint8_t current);

esp_err_t axp173_internal_temperature_monitor_enable(axp173_handle_t axp173, uint8_t enable);

esp_err_t axp173_set_ADC_sampling_freq(axp173_handle_t axp173, uint8_t freq_select);

esp_err_t axp173_set_TS_PIN_output_Current(axp173_handle_t axp173, uint8_t Current_select);

esp_err_t axp173_set_TS_PIN_Current_output_way(axp173_handle_t axp173, uint8_t way_select);

esp_err_t axp173_select_TS_PIN_function(axp173_handle_t axp173, uint8_t func_select);

esp_err_t axp173_coulomb_switch(axp173_handle_t axp173, uint8_t enable);

esp_err_t axp173_coulomb_counter_pause(axp173_handle_t axp173);

esp_err_t axp173_coulomb_counter_clear(axp173_handle_t axp173);

esp_err_t axp173_get_charge_coulomb_count(axp173_handle_t axp173, int32_t *charge_count);

esp_err_t axp173_get_discharge_coulomb_count(axp173_handle_t axp173, int32_t *discharge_count);

esp_err_t axp173_set_shutdown_time(axp173_handle_t axp173, shutdown_time_t time);

esp_err_t axp173_set_longPress_time(axp173_handle_t axp173, longPress_time_t time);

//预设置
esp_err_t apx173_default_setting(axp173_handle_t axp173);

#endif


#endif
