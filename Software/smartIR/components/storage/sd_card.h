#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <sys/unistd.h>
#include <sys/stat.h>
#include "driver/sdmmc_host.h"

#define MOUNT_POINT "/sdcard"       //挂载点

#define SPI_DMA_CHANNEL SPI_DMA_CH_AUTO

#define PIN_SD_MISO     19
#define PIN_SD_MOSI     23
#define PIN_SD_CLK      18
#define PIN_SD_CS       27

sdmmc_card_t * sdcard_init(void);



#endif
