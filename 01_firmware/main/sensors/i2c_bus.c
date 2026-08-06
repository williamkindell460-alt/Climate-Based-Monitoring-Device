#include "i2c_bus.h"
#include "driver/i2c_master.h"

#define I2C_PORT 0
#define SDA_PIN 21
#define SCL_PIN 22

static i2c_master_bus_handle_t bus_handle;

void i2c_bus_init(void) {
    i2c_master_bus_config_t bus_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &bus_handle));
}
