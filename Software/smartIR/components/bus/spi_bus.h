#ifndef _SPI_BUS_H_
#define _SPI_BUS_H_

#include <stdint.h>
#include <stdbool.h>
#include <driver/spi_master.h>

#define PIN_SPI_MISO    (-1)    // SPI master in slave out引脚(=spi_q)
#define PIN_SPI_MOSI    10      // SPI master out slave in引脚(=spi_d)
#define PIN_SPI_CLK     13      // SPI clock引脚
#define PIN_SPI_WP      (-1)    // SPI 写保护引脚
#define PIN_SPI_HOLD    (-1)    // SPI HOLD引脚

#define SPI_BUS_MAX_TRANSFER_SZ   (8192*2)

#define SPI_HOST SPI3_HOST



void spi_bus_init(spi_host_device_t host);
void spi_bus_deinit(spi_host_device_t host);




#endif
