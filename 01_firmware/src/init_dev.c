#include "bme68x.h"
#include <stdio.h>
#include <stdlib.h>

int init_dev(void) {
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;

    return 0;
}

/* Must be initialized
uint8_t chip_id;
void *intf_ptr;
uint32_t variant_id;
SPI/I2C interface
enum bme68x_intf intf;
Memory page used
uint8_t mem_page;
Ambient temperature in Degree C
int8_t amb_temp;
Sensor calibration data
struct bme68x_calib_data calib;
Read function pointer
bme68x_read_fptr_t read;
Write function pointer
bme68x_write_fptr_t write;
Delay function pointer
bme68x_delay_us_fptr_t delay_us;
To store interface pointer error
BME68X_INTF_RET_TYPE intf_rslt;
Store the info messages
uint8_t info_msg;
*/

int8_t bme68x_init(struct bme68x_dev *dev);
int8_t bme68x_set_conf(struct bme68x_conf *conf, struct bme68x_dev *dev);
int8_t bme68x_get_conf(struct bme68x_conf *conf, struct bme68x_dev *dev);