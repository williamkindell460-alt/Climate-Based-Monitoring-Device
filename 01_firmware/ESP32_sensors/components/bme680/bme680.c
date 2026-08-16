#include "bme680.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "i2c_bus.h"
#include <stdbool.h>

#define OSRS_8X_FOR_TEMPERATURE_PRESSURE_FORCED 0x91

static const char *TAG = "BME680";
static int32_t t_fine;

/* 
   Calibration data container
*/
typedef struct {
    uint16_t par_t1;
    int16_t  par_t2;
    int8_t   par_t3;

    uint16_t par_p1;
    int16_t  par_p2;
    int8_t   par_p3;
    int16_t  par_p4;
    int16_t  par_p5;
    int8_t   par_p6;
    int8_t   par_p7;
    int16_t  par_p8;
    int16_t  par_p9;
    uint8_t  par_p10;

    uint16_t par_h1;
    uint16_t par_h2;
    int8_t   par_h3;
    int8_t   par_h4;
    int8_t   par_h5;
    uint8_t  par_h6;
    int8_t   par_h7;

    int8_t   par_g1;
    int16_t  par_g2;
    int8_t   par_g3;

    uint8_t  res_heat_range;
    int8_t   res_heat_val;
    int8_t   range_sw_err;
} bme680_calib_data_t;

static bme680_calib_data_t calib;

/* 
   Initialization
*/
esp_err_t bme680_init(void)
{
    uint8_t id;
    esp_err_t err = i2c_master_write_read_device(
        I2C_MASTER_NUM, BME680_ADDR,
        (uint8_t[]){BME680_CHIP_ID}, 1, &id, 1,
        1000 / portTICK_PERIOD_MS);

    if (err != ESP_OK) return err;
    if (id != 0x61) {
        ESP_LOGE(TAG, "BME680 not found (ID=0x%02X)", id);
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(bme680_read_calibration());
    ESP_ERROR_CHECK(bme680_configure());
    ESP_LOGI(TAG, "BME680 detected and calibration loaded");
    return ESP_OK;
}

/* 
   Read calibration coefficients
*/
esp_err_t bme680_read_calibration(void)
{
    uint8_t buf1[25];
    uint8_t buf2[16];

    ESP_ERROR_CHECK(i2c_master_write_read_device(
        I2C_MASTER_NUM, BME680_ADDR,
        (uint8_t[]){STARTING_REGISTER_FOR_TEMP_PRESSURE}, 1,
        buf1, 25, 1000 / portTICK_PERIOD_MS));

    ESP_ERROR_CHECK(i2c_master_write_read_device(
        I2C_MASTER_NUM, BME680_ADDR,
        (uint8_t[]){STARTING_REGISTER_FOR_HUMIDITY_GAS}, 1,
        buf2, 16, 1000 / portTICK_PERIOD_MS));

    calib.par_t2 = (int16_t)((buf1[2] << 8) | buf1[1]);
    calib.par_t3 = (int8_t)buf1[3];
    calib.par_p1  = (uint16_t)((buf1[5] << 8) | buf1[4]);
    calib.par_p2  = (int16_t)((buf1[7] << 8) | buf1[6]);
    calib.par_p3  = (int8_t)buf1[8];
    calib.par_p4  = (int16_t)((buf1[11] << 8) | buf1[10]);
    calib.par_p5  = (int16_t)((buf1[13] << 8) | buf1[12]);
    calib.par_p6  = (int8_t)buf1[15];
    calib.par_p7  = (int8_t)buf1[14];
    calib.par_p8  = (int16_t)((buf1[19] << 8) | buf1[18]);
    calib.par_p9  = (int16_t)((buf1[21] << 8) | buf1[20]);
    calib.par_p10 = buf1[22];

    calib.par_h1 = (uint16_t)((buf2[2] << 4) | (buf2[1] & 0x0F));
    calib.par_h2 = (uint16_t)((buf2[1] >> 4) | (buf2[0] << 4));
    calib.par_h3 = (int8_t)buf2[3];
    calib.par_h4 = (int8_t)buf2[4];
    calib.par_h5 = (int8_t)buf2[5];
    calib.par_h6 = buf2[6];
    calib.par_h7 = (int8_t)buf2[7];

    calib.par_t1 = (uint16_t)((buf2[9] << 8) | buf2[8]);
    calib.par_g1 = (int8_t)buf2[12];
    calib.par_g2 = (int16_t)((buf2[11] << 8) | buf2[10]);
    calib.par_g3 = (int8_t)buf2[13];

    calib.res_heat_range = (buf2[14] & 0x30) >> 4;
    calib.res_heat_val   = (int8_t)buf2[15];
    calib.range_sw_err   = (int8_t)(buf2[15] & 0x0F);

    return ESP_OK;
}
/* 
   I2C helpers
*/
esp_err_t bme680_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    return i2c_master_write_to_device(I2C_MASTER_NUM, BME680_ADDR, data, 2, 1000 / portTICK_PERIOD_MS);
}

