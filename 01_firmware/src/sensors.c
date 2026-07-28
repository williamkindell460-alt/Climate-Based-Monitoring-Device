#include "bme68x.h"
#include <stdio.h>
#include <stdint.h>


int8_t bme68x_get_data(uint8_t op_mode, struct bme68x_data *data, uint8_t *n_data, struct bme68x_dev *dev);