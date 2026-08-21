#include "sd_logger.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <stdio.h>
#include <sys/stat.h>

#define LOG_TAG "SD_LOGGER"
#define LOG_PATH "/sdcard/real_output/data.csv"

float compute_breathability(float gas_resistance, float eCO2,
                            float humidity, float temperature, float pressure)
{
    // Normalize each factor
    float voc_score = fminf(gas_resistance / 50000.0f, 1.0f);   // higher = cleaner
    float co2_score = 1.0f - fminf(eCO2 / 2000.0f, 1.0f);       // lower = fresher
    float hum_score = 1.0f - fabsf(humidity - 50.0f) / 50.0f;   // ideal ~50% RH
    float temp_score = 1.0f - fabsf(temperature - 20.0f) / 20.0f; // ideal ~20°C
    float press_score = (pressure > 1010.0f) ? 0.8f : 1.0f;     // high pressure traps pollutants

    // Weighted average
    float breathability = (0.35f * voc_score) + (0.35f * co2_score) +
                          (0.15f * hum_score) + (0.10f * temp_score) +
                          (0.05f * press_score);

    return breathability * 100.0f; // Scale to 0–100
}

// 80–100	Excellent — clean, fresh outdoor air
// 60–80	Good — typical outdoor conditions
// 40–60	Moderate — noticeable pollution or humidity
// 20–40	Poor — stagnant or polluted air
// 0–20	Very poor — avoid prolonged exposure

esp_err_t sd_logger_init(void)
{
    ESP_LOGI(LOG_TAG, "Initializing SD logger...");

    // Ensure directory exists
    struct stat st;
    if (stat("/sdcard/real_output", &st) != 0) {
        ESP_LOGI(LOG_TAG, "Creating /sdcard/real_output directory...");
        mkdir("/sdcard/real_output", 0777);
    }

    // Check if CSV exists
    FILE *f = fopen(LOG_PATH, "r");
    if (f) {
        ESP_LOGI(LOG_TAG, "CSV file already exists, no header written.");
        fclose(f);
        return ESP_OK;
    }

    // Create CSV + write header
    f = fopen(LOG_PATH, "w");
    if (!f) {
        ESP_LOGE(LOG_TAG, "Failed to create CSV file!");
        return ESP_FAIL;
    }

    fprintf(f,
        "timestamp,temperature_c,humidity_pct,pressure_hpa,air_quality_index,co2_ppm,voc_ppb\n"
    );
    fclose(f);

    ESP_LOGI(LOG_TAG, "CSV header written.");
    return ESP_OK;
}

esp_err_t sd_logger_append(
    float temperature, 
    float humidity,
    float pressure,
    float gas_resistance,
    uint16_t eco2,
    uint16_t tvoc)
{
    FILE *f = fopen(LOG_PATH, "a");
    if (!f) {
        ESP_LOGE(LOG_TAG, "Failed to open CSV for appending!");
        return ESP_FAIL;
    }

    // Get timestamp in seconds since boot
    int64_t now_us = esp_timer_get_time();
    double timestamp = (double)now_us / 1e6;

    int air_breathability = compute_breathability(gas_resistance, eco2, humidity, temperature, pressure);

    fprintf(f,
        "%.0f,%.2f,%.2f,%.2f,%d,%u,%u\n",
        timestamp,
        temperature,
        humidity,
        pressure,
        air_breathability,
        eco2,
        tvoc
    );

    fclose(f);
    ESP_LOGI(LOG_TAG, "Logged row: T=%.2f H=%.2f P=%.2f AQI=%d CO2=%u TVOC=%u",
             temperature, humidity, pressure, aqi, eco2, tvoc);

    return ESP_OK;
}
