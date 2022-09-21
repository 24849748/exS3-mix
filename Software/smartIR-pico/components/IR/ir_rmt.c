#include "ir_rmt.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/rmt.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#include "ir_nvs.h"

#define TAG "IR"

#define IR_TASK_PRIORITY    5
#define IR_TASK_SIZE        2048

#define RMT_TX_CHANNEL      1
#define RMT_TX_PIN_NUM      26      
#define RMT_TX_CRRIER_EN    1  //载波使能

#define RMT_CLK_DIV         100     //rmt计数器分频系数
#define RMT_TICK_10_US      (800000000/RMT_CLK_DIV/100000)  //rmt 10us计数器的值

/* nec协议 */
#define HEADER_HIGH_9000US  9000
#define HEADER_LOW_4500US   4500
#define HEADER_HIGH_4300US  4300
#define HEADER_LOW_4300US   4300
#define HEADER_HIGH_7300US  7300
#define HEADER_LOW_5800US   5800
#define HEADER_HIGH_3000US  3000
#define HEADER_LOW_3000US   3000
#define HEADER_HIGH_4500US  4500

#define NEC_CONNECT_HIGH_US 646     /*!< NEC protocol CONNECT: positive 0.6ms*/
#define NEC_CONNECT_LOW_US  20000   /*!< NEC protocol CONNECT: negative 2ms*/
#define NEC_BIT_ONE_HIGH_US 646     /*!< NEC protocol data bit 1: positive 0.56ms */
#define NEC_BIT_ONE_LOW_US  1643    /*!< NEC protocol data bit 1: negative 1.69ms */
#define NEC_BIT_ZERO_HIGH_US 646    /*!< NEC protocol data bit 0: positive 0.56ms */
#define NEC_BIT_ZERO_LOW_US 516     /*!< NEC protocol data bit 0: negative 0.56ms */
#define NEC_BIT_END         646     /*!< NEC protocol end: positive 0.56ms */
#define NEC_BIT_MARGIN      200     /*!< NEC parse margin time */

#define NEC_ITEM_DURATION(d) ((d & 0x7fff) * 10 / RMT_TICK_10_US) /*!< Parse duration time from memory register value */
#define NEC_DATA_ITEM_NUM 70                                      /*!< NEC code item number: header + 35bit data + connect +32bit +end*/
#define RMT_TX_DATA_NUM 2                                         /*!< NEC tx test data number */
#define rmt_item32_tIMEOUT_US 21000                               /*!< RMT receiver timeout value(us) 由于连接码的时间长度大约为20600us所以设置时间长点 */


#define DEFAULT_BIN_NUM     0       //默认使用哪个bin文件, 0~8


static const int tx_channel = RMT_TX_CHANNEL;

EXT_RAM_ATTR uint16_t decode[1024] = {0};   //红外二进制文件解码的数据


TaskHandle_t ir_tx_handle;          //发射任务句柄
static SemaphoreHandle_t ir_sem;    //红外信号量，限制同时收发

//ir二进制文件路径，sd卡访问需要加/sdcard, 由bin_num指定
const char *ir_code_path[] = {
    "/sdcard/ac/ac_2132.bin",
    "/sdcard/ac/ac_2582.bin",
    "/sdcard/ac/ac_2587.bin",
    "/sdcard/ac/ac_2970.bin",
    "/sdcard/ac/ac_10012.bin",
    "/sdcard/ac/ac_10016.bin",
    "/sdcard/ac/ac_10727.bin",
    "/sdcard/ac/ac_25077.bin",
    "/sdcard/ac/ac_25097.bin",

    "/sdcard/fan/fan_1.bin",
    "/sdcard/fan/fan_2.bin",
    "/sdcard/fan/fan_3.bin",
    "/sdcard/fan/fan_4.bin",
    "/sdcard/fan/fan_5.bin",
    "/sdcard/fan/fan_6.bin",
}; 

//——————————————————————————————————————————————//
/* nec协议 */
/**
 * @brief 填充item电平和电平时间 需要将时间转换成计数器的计数值
 * 
 * @param item      要填入的item
 * @param high_us   高电平
 * @param low_us    低电平
 */
static void nec_fill_item_level(rmt_item32_t *item, int high_us, int low_us){
    item->level0 = 1;
    item->duration0 = (high_us) / 10 * RMT_TICK_10_US;
    item->level1 = 0;
    item->duration1 = (low_us) / 10 * RMT_TICK_10_US;
}

/**
 * @brief 通过irext构建item
 * 
 * @param item 
 * @param item_num 
 */
static void irext_build(rmt_item32_t *item, size_t item_num){
    int i = 0;
    nec_fill_item_level(item, decode[0], decode[0]);
    for (i = 1; i < item_num; i++)
    {
        item++;
        nec_fill_item_level(item, decode[2 * i], decode[2 * i + 1]);
    }
}


//----------------------空调功能set get-------------------------//
//通知执行发射任务，在每次设置status后调用
void execute_ac(void){
    xSemaphoreTake(ir_sem,portMAX_DELAY);
    xTaskNotifyGive(ir_tx_handle);
}


/**
 * @brief 设置空调状态
 * 
 * @param power_on 电源开关
 * @param temp      温度
 * @param speed     风速
 * @param mode      模式
 * @return esp_err_t 
 */
