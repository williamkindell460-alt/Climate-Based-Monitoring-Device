#include "i2c_bus.h"
#include "esp_log.h"
#include "driver/i2c.h"

static const char *I2C_TAG = "I2C_BUS";

esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        // .clk_flags = 0, // set if needed for the IDF version
    };

    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(I2C_TAG, "i2c_param_config failed: %s", esp_err_to_name(err));
        return err;
    }

    // If driver might already be installed, optionally delete it first:
    // i2c_driver_delete(I2C_MASTER_NUM);

    err = i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                             0,
                             0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(I2C_TAG, "i2c_driver_install failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(I2C_TAG, "I2C master initialized (SDA=%d SCL=%d @%dHz)",
             I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, I2C_MASTER_FREQ_HZ);

    return ESP_OK;
}

// make deinit return esp_err_t and be safe if driver not installed
esp_err_t i2c_master_deinit(void)
{
    esp_err_t err = i2c_driver_delete(I2C_MASTER_NUM);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(I2C_TAG, "i2c_driver_delete failed: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(I2C_TAG, "I2C driver deleted");
    return ESP_OK;
}
