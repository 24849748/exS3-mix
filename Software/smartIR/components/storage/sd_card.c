#include "sd_card.h"

#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"

#include "esp_log.h"

#define TAG "sd_card"

/**
 * @brief 初始化SD卡设备,挂载FATFFS文件系统, 需要先初始化spi总线
 * 
 * @return sdmmc_card_t* 
 */
sdmmc_card_t * sdcard_init(void){
    esp_err_t ret;

    esp_vfs_fat_mount_config_t mount_conf = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 1024*16,
    };
    const char mount_point[] = MOUNT_POINT;

    ESP_LOGI(TAG, "Init SD card");
    sdmmc_card_t *card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI3_HOST;


    //sd设备配置
    sdspi_device_config_t sd_dev_conf = {
        .host_id = host.slot,
        .gpio_cs = PIN_SD_CS,
        .gpio_cd = SDSPI_SLOT_NO_CD,
        .gpio_wp = SDSPI_SLOT_NO_WP,
        .gpio_int = GPIO_NUM_NC,
    };

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &sd_dev_conf, &mount_conf, &card); // 挂载sd卡
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
/* void sd_task(void * arg){
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
} */