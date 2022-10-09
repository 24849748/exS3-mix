/**
 * @file disp_driver.c
 * @brief 
 */

#include "disp_driver.h"
#include "disp_spi.h"
#include "st7789.h"
#include <driver/spi_master.h>
#include "driver_conf.h"

#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "disp_driver"

bool lvgl_spi_driver_init(int host,
    int miso_pin, int mosi_pin, int sclk_pin,
    int max_transfer_sz,
    int dma_channel,
    int quadwp_pin, int quadhd_pin)
{
#if defined(CONFIG_IDF_TARGET_ESP32)
    // assert((SPI_HOST <= host) && (VSPI_HOST > host));
    const char *spi_names[] = {
        "SPI_HOST", "HSPI_HOST", "VSPI_HOST"
    };
    dma_channel = SPI_DMA_CH_AUTO;
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
    assert((SPI_HOST <= host) && (HSPI_HOST > host));
    const char *spi_names[] = {
        "SPI_HOST", "", ""
    };
    dma_channel = SPI_DMA_CH_AUTO;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
    assert((SP1_HOST <= host) && (SPI3_HOST > host));
    const char *spi_names[] = {
        "SPI1_HOST", "SPI2_HOST", "SPI3_HOST"
    };
    dma_channel = SPI_DMA_CH_AUTO;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    //assert((SPI1_HOST <= host) && (SPI3_HOST > host));
    const char *spi_names[] = {
        "SPI_HOST", "HSPI_HOST", "VSPI_HOST"
    };
    dma_channel = SPI_DMA_CH_AUTO;
#else 
#error "Target chip not selected"
#endif

    ESP_LOGI(TAG, "Configuring SPI host %s", spi_names[host]);
    ESP_LOGI(TAG, "MISO pin: %d, MOSI pin: %d, SCLK pin: %d, IO2/WP pin: %d, IO3/HD pin: %d",
        miso_pin, mosi_pin, sclk_pin, quadwp_pin, quadhd_pin);

    ESP_LOGI(TAG, "Max transfer size: %d (bytes)", max_transfer_sz);

    spi_bus_config_t buscfg = {
        .miso_io_num = miso_pin,
        .mosi_io_num = mosi_pin,
        .sclk_io_num = sclk_pin,
        .quadwp_io_num = quadwp_pin,
        .quadhd_io_num = quadhd_pin,
        .max_transfer_sz = max_transfer_sz
    };

    ESP_LOGI(TAG, "Initializing SPI bus...");
    #if defined (CONFIG_IDF_TARGET_ESP32C3)
    dma_channel = SPI_DMA_CH_AUTO;
    #endif
    esp_err_t ret = spi_bus_initialize(host, &buscfg, (spi_dma_chan_t)dma_channel);
    assert(ret == ESP_OK);

    return ESP_OK != ret;
}


/*Initialize your display and the required peripherals. 初始化SPI屏幕*/
void driver_disp_init(void){
    ESP_LOGI(TAG, "Display buffer size: %d", DISP_BUF_SIZE);
    ESP_LOGI(TAG, "HOR_MAX: %d, VER_MAX: %d",LV_HOR_RES_MAX,LV_VER_RES_MAX);

    ESP_LOGI(TAG, "Initializing SPI master for display");

    lvgl_spi_driver_init(TFT_SPI_HOST,
        DISP_SPI_MISO, DISP_SPI_MOSI, DISP_SPI_CLK,
        SPI_BUS_MAX_TRANSFER_SZ, 1,
        DISP_SPI_IO2, DISP_SPI_IO3);

    disp_spi_add_device(TFT_SPI_HOST);
    
    st7789_init();

    //backlight
    /* const disp_backlight_config_t bckl_config = {
        .gpio_num = ST7789_BLK,
#if defined CONFIG_LV_DISP_BACKLIGHT_PWM
        .pwm_control = true,
#else
        .pwm_control = false,
#endif
#if defined CONFIG_LV_BACKLIGHT_ACTIVE_LVL
        .output_invert = false, // Backlight on high
#else
        .output_invert = true, // Backlight on low
#endif
        .timer_idx = 0,
        .channel_idx = 0 // @todo this prevents us from having two PWM controlled displays
    };
    disp_backlight_h bckl_handle = disp_backlight_new(&bckl_config);
    disp_backlight_set(bckl_handle, 100);
    return bckl_handle; */
    // tft_backlight_init();

}


void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map){
    st7789_flush(drv, area, color_map);
}

void tft_backlight_init(void){

    //zero-initialize the config structure.
    // gpio_config_t io_conf = {
    //     .intr_type = GPIO_INTR_DISABLE,
    //     .mode = GPIO_MODE_OUTPUT,
    //     .pin_bit_mask = ((1ULL << 13)),
    //     .pull_down_en = 0,
    //     .pull_up_en = 0
    // };
    // gpio_config(&io_conf);

    // gpio_set_direction(13, GPIO_MODE_DEF_OUTPUT);
    gpio_pad_select_gpio(13);
    gpio_set_direction(13, GPIO_MODE_OUTPUT);

    gpio_set_level(13, 0);
}