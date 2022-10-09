#ifndef _IR_NVS_H_
#define _IR_NVS_H_

#include "nvs.h"
#include "nvs_flash.h"


#define AC_KEY  "ac"                        //key必须唯一, key相当于文件
#define AC_CODE_NAMESPACE   "ac_space"      //ac命名空间， namespace相当于文件夹


esp_err_t nvs_save_ac_binNUM(uint8_t numToSave, const char *key);
esp_err_t nvs_get_ac_binNUM(const char *key, uint8_t *numToGet);

esp_err_t ir_nvs_init(void);


#endif
