#ifndef _AXP173_H_
#define _AXP173_H_

// #include "i2c_bus.h"


#define AXP173_I2C_ADDR         (0x34)      //axp173 i2c address

#define AXP173_ADDR_READ        (0x69)
#define AXP173_ADDR_WRITE       (0x68)



/* registers */
#define AXP173_POWER_STATUS     (0x00)
#define AXP173_CHARGE_STATUS    (0x01)
#define AXP173_OTG_VBUS_STATUS  (0x04)


#define AXP173_DATA_BUFFER_0    (0x06)  //default: F0
#define AXP173_DATA_BUFFER_1    (0x07)  //default: 0F
#define AXP173_DATA_BUFFER_2    (0x08)  //default: 00
#define AXP173_DATA_BUFFER_3    (0x09)  //default: FF
#define AXP173_DATA_BUFFER_4    (0x0a)  //default: 00
#define AXP173_DATA_BUFFER_5    (0x0b)  //default: 00

#define AXP173_EXTEN_DC2_SW     (0x10)  //default: X5H  EXTEN & DC-DC2 开关控制寄存器
#define AXP173_DC1_LDO234_SW    (0x12)  //default: XFH  DC-DC1/LDO4 & LDO2/3 开关控制寄存器


#define AXP173_DC2_VOLT         (0x23)  //default: 16H  DC-DC2 电压设置寄存器
#define AXP173_DC2_VOLT_SLOPE   (0x25)  //default: 00H  DC-DC2 电压斜率参数设置寄存器

/* 电压设置 */
#define AXP173_DC1_VOLT         (0x26)
#define AXP173_LDO4_VOLT        (0x27)
#define AXP173_LDO23_VOLT       (0x28)

#define AXP173_VBUS_TO_IPSOUT   (0x30)  //VBUS-IPSOUT 通路设置寄存器
#define AXP173_SHUTDOWN_VOLT    (0x31)  //VOFF 关机电压设置寄存器
#define AXP173_SHUTDOWN_BAT_CHGLED  (0x32)  //关机、电池检测、CHGLED 控制寄存器

#define AXP173_CHARGE_CONTROL_1 (0x33)  //充电控制寄存器 1
#define AXP173_CHARGE_CONTROL_2 (0x34)  //充电控制寄存器 2

#define AXP173_PEK              (0x36)  //PEK 参数设置寄存器
#define AXP173_DCDC_FREQUENCY   (0x37)  //DCDC 转换器工作频率设置寄存器

#define AXP173_BAT_CHG_L_TEMP   (0x38)  //电池充电低温报警设置寄存器
#define AXP173_BAT_CHG_H_TEMP   (0x39)  //电池充电高温报警设置寄存器

#define AXP173_APS_LOW_POWER1   (0x3a)  //APS 低电 Level1 设置寄存器
#define AXP173_APS_LOW_POWER2   (0x3b)  //APS 低电 Level2 设置寄存器


#define AXP173_BAT_DISCHG_L_TEMP    (0x3c)  //电池放电低温报警设置寄存器
#define AXP173_BAT_DISCHG_H_TEMP    (0x3d)  //电池放电高温报警设置寄存器
#define AXP173_DCDC_MODE        (0x80)  //DCDC 工作模式设置寄存器
#define AXP173_ADC_ENABLE_1     (0x82)  //ADC 使能设置寄存器 1
#define AXP173_ADC_ENABLE_2     (0x83)  //ADC 使能设置寄存器 2
#define AXP173_ADC_RATE_TS_PIN  (0x84)  //ADC 采样率设置，TS pin 控制寄存器


#define AXP173_TIMER_CONTROL            (0x8a)  //定时器控制寄存器
#define AXP173_VBUS_MONITOR             (0x8b)  //VBUS 监测设置寄存器
#define AXP173_TEMP_SHUTDOWN_CONTROL    (0x8f)  //过温关机控制寄存器


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

/* IOCTL commands */
#define	AXP173_READ_POWER_STATUS        (0x0001)
#define	AXP173_READ_CHARGE_STATUS       (0x0101)

#define AXP173_COULOMB_COUNTER_ENABLE   (0xb801)
#define AXP173_COULOMB_COUNTER_DISABLE  (0xb802)
#define AXP173_COULOMB_COUNTER_SUSPEND  (0xb803)
#define AXP173_COULOMB_COUNTER_CLEAR    (0xb804)

#define AXP173_LDOIO0_ENABLE            (0x9000)
#define AXP173_LDOIO0_DISABLE           (0x9001)

#define AXP173_DCDC2_ENABLE             (0x1000)
#define AXP173_DCDC2_DISABLE            (0x1001)
#define AXP173_EXTEN_ENABLE             (0x1002)
#define AXP173_EXTEN_DISABLE            (0x1003)

#define AXP173_LDO2_ENABLE              (0x1200)
#define AXP173_LDO2_DISABLE             (0x1201)
#define AXP173_LDO3_ENABLE              (0x1202)
#define AXP173_LDO3_DISABLE             (0x1203)
#define AXP173_DCDC1_ENABLE             (0x1204)
#define AXP173_DCDC1_DISABLE            (0x1205)
#define AXP173_LDO4_ENABLE              (0x1206)
#define AXP173_LDO4_DISABLE             (0x1207)

#define AXP173_DCDC1_SET_VOLTAGE        (0x2600)
#define AXP173_DCDC2_SET_VOLTAGE        (0x2300)
#define AXP173_LDO4_SET_VOLTAGE         (0x2700)
#define AXP173_LDO2_SET_VOLTAGE         (0x2800)
#define AXP173_LDO3_SET_VOLTAGE         (0x2801)
#define AXP173_LDOIO0_SET_VOLTAGE       (0x9100)

#define AXP173_LOW                      (0)
#define AXP173_HIGH                     (1)

#define AXP173_GPIO0_SET_LEVEL          (0x9400)
#define AXP173_GPIO1_SET_LEVEL          (0x9401)
#define AXP173_GPIO2_SET_LEVEL          (0x9402)
#define AXP173_GPIO4_SET_LEVEL          (0x9601)

/* Error codes */
#define AXP173_OK                       (0)
#define AXP173_ERROR_NOTTY              (-1)
#define AXP173_ERROR_EINVAL             (-22)
#define AXP173_ERROR_ENOTSUP            (-95)


typedef void *axp173_handle_t;


axp173_handle_t axp173_create(i2c_bus_handle_t bus, uint8_t dev_addr);
esp_err_t axp173_delete(axp173_handle_t *axp);
esp_err_t axp173_get_power_status(axp173_handle_t axp);
esp_err_t axp173_get_bat_status(axp173_handle_t axp);
esp_err_t axp173_get_bat_volt(axp173_handle_t axp);

esp_err_t axp173_init();
esp_err_t axp173_read();
esp_err_t axp173_write();
esp_err_t axp173_ctrl();



#endif


/* 
    power off
    VBUS exist

 */