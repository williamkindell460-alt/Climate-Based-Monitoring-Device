#pragma once
#include "esp_err.h"

#define CCS811_ADDR               0x5A
#define CCS811_REG_STATUS         0x00
#define CCS811_REG_MEAS_MODE      0x01
#define CCS811_REG_ALG_RESULT_DATA 0x02
#define CCS811_REG_ERROR_ID       0xE0

#define CCS811_CMD_APP_START      0xF4

/* Status bits */
#define CCS811_STATUS_FW_MODE     0x10  // 1 = application mode
#define CCS811_STATUS_DATA_READY  0x08  // 1 = new data available
#define CCS811_STATUS_ERROR       0x01  // 1 = error present

esp_err_t ccs811_init(void);
esp_err_t ccs811_read(uint16_t *eco2, uint16_t *tvoc);
