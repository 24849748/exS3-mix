#include "spi_bus.h"

// #include "driver_conf.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
// #include "freertos/semphr.h"
#include <freertos/task.h>

#include "esp_log.h"

#define TAG "spi_bus.h"

/* ================spi bus================== */

void spi_bus_init(spi_host_device_t host){
    const char *spi_names[] = {"SPI1_HOST","SPI2_HOST","SPI3_HOST"};    // ESP32S3

    ESP_LOGI(TAG, "Configuring SPI host %s", spi_names[host]);
    ESP_LOGI(TAG, "MISO pin: %d, MOSI pin: %d, SCLK pin: %d, IO2/WP pin: %d, IO3/HD pin: %d", PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_CLK, PIN_SPI_WP, PIN_SPI_HOLD);
    ESP_LOGI(TAG, "Max transfer size: %d (bytes)", SPI_BUS_MAX_TRANSFER_SZ);


    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_SPI_MISO,
        .mosi_io_num = PIN_SPI_MOSI,
        .sclk_io_num = PIN_SPI_CLK,
        .quadwp_io_num = PIN_SPI_WP,
        .quadhd_io_num = PIN_SPI_HOLD,
        .max_transfer_sz = SPI_BUS_MAX_TRANSFER_SZ,
    };

    esp_err_t ret = spi_bus_initialize(host, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret == ESP_OK);
}


void spi_bus_deinit(spi_host_device_t host) {
    spi_bus_free(host);
}

/* ==================end==================== */



/**
 *  移植disp_spi
 *  除去disp_dirver
 *  在main.c中初始化 st7789 和 ft6236
 * 
 */
