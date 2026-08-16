#include "ccs811.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "i2c_bus.h" // I2C init / bus defines

static const char *TAG = "CCS811";

esp_err_t ccs811_init(void)
{
    uint8_t status;

    // Read STATUS register
    ESP_ERROR_CHECK(i2c_master_write_read_device(
        I2C_MASTER_NUM,
        CCS811_ADDR,
        (uint8_t[]){CCS811_REG_STATUS},
        1,
        &status,
        1,
        1000 / portTICK_PERIOD_MS));

    // If in boot mode (FW_MODE == 0), start the application
    if ((status & CCS811_STATUS_FW_MODE) == 0) {
        uint8_t app_start_cmd = CCS811_CMD_APP_START;
        ESP_ERROR_CHECK(i2c_master_write_to_device(
            I2C_MASTER_NUM,
            CCS811_ADDR,
            &app_start_cmd,
            1,
            1000 / portTICK_PERIOD_MS));

        // small delay then poll until application mode is active
        vTaskDelay(pdMS_TO_TICKS(10));
        const TickType_t timeout = pdMS_TO_TICKS(1000);
        TickType_t start = xTaskGetTickCount();
        do {
            ESP_ERROR_CHECK(i2c_master_write_read_device(
                I2C_MASTER_NUM,
                CCS811_ADDR,
                (uint8_t[]){CCS811_REG_STATUS},
                1,
                &status,
                1,
                1000 / portTICK_PERIOD_MS));

            if ((xTaskGetTickCount() - start) > timeout) {
                ESP_LOGE(TAG, "Timeout waiting for application mode");
                return ESP_ERR_TIMEOUT;
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        } while ((status & CCS811_STATUS_FW_MODE) == 0);
    } else {
        ESP_LOGI(TAG, "CCS811 already in application mode");
    }

    // Set MEAS_MODE (register 0x01) to desired drive mode.
    // Example: 0x10 = Drive Mode 1 (1s), no interrupts
    uint8_t meas_mode = 0x10;
    uint8_t payload[2] = { CCS811_REG_MEAS_MODE, meas_mode };
    ESP_ERROR_CHECK(i2c_master_write_to_device(
        I2C_MASTER_NUM,
        CCS811_ADDR,
        payload,
        2,
        1000 / portTICK_PERIOD_MS));

    ESP_LOGI(TAG, "CCS811 initialized and measurement mode set");
    return ESP_OK;
}

esp_err_t ccs811_read(uint16_t *eco2, uint16_t *tvoc)
{
    uint8_t status;
    ESP_ERROR_CHECK(i2c_master_write_read_device(
        I2C_MASTER_NUM,
        CCS811_ADDR,
        (uint8_t[]){CCS811_REG_STATUS},
        1,
        &status,
        1,
        1000 / portTICK_PERIOD_MS));

    // Check DATA_READY
    if (!(status & CCS811_STATUS_DATA_READY)) {
        return ESP_ERR_INVALID_STATE; // data not ready
    }

    // Read algorithm results (ALG_RESULT_DATA)
    uint8_t buf[4];
    ESP_ERROR_CHECK(i2c_master_write_read_device(
        I2C_MASTER_NUM,
        CCS811_ADDR,
        (uint8_t[]){CCS811_REG_ALG_RESULT_DATA},
        1,
        buf,
        4,
        1000 / portTICK_PERIOD_MS));

    *eco2 = (uint16_t)((buf[0] << 8) | buf[1]);
    *tvoc = (uint16_t)((buf[2] << 8) | buf[3]);

    // Optional: check ERROR bit and log ERROR_ID if set
    if (status & CCS811_STATUS_ERROR) {
        uint8_t err_id = 0;
        if (i2c_master_write_read_device(
                I2C_MASTER_NUM,
                CCS811_ADDR,
                (uint8_t[]){CCS811_REG_ERROR_ID},
                1,
                &err_id,
                1,
                1000 / portTICK_PERIOD_MS) == ESP_OK) {
            ESP_LOGW(TAG, "CCS811 error, ERROR_ID=0x%02X", err_id);
        } else {
            ESP_LOGW(TAG, "CCS811 error and failed to read ERROR_ID");
        }
    }

    return ESP_OK;
}