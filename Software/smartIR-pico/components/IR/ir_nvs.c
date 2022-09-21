#include "ir_nvs.h"

// #include <stdio.h>

// #include "nvs.h"
// #include "nvs_flash.h"
#include "esp_log.h"


#define TAG "ir_nvs"


/**
 * @brief 初始化nvs存储系统
 * 
 * @return esp_err_t 
 */
esp_err_t ir_nvs_init(void){
    ESP_LOGI(TAG, "init ir_nvs system");
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES){
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_LOGI(TAG, "ir_nvs init over");
    return ESP_OK;
}


/**
 * @brief 将ac数据写入nvs
 * 
 * @param code 
 * @param key 
 * @return esp_err_t 
 */
esp_err_t nvs_save_ac_binNUM(uint8_t numToSave, const char *key){
    esp_err_t err;
    nvs_handle handle;
    // size_t size = 0;
    ESP_LOGI(TAG, "open NVS: %s", AC_CODE_NAMESPACE);    
    err = nvs_open(AC_CODE_NAMESPACE, NVS_READWRITE, &handle);
    if(err != ESP_OK){
        ESP_LOGI(TAG, "NVS open fail");
        return err;
    }

    ESP_LOGI(TAG, "write to blob");
    err = nvs_set_u8(handle, key, numToSave);
    if(err != ESP_OK){
        ESP_LOGI(TAG,"set blob fail");
        return err;
    }
    ESP_LOGI(TAG, "save in nvs success,key = %s,item_size = %d", key, 1);

    err = nvs_commit(handle);
    if(err != ESP_OK){
        ESP_LOGI(TAG,"commit blob fail");
        return err;
    }

    nvs_close(handle);
    return ESP_OK;
}


/**
 * @brief 从nvs中读取ac bin_num
 * 
 * @param key           存储bin_num的键值对
 * @param numToGet      待读取的bin_num
 * @return esp_err_t 
 */
esp_err_t nvs_get_ac_binNUM(const char *key, uint8_t *numToGet){
    esp_err_t err;
    nvs_handle handle;
    // size_t size = 0;
    err = nvs_open(AC_CODE_NAMESPACE, NVS_READWRITE, &handle);
    if(err != ESP_OK){
        ESP_LOGI(TAG,"NVS open fail");
        return err;
    }

    // numToGet = (uint8_t *)malloc(sizeof(uint8_t));
    
    ESP_LOGI(TAG, "write to blob");
    err = nvs_get_u8(handle, key, numToGet);
    if(err != ESP_OK){
        ESP_LOGI(TAG,"get u8 blob fail, err = %d, key = %s", err, key);
        return err;
    }
    ESP_LOGI(TAG, "get code = %u from nvs", *numToGet);
    
    nvs_close(handle);
    return ESP_OK;
}