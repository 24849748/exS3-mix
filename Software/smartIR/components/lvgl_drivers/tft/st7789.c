#include "st7789.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
// #include "sdkconfig.h"
// #include "driver_conf.h"

// #include "disp_spi.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "st7789.h"


#define SPI_TRANSACTION_POOL_SIZE 50	/* maximum number of DMA transactions simultaneously in-flight */

/* DMA Transactions to reserve before queueing additional DMA transactions. A 1/10th seems to be a good balance. Too many (or all) and it will increase latency. */
#define SPI_TRANSACTION_POOL_RESERVE_PERCENTAGE 10
#if SPI_TRANSACTION_POOL_SIZE >= SPI_TRANSACTION_POOL_RESERVE_PERCENTAGE
#define SPI_TRANSACTION_POOL_RESERVE (SPI_TRANSACTION_POOL_SIZE / SPI_TRANSACTION_POOL_RESERVE_PERCENTAGE)	
#else
#define SPI_TRANSACTION_POOL_RESERVE 1	/* defines minimum size */
#endif


typedef enum {
    SPI_BUS_SEND_QUEUED        = 0x00000000,
    SPI_BUS_SEND_POLLING       = 0x00000001,
    SPI_BUS_SEND_SYNCHRONOUS   = 0x00000002,
    SPI_BUS_SIGNAL_FLUSH       = 0x00000004,
    SPI_BUS_RECEIVE            = 0x00000008,
    SPI_BUS_CMD_8              = 0x00000010, /* Reserved */
    SPI_BUS_CMD_16             = 0x00000020, /* Reserved */
    SPI_BUS_ADDRESS_8          = 0x00000040, 
    SPI_BUS_ADDRESS_16         = 0x00000080, 
    SPI_BUS_ADDRESS_24         = 0x00000100, 
    SPI_BUS_ADDRESS_32         = 0x00000200, 
    SPI_BUS_MODE_DIO           = 0x00000400, 
    SPI_BUS_MODE_QIO           = 0x00000800, 
    SPI_BUS_MODE_DIOQIO_ADDR   = 0x00001000, 
	SPI_BUS_VARIABLE_DUMMY		= 0x00002000,
} spi_bus_send_flag_t;


typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes;  //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;


// static spi_host_device_t spi_host;
// static transaction_cb_t chained_post_cb;
static spi_device_handle_t st7789_spi_handle;
static QueueHandle_t TransactionPool = NULL;

static void IRAM_ATTR spi_ready(spi_transaction_t *trans);


/**********************
 *   Low level Func
 **********************/

/* 添加st7789 spi设备 */
void st7789_spi_add_device(spi_host_device_t host){
    ESP_LOGI(TAG, "Adding SPI device");
    ESP_LOGI(TAG, "Clock speed: %dMHz, mode: %d, CS pin: %d", ST7789_SPI_CLK, ST7789_SPI_MODE, ST7789_PIN_CS);
    
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = (ST7789_SPI_CLK*1000*1000),
        .mode = ST7789_SPI_MODE,               
        .spics_io_num = ST7789_PIN_CS,       
        .input_delay_ns = ST7789_INPUT_DELAY_NS,     
        .queue_size = SPI_TRANSACTION_POOL_SIZE,         
        .pre_cb = NULL,         // <-传输前回调函数
        .post_cb = spi_ready,        // <-传输完成回调函数
    };
    
    spi_bus_add_device(host, &devcfg, &st7789_spi_handle);
    // spi_bus_add_device(SPI3_HOST, &devcfg, &st7789_dev_handle);

    /* create the transaction pool and fill it with ptrs to spi_transaction_ext_t to reuse */
    /* 创建传输池并用 指向 spi_transaction_ext_t 的 指针填充它 以重复使用*/
    if(TransactionPool == NULL) {
		TransactionPool = xQueueCreate(SPI_TRANSACTION_POOL_SIZE, sizeof(spi_transaction_ext_t*));
		assert(TransactionPool != NULL);
		for (size_t i = 0; i < SPI_TRANSACTION_POOL_SIZE; i++)
		{
			spi_transaction_ext_t* pTransaction = (spi_transaction_ext_t*)heap_caps_malloc(sizeof(spi_transaction_ext_t), MALLOC_CAP_DMA);
			assert(pTransaction != NULL);
			memset(pTransaction, 0, sizeof(spi_transaction_ext_t));
			xQueueSend(TransactionPool, &pTransaction, portMAX_DELAY);
		}
	}
}

