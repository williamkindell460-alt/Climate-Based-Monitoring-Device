idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES bme680 ccs811 i2c_bus sd_logger spi_bus
)