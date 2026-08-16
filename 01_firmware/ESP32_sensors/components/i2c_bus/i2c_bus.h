#pragma once // Prevents double inclusion
#include "driver/i2c.h" // This is the ESP-IDF's I2C driver, which gives access to reading, writing, configuring, installing, etc.

#define I2C_MASTER_SCL_IO           22 // SCL (Clock line) goes to GPIO 22/pin OI22 on ESP-WROOM-32
#define I2C_MASTER_SDA_IO           21 // SDA (data line) does to GPIO 21/pin OI21 on ESP-WROOM-32
#define I2C_MASTER_NUM              I2C_NUM_0 // The ESP32 has two hardware controllers: I2C_NUM_0 and I2C_NUM_1. We are using Controller 0, which is default and typically recommended
#define I2C_MASTER_FREQ_HZ          100000 // This defines the bus speed as 100kHz is standard mode, 400kHz is Fast mode, 1MHz is Fast+ mode, and 3.4 MHz is High-Speed mode. 

esp_err_t i2c_master_init(void); // This simply declares the function: i2c_master_init

// More Documentation on I2C: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
// More Documentation on ESP-WROOM-32: https://documentation.espressif.com/esp32-wroom-32_datasheet_en.html