void st7789_wait_for_pending_transactions(void){
    spi_transaction_t *presult;

	while(uxQueueMessagesWaiting(TransactionPool) < SPI_TRANSACTION_POOL_SIZE) {	/* service until the transaction reuse pool is full again */
        if (spi_device_get_trans_result(st7789_spi_handle, &presult, 1) == ESP_OK) {
			xQueueSend(TransactionPool, &presult, portMAX_DELAY);
        }
    }
}

/* 移除st7789 spi设备 */
void st7789_spi_remove_device() {
    /* Wait for previous pending transaction results */
    st7789_wait_for_pending_transactions();

    esp_err_t ret=spi_bus_remove_device(st7789_spi_handle);
    assert(ret==ESP_OK);
}

/* ST7789传输数据 */
void st7789_spi_transaction(const uint8_t *data, size_t length, spi_bus_send_flag_t flags, uint8_t *out, uint64_t addr, uint8_t dummy_bits){
    if(0 == length) {return;}

    spi_transaction_ext_t t={0};
    
    t.base.length = length*8;       // 单位换算为bit

    // spi发送数据或放入buffer
    if(length<=4 && data !=NULL){
        t.base.flags = SPI_TRANS_USE_TXDATA;
        memcpy(t.base.tx_data, data, length);   //将data填充进tx_data
    }else {
        t.base.tx_buffer = data;
    }

    // spi接受数据
    if(flags & SPI_BUS_RECEIVE){
        assert(out != NULL && (flags & (SPI_BUS_SEND_POLLING|SPI_BUS_SEND_SYNCHRONOUS)));
        t.base.rx_buffer = out;
    }

    if(flags & SPI_BUS_ADDRESS_8){
        t.address_bits = 8;
    }else if(flags & SPI_BUS_ADDRESS_16){
        t.address_bits = 16;
    }else if(flags & SPI_BUS_ADDRESS_16){
        t.address_bits = 24;
    }else if(flags & SPI_BUS_ADDRESS_16){
        t.address_bits = 32;
    }
    if(t.address_bits){
        t.base.addr = addr;
        t.base.flags |= SPI_TRANS_VARIABLE_ADDR;
    }
    
    t.base.user = (void *)flags;    // 保存flags用于pre/post回调函数处理

    if(flags & SPI_BUS_SEND_POLLING){
        st7789_wait_for_pending_transactions();
        
        spi_device_polling_transmit(st7789_spi_handle, (spi_transaction_t *)&t);
    }else if(flags & SPI_BUS_SEND_SYNCHRONOUS){
        st7789_wait_for_pending_transactions();
        spi_device_transmit(st7789_spi_handle, (spi_transaction_t *)&t);
    }else {
        if(uxQueueMessagesWaiting(TransactionPool) == 0){
            spi_transaction_t *presult;
            while(uxQueueMessagesWaiting(TransactionPool) < SPI_TRANSACTION_POOL_RESERVE){
                if(spi_device_get_trans_result(st7789_spi_handle, &presult,1)==ESP_OK){
                    xQueueSend(TransactionPool, &presult, portMAX_DELAY);
                }
            }
        }
        spi_transaction_ext_t *pTransaction = NULL;
		xQueueReceive(TransactionPool, &pTransaction, portMAX_DELAY);
        memcpy(pTransaction, &t, sizeof(t));
        if (spi_device_queue_trans(st7789_spi_handle, (spi_transaction_t *) pTransaction, portMAX_DELAY) != ESP_OK) {
			xQueueSend(TransactionPool, &pTransaction, portMAX_DELAY);	/* send failed transaction back to the pool to be reused */
        }
    }
}

void st7789_spi_acquire(void){
    esp_err_t ret = spi_device_acquire_bus(st7789_spi_handle, portMAX_DELAY);
    assert(ret == ESP_OK);
}

void st7789_spi_release(void) {
    spi_device_release_bus(st7789_spi_handle);
}

//require lvgl version > 8 
static void IRAM_ATTR spi_ready(spi_transaction_t *trans){
    spi_bus_send_flag_t flags = (spi_bus_send_flag_t) trans->user;

    if (flags & SPI_BUS_SIGNAL_FLUSH) {
        lv_disp_t * disp = NULL;
        disp = _lv_refr_get_disp_refreshing();
        lv_disp_flush_ready(disp->driver);
    }

    // if (chained_post_cb) {
    //     chained_post_cb(trans);
    // }
}


/* ====================Middle==================== */

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief ST7789发送控制命令
 * 
 * @param cmd 
 */
void st7789_send_cmd(uint8_t cmd) {
    st7789_wait_for_pending_transactions();
    gpio_set_level(ST7789_PIN_DC, 0);
    st7789_spi_transaction(&cmd, 1, SPI_BUS_SEND_POLLING, NULL, 0, 0);
}

