#include "bme68x.h"
#include "bme68x_defs.h"
#include <time.h>

int read_bme680() {
    float *data = read_sensors();
    if (data != NULL) {
        printf("Temperature: %f, Humidity: %f, Pressure: %f\n", data[0], data[1], data[2]);
        free(data);
    }
    return 0;
}

float *read_bme680_sensors() {
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;
    uint32_t del_period;
    uint32_t time_ms = 0;
    uint8_t n_fields;
    uint16_t sample_count = 1;
    /* Interface preference is updated as a parameter
     * For I2C : BME68X_I2C_INTF
     * For SPI : BME68X_SPI_INTF
     */
    rslt = bme68x_interface_init(&bme, BME68X_I2C_INTF);
    if (rslt != BME68X_OK) {
        printf("rslt == BME68X_OK: False");
        return;
    }

    float *sensor_values = malloc(5 * sizeof(float));
    if (sensor_values == NULL) {
        printf("Memory allocation for sensor_values in read_bme680.c has failed.\n");
        return;
    }
    sensor_values[0] = read_temp(data);
    sensor_values[1] = read_humidity(data);
    sensor_values[2] = read_pressure(data);
    return sensor_values;
}

float read_temp(struct bme68x_data data) {
    return data.temperature;
}
float read_humidity(struct bme68x_data data) {
    return data.humidity;
}
float read_pressure(struct bme68x_data data) {
    return data.pressure;
}
float read_gas_resistance(struct bme68x_data data) {
    return data.gas_resistance; // For possible future reference.
}