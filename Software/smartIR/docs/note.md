

## 硬件信息

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

### menuconfig设置

#### 屏幕

![image-20220817150621305](C:\Users\bosco\AppData\Roaming\Typora\typora-user-images\image-20220817150621305.png)



## 开发进度

主体进度

- [x] lv测试
- [x] encoder+button
  - [ ] 取消任务，写lvgl接口
- [ ] buzz+led
  - [ ] det402无源蜂鸣器，pwm 2-5khz(2k)，使用中断
  - [x] led(todo：重构)
- [ ] IR
  - [ ] sd卡
  - [ ] rmt
- [ ] GUI
  - [ ] button+encoder滑条
- [ ] adc 读取电池电压



## Todo：

- [ ] spiffs驱动sd卡（与lcd分时复用）
- [ ] 用codeblock模拟lvgl
- [ ] 裁剪lv_esp32_drivers



## 可选：

- [ ] homekit





## PCB:

- [ ] PCB+电源模块





2022.8.24	移植8.2，还没完成lv_port_disp,移植lvgl_helper和conf