/**
 * @brief ST7789发送数据
 * 
 * @param data 
 * @param length 
 */
void st7789_send_data(void * data, uint16_t length) {
    st7789_wait_for_pending_transactions();
    gpio_set_level(ST7789_PIN_DC, 1);
    st7789_spi_transaction(data, length, SPI_BUS_SEND_POLLING, NULL, 0, 0);
}

/**
 * @brief 设置st7789颜色
 * 
 * @param data 
 * @param length 
 */
void st7789_send_color(void * data, size_t length) {
    st7789_wait_for_pending_transactions();
    gpio_set_level(ST7789_PIN_DC, 1);
    st7789_spi_transaction(data, length, SPI_BUS_SEND_QUEUED|SPI_BUS_SIGNAL_FLUSH,NULL,0,0);
}

/**
 * @brief 设置st7789显示方向
 * 
 * @param orientation 
 */
void st7789_set_orientation(uint8_t orientation) {
    // ESP_ASSERT(orientation < 4);
    // assert(orientation < 4);

    const char *orientation_str[] = {
        "PORTRAIT", "PORTRAIT_INVERTED", "LANDSCAPE", "LANDSCAPE_INVERTED"
    };

    ESP_LOGI(TAG, "Display orientation: %s", orientation_str[orientation]);

    uint8_t data[] = {0xC0, 0x00, 0x60, 0xA0};

    ESP_LOGI(TAG, "0x36 command value: 0x%02X", data[orientation]);
    
    st7789_send_cmd(ST7789_MADCTL);
    st7789_send_data((void *) &data[orientation], 1);
}

/**
 * @brief st7789开启背光
 * 
 * @param backlight 
 */
void st7789_enable_backlight(bool backlight){
    ESP_LOGI(TAG,"%s backlight.\n", backlight ? "Enabling" : "Disabling");
    uint32_t tmp = 0;
    tmp = backlight ? 1 : 0;

    gpio_set_level(ST7789_PIN_BLK, tmp);
}



/* ====================Application==================== */