esp_err_t ac_set_status(bool power_on, int temp, t_ac_wind_speed speed, int mode){
    
    //电源
    if(power_on){
        ac.status.ac_power = AC_POWER_ON;
    }else{
        ac.status.ac_power = AC_POWER_OFF;
    }

    //温度
    if(temp > 28 || temp < 16){
        ESP_LOGI(TAG,"error temperature");
        return ESP_FAIL;
    }
    ac.status.ac_temp = temp - 16;

    //风速
    if(speed < 0 || speed > 3){
        ESP_LOGI(TAG,"error windspeed");
        return ESP_FAIL;
    }

    execute_ac();

    return ESP_OK;
}



//——————————————————————————————————————————————//
/* 硬件 */

/**
 * @brief 初始化RMT发射器
 * 
 */
static void nec_tx_init(){

    ESP_LOGI(TAG, "rmt config...");
    rmt_config_t rmt_tx = {
        .channel = tx_channel,      //发射通道
        .clk_div = RMT_CLK_DIV,     //分频系数
        .gpio_num = RMT_TX_PIN_NUM, //发射引脚
        .mem_block_num = 2,         //内存块，1个内存块64个item，格力的空调有70个
        .rmt_mode = RMT_MODE_TX,    //rmt模式，发射模式
        
        .tx_config.loop_en =false,                  //关闭循环，只发射一次
        .tx_config.idle_output_en = true,           //空闲输出使能
        .tx_config.idle_level = 0,                  //空闲电平
        .tx_config.carrier_en = RMT_TX_CRRIER_EN,   //载波使能
        .tx_config.carrier_duty_percent = 50,       //载波占空比
        .tx_config.carrier_freq_hz = 38000,         //载波频率
        .tx_config.carrier_level = 1,               //载波电平
    };
    rmt_config(&rmt_tx);

    ESP_LOGI(TAG, "rmt install rmt driver");
    rmt_driver_install(rmt_tx.channel, 0,0);
}



/**
 * @brief 红外发射任务
 * 
 * @param arg 
 */
void IR_txTask(void *arg){
    (void)arg;

    rmt_item32_t *item;
    size_t size = 0;    //item所需内存
    int item_num = 0;
    while (1)
    {
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY); //等待消息通知
        ESP_LOGI(TAG, "ir_tx_irext: power = %d, temp = %d",ac.status.ac_power,ac.status.ac_temp);
        ESP_LOGI(TAG, "using code lib: %s", ir_code_path[ac.bin_num]);

        //打开irext .bin 文件
        if(ir_file_open(REMOTE_CATEGORY_AC,SUB_CATEGORY_QUATERNARY,ir_code_path[ac.bin_num]) != 0){
            ESP_LOGI(TAG,"open file fail");
            xSemaphoreGive(ir_sem);
            break;
        }

        //根据ac.status解码出特定的红外序列，存储在decode_data里
        uint16_t decode_data = ir_decode(KEY_AC_POWER, decode, &ac.status, 0);
        //检查
        if(decode_data > 200){
            decode_data = (decode_data + 1) / 2;
        }
        //关闭解码库，释放内存
        ir_close();

        item_num = (decode_data / 2);   //解码出来的序列是重复的，取一半
        size = (sizeof(rmt_item32_t) * item_num);
        item = (rmt_item32_t *)malloc(size);
        irext_build(item, item_num);

        ESP_LOGI(TAG, "write item num = %d", item_num);
        rmt_write_items(tx_channel, item, item_num, true);  //将item写入发射通道RAM
        rmt_wait_tx_done(tx_channel, portMAX_DELAY); //等发射

        free(item);
        xSemaphoreGive(ir_sem);
    }
    vTaskDelete(NULL);
}

/**
 * @brief 红外初始化
 * 
 */
void IR_init(void){
    ESP_LOGI(TAG, "init IR");

    //初始化空调红外结构体
    ac.status.ac_mode = AC_MODE_COOL;   //制冷
    ac.status.ac_power = AC_POWER_OFF;  //电源关闭
    ac.status.ac_temp = AC_TEMP_26;
    ac.status.ac_wind_dir = AC_SWING_ON;
    ac.status.ac_wind_speed = AC_WS_LOW;
    //nvs存储上次使用的码库
    uint8_t *temp = 0;;
    nvs_get_ac_binNUM(AC_KEY, temp);
    //第一次使用，nvs中无保存的码库编号
    if(temp == NULL){
        ESP_LOGI(TAG, "use default bin file");
        ac.bin_num = DEFAULT_BIN_NUM;
        if(nvs_save_ac_binNUM(ac.bin_num, AC_KEY) != ESP_OK){
            ESP_LOGI(TAG, "fail to save ac bin_num to nvs");
        }
        ESP_LOGI(TAG, "set ac default bin file: %u", ac.bin_num);
    }else {
        ESP_LOGI(TAG, "get code successed, code = %u", *temp);
        ac.bin_num = *temp;
        // free(temp);
    }


    //ir信号量，同步发射
    vSemaphoreCreateBinary(ir_sem);

    //初始化nec协议
    nec_tx_init();

    xTaskCreate(IR_txTask, "ir_tx", IR_TASK_SIZE, NULL, IR_TASK_PRIORITY, &ir_tx_handle);

    ESP_LOGI(TAG,"IR init over");
}

/* 
#todo(tomorrow)：
    代码整理
    复习指针部分

 */