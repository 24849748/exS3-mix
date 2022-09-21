#include "sd_spi_fatfs.h"

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"

#define TAG "sd_spi_fatffs"

/**
 * @brief 使用SPI总线挂载SD卡的FATFFS文件系统
 *      - (暂时不用了)需要先初始化spi总线 spi_bus_config_t、spi_bus_initialize()
 *      - DMA 通道，CLK频率20MHz
 * 
 * @return sdmmc_card_t* 
 */
sdmmc_card_t * sd_spi_fatfs_init(void){
    esp_err_t ret;

    //挂载点
    esp_vfs_fat_mount_config_t mount_conf = {
        .format_if_mount_failed = true,     //如果挂载失败自动格式化
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Init SD card");

    //sd卡句柄
    sdmmc_card_t * card;

    //spi host配置
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = VSPI_HOST;

    //spi bus配置
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_SD_MISO,              // MISO引脚定义
        .mosi_io_num = PIN_SD_MOSI,              // MOSI引脚定义
        .sclk_io_num = PIN_SD_CLK,               // CLK引脚定义
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 8192,        // 最大传输字节数
    };
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_CHANNEL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return NULL;
    }

    //spi 设备配置
    sdspi_device_config_t slot_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_conf.gpio_cs = PIN_SD_CS;
    slot_conf.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_conf, &mount_conf, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return NULL;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    sdmmc_card_print_info(stdout, card);
    return card;
}

/**
 * @brief SD卡任务
 * 
 * @param arg 
 */
void sd_task(void * arg){
    sdmmc_card_t * card = sd_spi_fatfs_init();
    if(card == NULL){
        ESP_LOGE(TAG, "sd card init failed!");
        return;
    }

    // 使用POSIX和C标准库函数来处理文件。
    // 首先创建一个文件。
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen(MOUNT_POINT"/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello %s!\n", card->cid.name);  // 将字符串写入到文件中
    fclose(f);  // 关闭文件
    ESP_LOGI(TAG, "File written");

    // 重命名前检查目标文件是否存在
    struct stat st;
    if (stat(MOUNT_POINT"/foo.txt", &st) == 0) {
        unlink(MOUNT_POINT"/foo.txt");  // 删除（如果存在）
    }

    // 重命名原始文件
    ESP_LOGI(TAG, "Renaming file");
    if (rename(MOUNT_POINT"/hello.txt", MOUNT_POINT"/foo.txt") != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // 打开重命名的文件以供阅读
    ESP_LOGI(TAG, "Reading file");
    f = fopen(MOUNT_POINT"/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    
    // 读文件
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);

    // 末尾换行符后，插入\0，表示字符串结束
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    while(1) {
		vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