void st7789_init(void){

    ESP_LOGI(TAG, "Display buffer size: %d", DISP_BUF_SIZE);
    ESP_LOGI(TAG, "HOR_MAX: %d, VER_MAX: %d",LV_HOR_RES_MAX, LV_VER_RES_MAX);

    ESP_LOGI(TAG, "Initializing SPI master for display");
    
    st7789_spi_add_device(ST7789_SPI_HOST);     // 在spi bus上添加st7789设备

    lcd_init_cmd_t st7789_init_cmds[] = {
        {0xCF, {0x00, 0x83, 0X30}, 3},
        {0xED, {0x64, 0x03, 0X12, 0X81}, 4},
        {ST7789_PWCTRL2, {0x85, 0x01, 0x79}, 3},
        {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
        {0xF7, {0x20}, 1},
        {0xEA, {0x00, 0x00}, 2},
        {ST7789_LCMCTRL, {0x26}, 1},
        {ST7789_IDSET, {0x11}, 1},
        {ST7789_VCMOFSET, {0x35, 0x3E}, 2},
        {ST7789_CABCCTRL, {0xBE}, 1},
        {ST7789_MADCTL, {0x00}, 1}, // Set to 0x28 if your display is flipped
        {ST7789_COLMOD, {0x55}, 1},

#if ST7789_INVERT_COLORS == 1
		{ST7789_INVON, {0}, 0}, // set inverted mode
#else
 		{ST7789_INVOFF, {0}, 0}, // set non-inverted mode
#endif

        {ST7789_RGBCTRL, {0x00, 0x1B}, 2},
        {0xF2, {0x08}, 1},
        {ST7789_GAMSET, {0x01}, 1},
        {ST7789_PVGAMCTRL, {0xD0, 0x00, 0x02, 0x07, 0x0A, 0x28, 0x32, 0x44, 0x42, 0x06, 0x0E, 0x12, 0x14, 0x17}, 14},
        {ST7789_NVGAMCTRL, {0xD0, 0x00, 0x02, 0x07, 0x0A, 0x28, 0x31, 0x54, 0x47, 0x0E, 0x1C, 0x17, 0x1B, 0x1E}, 14},
        {ST7789_CASET, {0x00, 0x00, 0x00, 0xEF}, 4},
        {ST7789_RASET, {0x00, 0x00, 0x01, 0x3f}, 4},
        {ST7789_RAMWR, {0}, 0},
        {ST7789_GCTRL, {0x07}, 1},
        {0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
        {ST7789_SLPOUT, {0}, 0x80},
        {ST7789_DISPON, {0}, 0x80},
        {0, {0}, 0xff},
    };


    //Initialize non-SPI GPIOs
    gpio_pad_select_gpio(ST7789_PIN_DC);
    gpio_set_direction(ST7789_PIN_DC, GPIO_MODE_OUTPUT);

    //背光初始化在encoder组件中
    // gpio_pad_select_gpio(ST7789_PIN_BLK);
    // gpio_set_direction(ST7789_PIN_BLK, GPIO_MODE_OUTPUT);

#if !defined(ST7789_SOFT_RST)
    gpio_pad_select_gpio(ST7789_PIN_RST);
    gpio_set_direction(ST7789_PIN_RST, GPIO_MODE_OUTPUT);
#endif

    // st7789_enable_backlight(false);
    
    //Reset the display
#if defined(ST7789_SOFT_RST)
    st7789_send_cmd(ST7789_SWRESET);
#else
    gpio_set_level(ST7789_PIN_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(ST7789_PIN_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
#endif

    ESP_LOGI(TAG, "ST7789 initialization.");

    //Send all the commands
    uint16_t cmd = 0;
    while (st7789_init_cmds[cmd].databytes!=0xff) {
        st7789_send_cmd(st7789_init_cmds[cmd].cmd);
        st7789_send_data(st7789_init_cmds[cmd].data, st7789_init_cmds[cmd].databytes&0x1F);
        if (st7789_init_cmds[cmd].databytes & 0x80) {
                vTaskDelay(100 / portTICK_RATE_MS);
        }
        cmd++;
    }

    st7789_set_orientation(ST7789_ORIENTATION);
    // st7789_enable_backlight(true);
}


/* The ST7789 display controller can drive up to 320*240 displays, when using a 240*240 or 240*135
 * displays there's a gap of 80px or 40/52/53px respectively. 52px or 53x offset depends on display orientation.
 * We need to edit the coordinates to take into account those gaps, this is not necessary in all orientations. */
void st7789_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map) {
    uint8_t data[4] = {0};

    uint16_t offsetx1 = area->x1;
    uint16_t offsetx2 = area->x2;
    uint16_t offsety1 = area->y1;
    uint16_t offsety2 = area->y2;

#if (LV_TFT_DISP_OFFSETS)
    offsetx1 += LV_TFT_DISP_X_OFFSETS;
    offsetx2 += LV_TFT_DISP_X_OFFSETS;
    offsety1 += LV_TFT_DISP_Y_OFFSETS;
    offsety2 += LV_TFT_DISP_Y_OFFSETS;

#elif (LV_HOR_RES_MAX == 240) && (LV_VER_RES_MAX == 240)
    #if (LV_DISP_ORIENTATION == 0)          //PORTRAIT
        offsetx1 += 80;
        offsetx2 += 80;
    #elif (LV_DISP_ORIENTATION == 3)        //LANDSCAPE_INVERTED
        offsety1 += 80;
        offsety2 += 80;
    #endif
#elif (LV_HOR_RES_MAX == 240) && (LV_VER_RES_MAX == 135)
    #if (LV_DISP_ORIENTATION == 0) || \     //PORTRAIT
        (LV_DISP_ORIENTATION == 1)          //PORTRAIT_INVERTED
        offsetx1 += 40;
        offsetx2 += 40;
        offsety1 += 53;
        offsety2 += 53;
    #endif
#elif (LV_HOR_RES_MAX == 135) && (LV_VER_RES_MAX == 240)
    #if (LV_DISP_ORIENTATION == 2) || \     //LANDSCAPE
        (LV_DISP_ORIENTATION == 3)          //LANDSCAPE_INVERTED
        offsetx1 += 52;
        offsetx2 += 52;
        offsety1 += 40;
        offsety2 += 40;
    #endif
#endif

    /*Column addresses*/
    st7789_send_cmd(ST7789_CASET);
    data[0] = (offsetx1 >> 8) & 0xFF;
    data[1] = offsetx1 & 0xFF;
    data[2] = (offsetx2 >> 8) & 0xFF;
    data[3] = offsetx2 & 0xFF;
    st7789_send_data(data, 4);

    /*Page addresses*/
    st7789_send_cmd(ST7789_RASET);
    data[0] = (offsety1 >> 8) & 0xFF;
    data[1] = offsety1 & 0xFF;
    data[2] = (offsety2 >> 8) & 0xFF;
    data[3] = offsety2 & 0xFF;
    st7789_send_data(data, 4);

    /*Memory write*/
    st7789_send_cmd(ST7789_RAMWR);

    size_t size = (size_t)lv_area_get_width(area) * (size_t)lv_area_get_height(area);

    st7789_send_color((void*)color_map, size * 2);
}
