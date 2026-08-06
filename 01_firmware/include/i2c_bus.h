#pragma once

#include "driver/i2c_master.h"

void i2c_bus_init(void);
esp_err_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *data, size_t len);
esp_err_t i2c_write(uint8_t addr, uint8_t reg, uint8_t *data, size_t len);
