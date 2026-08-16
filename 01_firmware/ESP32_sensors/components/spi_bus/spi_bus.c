#include "spi_bus.h"
#include "esp_log.h"
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <sys/stat.h>

static const char *TAG = "SD_BUS";

// SPI pin mapping for HiLetgo SD module
#define SD_MISO   19
#define SD_MOSI   23
#define SD_CLK    18
#define SD_CS     5

// SD card mount point
#define MOUNT_POINT "/sdcard"

static sdmmc_card_t *card = NULL;

esp_err_t spi_bus_init(void)
{
    ESP_LOGI(TAG, "Initializing SPI bus for SD card...");

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_MOSI,
        .miso_io_num = SD_MISO,
        .sclk_io_num = SD_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    // Use SPI2_HOST instead of SDSPI_HOST_ID
    esp_err_t err = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(err));
        return err;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_CS;
    slot_config.host_id = SPI2_HOST;

    esp_vfs_fat_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;

    err = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_cfg, &card);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SD card: %s", esp_err_to_name(err));
        spi_bus_free(SPI2_HOST);
        return err;
    }

    ESP_LOGI(TAG, "SD card mounted at %s", MOUNT_POINT);

    struct stat st;
    if (stat(MOUNT_POINT "/real_output", &st) != 0) {
        ESP_LOGI(TAG, "Creating /sdcard/real_output directory...");
        mkdir(MOUNT_POINT "/real_output", 0777);
    }

    return ESP_OK;
}

esp_err_t spi_bus_deinit(void)
{
    ESP_LOGI(TAG, "Unmounting SD card...");
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    spi_bus_free(SPI2_HOST);
    ESP_LOGI(TAG, "SPI bus deinitialized");
    return ESP_OK;
}

