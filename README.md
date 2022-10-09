# SmartIR

SmartIR的本意是减少家里的红外遥控器数量

## 关键词：

- ESP32
- ESP-IDF
- LVGL
- IREXT : https://cc.irext.net/index.html#intro



## 硬件信息

自制PCB，先上图片~

【to add】



### 引脚（基于exS3）

| name     | gpio   | name     | gpio   |
| -------- | ------ | -------- | ------ |
| SPI_MOSI | 10     | I2C_SDA  | 48     |
| SPI_MISO | 14     | I2C_SCL  | 47     |
| SPI_CLK  | 13     | CTP_INT  | 46     |
| TFT_CS   | 12     | ////     | ////   |
| TFT_DC   | 21     | LED      | 1      |
| TFT_RST  | 11     | BUZZ     | 38     |
| TFT_BLK  | 45     | ////     | ////   |
| SD_CS    | 9      | ECD_A    | 6      |
| ////     | ////   | ECD_B    | 8      |
| PMU_IRQ  | 39     | ECD_BTN  | 15     |
| ////     | ////   | ////     | ////   |
| ~~IR_R~~ | ~~34~~ | ~~IR_T~~ | ~~26~~ |




