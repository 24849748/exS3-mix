#ifndef _AXP173_INTR_H_
#define _AXP173_INTR_H_

// #define _MERGE(reg, bitNum) ((reg<<8) | bitNum)

#define bitNumMask  0xff
#define regMask     0xff00

// code = ((reg<<8) | bitNum)
// IRQ1
#define IRQ_ACIN_OVERVOLTAGE            (0x4007)    // Defalut: 1
#define IRQ_ACIN_IN                     (0x4006)    // Defalut: 1
#define IRQ_ACIN_OUT                    (0x4005)
#define IRQ_VBUS_OVERVOLTAGE            (0x4004)    // Defalut: 1
#define IRQ_VBUS_IN                     (0x4003)    // Defalut: 1
#define IRQ_VBUS_OUT                    (0x4002)
#define IRQ_VBUS_LESSTHAN_VHOLD         (0x4001)

// IRQ2
#define IRQ_BAT_IN                      (0x4107)    // Defalut: 1
#define IRQ_BAT_OUT                     (0x4106)    // Defalut: 1
#define IRQ_BAT_ENTER_ACTIVE            (0x4105)    // Defalut: 1
#define IRQ_BAT_EXIT_ACTIVE             (0x4104)    // Defalut: 1
#define IRQ_BAT_CHARGING                (0x4103)    // Defalut: 1
#define IRQ_BAT_CHARGING_FINISH         (0x4102)    // Defalut: 1
#define IRQ_BAT_TEMP_HIGH               (0x4101)    // Defalut: 1
#define IRQ_BAT_TEMP_LOW                (0x4100)    // Defalut: 1

// IRQ3
#define IRQ_INTER_TEMP_HIGH                 (0x4207)
#define IRQ_CHARGECURRENT_LESSTHAN_SETTING  (0x4206)
#define IRQ_VOLT_DC1_LESSTHAN_SETTING       (0x4205)    // Defalut: 1
#define IRQ_VOLT_DC2_LESSTHAN_SETTING       (0x4204)    // Defalut: 1
#define IRQ_VOLT_LDO4_LESSTHAN_SETTING      (0x4203)    // Defalut: 1
#define IRQ_SHORT_PRESS                     (0x4201)    // Defalut: 1
#define IRQ_LONG_PRESS                      (0x4200)    // Defalut: 1

// IRQ4
#define IRQ_VBUS_VALID          (0x4305)
#define IRQ_VBUS_INVALID        (0x4304)
#define IRQ_VBUS_SESSION_AB     (0x4303)
#define IRQ_VBUS_SESSION_END    (0x4302)
#define IRQ_APS_VOLT_LOW        (0x4300)    // Defalut: 1

// IRQ5
// #define IRQ_TIMER_TIMEOUT       (0x4a07)



#endif
