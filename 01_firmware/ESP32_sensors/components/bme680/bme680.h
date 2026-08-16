#pragma once
#include "esp_err.h"
#include <stdbool.h>

/* 
   Device address and chip ID
*/
// The BME680 has two possible addresses: 0x76 (default), 0x77 (if SDO pin is pulled high). The BME680 uses 0x76
// We use 0xD0 because it is the BME680's chip ID register, and this should be 0x61 based on the datasheet.
// As noted in Chapter 5.3.1.5.
#define BME680_ADDR 0x76
#define BME680_CHIP_ID 0xD0

/* 
   Status registers
*/
#define BME680_REG_STATUS        0x1D
#define BME680_REG_MEAS_STATUS   0x1B

/* 
   Raw data registers
*/
#define BME680_REG_TEMP_MSB      0x22
#define BME680_REG_PRESS_MSB     0x1F
#define BME680_REG_HUM_MSB       0x25
#define BME680_REG_GAS_MSB       0x2A

/* 
   Configuration registers
*/
#define BME68X_REG_CTRL_HUM      0x72
#define BME68X_REG_CTRL_MEAS     0x74
#define BME68X_REG_CONFIG        0x75

/* 
   Gas / heater control
*/
#define BME68X_REG_CTRL_GAS_1    0x71
#define BME68X_REG_RES_HEAT_0    0x5A
#define BME68X_REG_GAS_WAIT_0    0x64

/* 
   Calibration coefficient blocks
*/
// This value reads the compensation for temp and pressure from 0x89–0xA1,
// defined in Table 11 and 12, where 0x89 and 0xA1 are starting and ending registers
// and hold no meaningful value.
#define STARTING_REGISTER_FOR_TEMP_PRESSURE 0x89

// This value is the beginning of the humidity and gas calibration coefficients
// defined in Table 13 and 14, which covers 0xE1–0xF0, where 0xE1 does hold data,
// and 0xF0 is an ending register and holds no meaningful value.
#define STARTING_REGISTER_FOR_HUMIDITY_GAS  0xE1

// This is defined in 5.3.4 in the datasheet.
#define STARTING_REGISTER_FOR_RAW_VALUES    0x1F

/* 
   Register map documentation (from Bosch datasheet)
*/

/*
Register | buf index | Parameter | Decode
---------------------------------------------------------------
0xE1 | buf2[0] | lower nibble for par_h2 and upper nibble for par_h1 | used in bit‑combine expressions below
0xE2 | buf2[1] | shared byte for par_h1 and par_h2
0xE3 | buf2[2] | MSB of par_h1
0xE4 | buf2[3] | par_h3 (signed 8‑bit) | par_h3 = (int8_t)buf2[3];
0xE5 | buf2[4] | par_h4 (signed 8‑bit) | par_h4 = (int8_t)buf2[4];
0xE6 | buf2[5] | par_h5 (signed 8‑bit) | par_h5 = (int8_t)buf2[5];
0xE7 | buf2[6] | par_h6 (unsigned 8‑bit) | par_h6 = buf2[6];
0xE8 | buf2[7] | par_h7 (signed 8‑bit) | par_h7 = (int8_t)buf2[7];
0xE9 | buf2[8] | LSB par_t1 | par_t1 = (uint16_t)((buf2[9] << 8) | buf2[8]);
0xEA | buf2[9] | MSB par_t1 | (see above)
0xEB | buf2[10] | LSB par_g2 | par_g2 = (int16_t)((buf2[11] << 8) | buf2[10]);
0xEC | buf2[11] | MSB par_g2 | (see above)
0xED | buf2[12] | par_g1 (signed 8‑bit) | par_g1 = (int8_t)buf2[12];
0xEE | buf2[13] | par_g3 (signed 8‑bit) | par_g3 = (int8_t)buf2[13];
0xEF | buf2[14] | res_heat_range (bits 5:4) | res_heat_range = (buf2[14] & 0x30) >> 4;
0xF0 | buf2[15] | res_heat_val and range_sw_err (lower nibble) | res_heat_val = (int8_t)buf2[15]; range_sw_err = (int8_t)(buf2[15] & 0x0F);
*/

/*
Address | Register | Purpose
---------------------------------------------------------------
0x1F–0x21 | press_msb, press_lsb, press_xlsb | Raw pressure (20 bits)
0x22–0x24 | temp_msb, temp_lsb, temp_xlsb | Raw temperature (20 bits)
0x25–0x26 | hum_msb, hum_lsb | Raw humidity (16 bits)
0x2A–0x2E | gas_r_msb, gas_r_lsb, gas_range | Raw gas resistance and range bits
*/

/* 
   Raw data structure
*/
typedef struct {
    uint32_t raw_temp;
    uint32_t raw_press;
    uint16_t raw_hum;
    uint16_t raw_gas;
    uint8_t  gas_range;
} bme680_raw_data_t;

/* 
   Public API (clean version)
*/
esp_err_t bme680_init(void);
esp_err_t bme680_read(float *temperature, float *humidity,
                      float *pressure, float *gas_resistance);

esp_err_t bme680_read_calibration(void);
esp_err_t bme680_configure(void);
esp_err_t bme680_trigger_measurement(void);

bool bme680_is_measuring(void);
bool bme680_new_data_ready(void);

esp_err_t bme680_read_raw(bme680_raw_data_t *raw);

/* 
   Heater control helpers
*/
uint8_t calc_res_heat(uint16_t target_temp);
uint8_t calc_gas_wait(uint16_t dur_ms);
/* 
   References
*/
// Datasheet: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme680-ds001.pdf
// Another useful documentation on the BME680 for the "Zanduino": https://deepwiki.com/Zanduino/BME680