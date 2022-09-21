#ifndef _SD_SPI_FATFS_H_
#define _SD_SPI_FATFS_H_

#include <sys/unistd.h>
#include <sys/stat.h>
#include "driver/sdmmc_host.h"

#define MOUNT_POINT "/sdcard"       //挂载点

#define SPI_DMA_CHANNEL SPI_DMA_CH2

#define PIN_SD_MISO     19
#define PIN_SD_MOSI     23
#define PIN_SD_CLK      18
#define PIN_SD_CS       27

sdmmc_card_t * sd_spi_fatfs_init(void);

void sd_task(void *arg);

#endif
