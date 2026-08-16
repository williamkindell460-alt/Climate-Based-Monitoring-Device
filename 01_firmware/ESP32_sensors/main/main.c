#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "bme680.h"
#include "ccs811.h"
#include "i2c_bus.h"
#include <math.h>
#include "sd_logger.h"
#include "spi_bus.h"

static const char *TAG = "SENSORS";

void app_main(void)
{
    // 1. Initialize I2C bus
    esp_err_t err = i2c_master_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C init failed: %s", esp_err_to_name(err));
        return;
    }

    // 2. Initialize SPI bus for SD card
    err = spi_bus_init();   // ✅ renamed from sd_bus_init()
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SPI bus init failed: %s", esp_err_to_name(err));
        return;
    }

    // 3. Initialize SD logger (creates CSV + header if needed)
    err = sd_logger_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SD logger init failed: %s", esp_err_to_name(err));
        return;
    }

    // 4. Initialize sensors
    err = bme680_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "BME680 init failed: %s", esp_err_to_name(err));
        return;
    }

    err = ccs811_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "CCS811 init failed: %s", esp_err_to_name(err));
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // let sensors stabilize

    // 5. Main loop
    TickType_t last_wake = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(120000); // 2 minutes

    while (true) {
        float temperature = NAN, humidity = NAN, pressure = NAN, gas_resistance = NAN;
        uint16_t eco2 = 0, tvoc = 0;

        err = bme680_read(&temperature, &humidity, &pressure, &gas_resistance);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "BME680 read failed: %s", esp_err_to_name(err));
        }

        err = ccs811_read(&eco2, &tvoc);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "CCS811 read failed: %s", esp_err_to_name(err));
        }

        ESP_LOGI(TAG, "T=%.2f°C H=%.2f%% P=%.2f hPa Gas=%.2fΩ eCO2=%u ppm TVOC=%u ppb",
                 isnan(temperature) ? 0.0f : temperature,
                 isnan(humidity) ? 0.0f : humidity,
                 isnan(pressure) ? 0.0f : pressure,
                 isnan(gas_resistance) ? 0.0f : gas_resistance,
                 eco2, tvoc);

        // 6. Append to CSV
        sd_logger_append(temperature, humidity, pressure, gas_resistance, eco2, tvoc);

        vTaskDelayUntil(&last_wake, period);
    }
}