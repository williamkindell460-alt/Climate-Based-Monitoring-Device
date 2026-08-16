idf_component_register(
    SRCS "bme680.c"
    INCLUDE_DIRS "."
    REQUIRES driver i2c_bus esp_timer
)