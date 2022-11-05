#ifndef _ST7789_H_
#define _ST7789_H_

#include "lvgl.h"
#include "spi_bus.h"
#include "st7789_reg.h"


#define ST7789_PIN_RST          11
#define ST7789_PIN_BLK          45
#define ST7789_PIN_DC           21
#define ST7789_PIN_CS           12

#define ST7789_SPI_CLK          40  // 40MHz
#define ST7789_SPI_MODE         (2)
#define ST7789_INPUT_DELAY_NS   (0)

#define ST7789_SPI_HOST         SPI_HOST

#define ST7789_INVERT_COLORS    1  //颜色反转
#define ST7789_ORIENTATION      2  //画面旋转，0--portratit; 1--portratit inverted; 2--landscape; 3--landscape inverted

//水平最大分辨率
#ifndef LV_HOR_RES_MAX  
    #define LV_HOR_RES_MAX    (240)
#endif
//垂直最大分辨率
#ifndef LV_VER_RES_MAX  
    #define LV_VER_RES_MAX    (320)
#endif

#define CUSTOM_DISPLAY_BUFFER_SIZE 1
#if (CUSTOM_DISPLAY_BUFFER_SIZE)
    #define DISP_BUF_SIZE 8192
#else
    #define DISP_BUF_SIZE (LV_HOR_RES_MAX * 40)
#endif

// #define SPI_BUS_MAX_TRANSFER_SZ (DISP_BUF_SIZE * 2)



/* ====================应用层API==================== */
void st7789_init(void);
void st7789_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

// void st7789_send_cmd(uint8_t cmd);
// void st7789_send_data(void *data, uint16_t length);

void st7789_enable_backlight(bool backlight);




#endif
