#pragma once
#include "esp_err.h"

// Initialize CSV logging system (creates file + header if needed)
esp_err_t sd_logger_init(void);

// Append a row of sensor data to the CSV file
esp_err_t sd_logger_append(float temperature,
                           float humidity,
                           float pressure,
                           float gas_resistance,
                           uint16_t eco2,
                           uint16_t tvoc);
