#include "analyzer.h"
#include "bme68x.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;

    printf("====================== MAIN ======================\n\n");
    printf("[INFO] Main: main is running.\n\n");

    int sensor_ok = 0;
    int plot_ok = 0;
    int analyzer_ok = 0;

    /* ---------------- SENSOR READER ---------------- */
    printf("============== Sensor Reader ==============\n");
    int ret = system("\"" PYTHON_EXECUTABLE "\" ../../03_data/src/sensor_reader.py");

    if (ret != 0) {
        printf("[ERROR] Sensor Reader failed.\n\n");
        printf("==================== SUMMARY ====================\n");
        printf("[ERROR] Sensor Reader\n");
        printf("[SKIP] Plot Data\n");
        printf("[SKIP] Analyzer\n");
        printf("[SKIP] Main\n");
        return 1;
    }

    sensor_ok = 1;

    /* ---------------- ANALYZER ---------------- */
    analyzer("../../03_data/output_data/output.bin",
             "../../03_data/output_data/analysis_output.txt");
    analyzer_ok = 1;

    /* ---------------- PLOT DATA ---------------- */
    ret = system("\"" PYTHON_EXECUTABLE "\" ../../03_data/src/plot_data.py");
    plot_ok = (ret == 0);

    printf("[OK] Main: main ran successfully.\n\n");

    /* ---------------- SUMMARY ---------------- */
    printf("==================== SUMMARY ====================\n");
    printf(sensor_ok ? "[OK] Sensor Reader\n" : "[ERROR] Sensor Reader\n");
    printf(plot_ok ? "[OK] Plot Data\n" : "[ERROR] Plot Data\n");
    printf(analyzer_ok ? "[OK] Analyzer\n" : "[ERROR] Analyzer\n");
    printf("[OK] Main\n");

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