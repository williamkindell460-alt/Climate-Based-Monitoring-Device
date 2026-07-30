#include "analyzer.h"
#include <stdio.h>
#include <stdlib.h>

int run_pipeline() {

    printf("====================== RUN PIPELINE ======================\n\n");
    printf("[INFO] Run Pipeline: pipeline is running.\n\n");

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

    printf("[OK] Pipeline: pipeline ran successfully.\n\n");

    /* ---------------- SUMMARY ---------------- */
    printf("==================== SUMMARY ====================\n");
    printf(sensor_ok ? "[OK] Sensor Reader\n" : "[ERROR] Sensor Reader\n");
    printf(plot_ok ? "[OK] Plot Data\n" : "[ERROR] Plot Data\n");
    printf(analyzer_ok ? "[OK] Analyzer\n" : "[ERROR] Analyzer\n");
    printf("[OK] Pipeline\n");

    return 0;
}