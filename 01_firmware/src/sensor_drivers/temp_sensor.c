#include "bme68x.h"

float read_temperature(struct bme68x_dev *dev)
{
    struct bme68x_data data;
    uint8_t n_fields;

    bme68x_set_mode(BME68X_FORCED_MODE, dev);

    dev->delay_us(50000, dev->intf_ptr);

    // Read data
    bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, dev);

    return data.temperature;
}