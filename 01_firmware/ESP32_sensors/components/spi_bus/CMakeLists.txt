idf_component_register(
    SRCS "spi_bus.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_spi esp_driver_sdspi esp_driver_sdmmc vfs fatfs
)