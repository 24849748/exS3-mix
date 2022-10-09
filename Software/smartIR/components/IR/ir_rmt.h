#ifndef _IR_RMT_H_
#define _IR_RMT_H_

#include "ir_decode.h"

typedef struct {
    uint8_t bin_num;                //记录.bin文件的序号, 0~8
    t_remote_ac_status status;      //irext的空调状态
} ac_control_t;
ac_control_t ac;


void IR_init(void);
void execute_ac(void);

#endif
