#pragma once
#include "esp_err.h"

// Initializes SPI bus + SD card + FAT filesystem
esp_err_t spi_bus_init(void);

// Safely unmounts SD card and frees SPI bus
esp_err_t spi_bus_deinit(void);