esp_err_t bme680_read_reg(uint8_t reg, uint8_t *value)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, BME680_ADDR, &reg, 1, value, 1, 1000 / portTICK_PERIOD_MS);
}

esp_err_t bme680_read_block(uint8_t reg, uint8_t *buf, uint8_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, BME680_ADDR, &reg, 1, buf, len, 1000 / portTICK_PERIOD_MS);
}
/* 
   Configuration
*/
esp_err_t bme680_configure(void)
{
    ESP_ERROR_CHECK(bme680_write_reg(BME68X_REG_CTRL_HUM, 0x04));
    ESP_ERROR_CHECK(bme680_write_reg(BME68X_REG_CTRL_MEAS, OSRS_8X_FOR_TEMPERATURE_PRESSURE_FORCED));
    ESP_ERROR_CHECK(bme680_write_reg(BME68X_REG_CONFIG, 0x0C));

    uint8_t heat = calc_res_heat(300);
    ESP_ERROR_CHECK(bme680_write_reg(BME68X_REG_RES_HEAT_0, heat));

    uint8_t wait = calc_gas_wait(150);
    ESP_ERROR_CHECK(bme680_write_reg(BME68X_REG_GAS_WAIT_0, wait));

    ESP_ERROR_CHECK(bme680_write_reg(BME68X_REG_CTRL_GAS_1, 0x10));
    ESP_LOGI(TAG, "BME680 configuration applied");
    return ESP_OK;
}
/* 
   Measurement control
*/
bool bme680_is_measuring(void)
{
    uint8_t status;
    bme680_read_reg(BME680_REG_MEAS_STATUS, &status);
    return (status & 0x20);
}

bool bme680_new_data_ready(void)
{
    uint8_t status;
    bme680_read_reg(BME680_REG_STATUS, &status);
    return (status & 0x80);
}

esp_err_t bme680_trigger_measurement(void)
{
    return bme680_write_reg(BME68X_REG_CTRL_MEAS, OSRS_8X_FOR_TEMPERATURE_PRESSURE_FORCED);
}

/* 
   Raw data readout
*/
esp_err_t bme680_read_raw(bme680_raw_data_t *raw)
{
    uint8_t buf[15];
    ESP_ERROR_CHECK(bme680_read_block(STARTING_REGISTER_FOR_RAW_VALUES, buf, 15));

    raw->raw_press = (uint32_t)((buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4));
    raw->raw_temp  = (uint32_t)((buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4));
    raw->raw_hum   = (uint16_t)((buf[6] << 8) | buf[7]);
    raw->raw_gas   = (uint16_t)((buf[13] << 2) | (buf[14] >> 6));
    raw->gas_range = buf[14] & 0x0F;

    return ESP_OK;
}

/* 
   Compensation formulas (Bosch datasheet 3.3)
*/
float bme680_compensate_temperature(uint32_t adc_temp)
{
    int32_t var1 = ((((adc_temp >> 3) - ((int32_t)calib.par_t1 << 1))) *
                    ((int32_t)calib.par_t2)) >> 11;

    int32_t var2 = (((((adc_temp >> 4) - ((int32_t)calib.par_t1)) *
                      ((adc_temp >> 4) - ((int32_t)calib.par_t1))) >> 12) *
                    ((int32_t)calib.par_t3)) >> 14;

    t_fine = var1 + var2;
    float temp = ((t_fine * 5 + 128) >> 8) / 100.0f;
    return temp;
}

