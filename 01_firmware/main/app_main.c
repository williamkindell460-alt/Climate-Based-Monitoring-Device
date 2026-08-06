#include "bme680.h"
#include "ccs811.h"
#include "i2c_bus.h"
#include "logger.h"
#include "sleep_manager.h"

void app_main(void) {
    logger_init();
    log_info("Starting climate monitor...");

    i2c_bus_init();

    bme680_init();
    ccs811_init();

    float temp = bme680_read_temperature();
    float hum = bme680_read_humidity();
    float pres = bme680_read_pressure();
    uint16_t co2 = ccs811_read_co2();

    log_info("Temp: %.2f C, Hum: %.2f %%, Pressure: %.2f hPa, CO2: %u ppm",
             temp, hum, pres, co2);

    enter_deep_sleep(2 * 60 * 1000000ULL);
}
