#ifndef _AXP173_TYPE_H_
#define _AXP173_TYPE_H_


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
    OUTPUT_CH_DC1 = 0,
    OUTPUT_CH_LDO4 = 1,
    OUTPUT_CH_LDO2 = 2,
    OUTPUT_CH_LDO3 = 3,
    OUTPUT_CH_DC2 = 4,
    OUTPUT_CH_EXTEN = 6,
} output_channel_t;


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


/*  */
typedef enum {
    TS_CURRENT_20uA,
    TS_CURRENT_40uA,
    TS_CURRENT_60uA,
    TS_CURRENT_80uA,
} TS_output_current_t;


/* TS pin 电流输出方式 */
typedef enum {
    C_OUTPUT_WAY_CLOSE,     // 关闭
    C_OUTPUT_WAY_CHARGE,    // 充电时
    C_OUTPUT_WAY_SAMPLING,  // ADC 采样时
    C_OUTPUT_WAY_ALWAYS,    // 一直打开
} TS_current_output_way_t;   




typedef enum {
    // IRQ1
    IRQ_ACIN_OVERVOLTAGE,
    IRQ_ACIN_IN,
    IRQ_ACIN_OUT,
    IRQ_VBUS_OVERVOLTAGE,
    IRQ_VBUS_IN,
    IRQ_VBUS_OUT,
    IRQ_VBUS_LESSTHAN_VHOLD,

    // IRQ2
    IRQ_BAT_IN,
    IRQ_BAT_OUT,
    IRQ_BAT_ENTER_ACTIVE,
    IRQ_BAT_EXIT_ACTIVE,
    IRQ_BAT_CHARGING,
    IRQ_BAT_CHARGING_FINISH,
    IRQ_BAT_TEMP_HIGH,
    IRQ_BAT_TEMP_LOW,

    // IRQ3
    IRQ_INTER_TEMP_HIGH,
    IRQ_CHARGECURRENT_LESSTHAN_SETTING,
    IRQ_VOLT_DC1_LESSTHAN_SETTING,
    IRQ_VOLT_DC2_LESSTHAN_SETTING,
    IRQ_VOLT_LDO4_LESSTHAN_SETTING,
    
    IRQ_SHORT_PRESS,
    IRQ_LONG_PRESS,

    // IRQ4
    IRQ_VBUS_VALID,
    IRQ_VBUS_INVALID,
    IRQ_VBUS_SESSION_AB,
    IRQ_VBUS_SESSION_END,
    IRQ_APS_VOLT_LOW,

    // IRQ5
    IRQ_TIMER_TIMEOUT,
} IRQ_event_t;


#endif