float bme680_compensate_pressure(uint32_t adc_press)
{
    int64_t var1 = ((int64_t)t_fine >> 1) - 64000;
    int64_t var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * calib.par_p6;
    var2 += (var1 * calib.par_p5) << 1;
    var2 = (var2 >> 2) + ((int64_t)calib.par_p4 << 16);

    int64_t var3 = (((calib.par_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) +
                    ((calib.par_p2 * var1) >> 1)) >> 18;

    var3 = ((32768 + var3) * calib.par_p1) >> 15;
    if (var3 == 0) return 0; // avoid divide-by-zero

    int64_t pressure = (((1048576 - adc_press) - (var2 >> 12)) * 3125);
    if (pressure >= (1 << 31))
        pressure = (pressure / var3) << 1;
    else
        pressure = (pressure << 1) / var3;

    var1 = (calib.par_p9 * ((pressure >> 3) * (pressure >> 3)) >> 13) >> 12;
    var2 = ((pressure >> 2) * calib.par_p8) >> 13;
    pressure = pressure + ((var1 + var2 + calib.par_p7) >> 4);

    return pressure / 100.0f; // hPa
}

float bme680_compensate_humidity(uint16_t adc_hum)
{
    int32_t temp_scaled = (t_fine * 5 + 128) >> 8;

    int32_t var1 = adc_hum - ((int32_t)calib.par_h1 << 4) -
                   (((temp_scaled * (int32_t)calib.par_h3) / 100) >> 1);

    int32_t var2 = ((int32_t)calib.par_h2 *
                ((((temp_scaled * (int32_t)calib.par_h4) / 100) +
                    (((temp_scaled * (int32_t)calib.par_h5) / 100) >> 6) +
                    (1 << 14))) >> 10);

    int32_t hum = var1 * var2;
    float h = hum / 1024.0f;
    if (h < 0.0f) h = 0.0f;
    if (h > 100.0f) h = 100.0f;
    return h;
}

/* 
   Gas resistance compensation
*/
static const uint32_t gas_range_table[16] = {
    2147483647, 2147483647, 2147483647, 2147483647,
    2147483647, 2130303777, 2147483647, 2130303777,
    2147483647, 2147483647, 2143188679, 2136746228,
    2147483647, 2126008810, 2147483647, 2147483647
};

float bme680_compensate_gas(uint16_t adc_gas, uint8_t gas_range)
{
    float var1 = (float)(1340.0f + (5.0f * calib.range_sw_err));
    float var2 = gas_range_table[gas_range] / 65536.0f;
    float gas_res = var1 * var2 * adc_gas;
    return gas_res;
}

/* 
   Heater control helpers
*/
uint8_t calc_res_heat(uint16_t target_temp)
{
    float var1 = ((float)calib.res_heat_range / 16.0f) + 1.0f;
    float var2 = (float)calib.res_heat_val * 0.002f;
    float var3 = (float)target_temp / (var1 * (1.0f + var2));
    return (uint8_t)(var3 + 0.5f);
}

uint8_t calc_gas_wait(uint16_t dur_ms)
{
    if (dur_ms < 0x3F)
        return dur_ms;

    uint8_t factor = 0;
    while (dur_ms > 0x3F) {
        dur_ms >>= 2;
        factor++;
    }
    return (uint8_t)(dur_ms + (factor << 6));
}

/* 
   High-level read wrapper
*/
esp_err_t bme680_read(float *temperature, float *humidity,
                      float *pressure, float *gas_resistance)
{
    ESP_ERROR_CHECK(bme680_trigger_measurement());

    const TickType_t timeout = pdMS_TO_TICKS(1000);
    TickType_t start = xTaskGetTickCount();

    while (bme680_is_measuring()) {
        if ((xTaskGetTickCount() - start) > timeout) return ESP_ERR_TIMEOUT;
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }

    start = xTaskGetTickCount();
    while (!bme680_new_data_ready()) {
        if ((xTaskGetTickCount() - start) > timeout) return ESP_ERR_TIMEOUT;
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }

    bme680_raw_data_t raw;
    esp_err_t err = bme680_read_raw(&raw);
    if (err != ESP_OK) return err;

    *temperature    = bme680_compensate_temperature(raw.raw_temp);
    *pressure       = bme680_compensate_pressure(raw.raw_press);
    *humidity       = bme680_compensate_humidity(raw.raw_hum);
    *gas_resistance = bme680_compensate_gas(raw.raw_gas, raw.gas_range);

    ESP_LOGD(TAG, "Raw T=%u P=%u H=%u G=%u range=%u",
             raw.raw_temp, raw.raw_press, raw.raw_hum, raw.raw_gas, raw.gas_range);

    return ESP_OK;
}