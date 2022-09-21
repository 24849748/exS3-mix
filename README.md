# SmartIR

SmartIR的本意是减少家里的红外遥控器数量

## 关键词：

- ESP32
- ESP-IDF
- LVGL
- IREXT : https://cc.irext.net/index.html#intro



## 硬件信息

自制PCB，先上图片~

【正反图片】



### 引脚

| name      | gpio | name    | gpio |
| --------- | ---- | ------- | ---- |
| VSPI_MOSI | 23   | SDA     | 4    |
| VSPI_MISO | 19   | SCL     | 21   |
| VSPI_CLK  | 18   | CTP_INT | 36   |
| TFT_CS    | 5    | ////    | //// |
| TFT_DC    | 22   | LED     | 2    |
| TFT_RST   | EN   | BUZZ    | 25   |
| TFT_BLK   | 13   | ////    | //// |
| SD_CS     | 27   | ECD_A   | 35   |
| ////      | //// | ECD_B   | 33   |
| PMU_IRQ   | 14   | ECD_BTN | 32   |
| ////      | //// | ////    | //// |
| IR_R      | 34   | IR_T    | 26   |

![image-20220817150621305](C:\Users\bosco\AppData\Roaming\Typora\typora-user-images\image-20220817150621305.png)





## 开发进度

- [x] lv8.2+driver测试
- [x] encoder+button
- [ ] buzz+led
  - [ ] ~~det402无源蜂鸣器，pwm 2-5khz(2k)，使用中断~~
  - [x] led(todo：重构)
- [ ] IR
  - [ ] sd卡
  - [ ] rmt
- [ ] GUI
  - [x] 基础界面
  - [ ] 硬件移植
- [ ] ~~adc 读取电池电压~~
- [x] 使用AXP173模块
  - [ ] axp173硬件层驱动

- [x] 用codeblock模拟lvgl

## Todo（可能做也可能不做）：

- [ ] homekit





