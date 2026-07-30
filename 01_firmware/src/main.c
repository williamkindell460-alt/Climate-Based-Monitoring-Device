#include "bme68x.h"
#include "analyzer.h"
#include <stdlib.h>

int main(void)
{
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;

    system("\"C:/Users/jrkin/.local/bin/python3.14.exe\" ../../03_data/src/sensor_reader.py");

    analyzer("../../03_data/output_data/output.bin",
             "../../03_data/output_data/analysis_output.txt");

    return 0;
}

int8_t bme68x_init(struct bme68x_dev *dev);
int8_t bme68x_set_conf(struct bme68x_conf *conf, struct bme68x_dev *dev);
int8_t bme68x_get_conf(struct bme68x_conf *conf, struct bme68x_dev *dev);
