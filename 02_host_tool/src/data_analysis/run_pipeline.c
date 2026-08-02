#include "analyzer.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>

#define MODE_DEFAULT 0   // stats + anomalies
#define MODE_ANOMALIES 1 // anomalies only
#define MODE_STATS 2     // stats only

int pipeline_argc = 0;
char **pipeline_argv = NULL;

int run_pipeline(int mode) {

    printf(COL_CYAN "====================== RUN PIPELINE ======================\n\n" COL_RESET);
    printf(COL_YELLOW "[INFO] Run Pipeline: pipeline is running.\n\n" COL_RESET);

    int sensor_ok = 0;
    int plot_ok = 0;
    int analyzer_ok = 0;

    const char *test_output_dir = getenv("CBMD_TEST_OUTPUT_DIR");
    const char *test_data_dir = getenv("CBMD_TEST_DATA_DIR");

    // Make test_data_dir accessible for python
    if (test_data_dir) {
#ifdef _WIN32
        char envbuf[512];
        snprintf(envbuf, sizeof(envbuf), "CBMD_TEST_DATA_DIR=%s", test_data_dir);
        _putenv(envbuf);
#else
        setenv("CBMD_TEST_DATA_DIR", test_data_dir, 1);
#endif
    }
    /* ---------------- SENSOR READER ---------------- */
    printf(COL_CYAN "============== Sensor Reader ==============\n" COL_RESET);

    char cmd_reader[512];
    snprintf(cmd_reader, sizeof(cmd_reader),
             "%s %s/02_host_tool/src/data_reading_plotting/sensor_reader.py",
             PYTHON_EXECUTABLE,
             SOURCE_DIR);

    int ret = system(cmd_reader);

    if (ret != 0) {
        printf(COL_RED "[ERROR] Sensor Reader failed.\n\n" COL_RESET);
        printf(COL_CYAN "==================== SUMMARY ====================\n" COL_RESET);
        printf(COL_RED "[ERROR] Sensor Reader\n" COL_RESET);
        printf(COL_YELLOW "[SKIP] Plot Data\n" COL_RESET);
        printf(COL_YELLOW "[SKIP] Analyzer\n" COL_RESET);
        printf(COL_YELLOW "[SKIP] Main\n" COL_RESET);
        return 1;
    }

    sensor_ok = 1;

    /* ---------------- ANALYZER ---------------- */
    const char *default_output =
        SOURCE_DIR "/03_data/output_data/analysis_output.txt";

    const char *output_path =
        test_output_dir
            ? SOURCE_DIR "/02_host_tool/tests/output_data/analysis_output.txt"
            : default_output;

    const char *an_args[16];
    int an_argc = 0;

    an_args[an_argc++] = "analyzer";
    an_args[an_argc++] = test_data_dir
                             ? SOURCE_DIR "/02_host_tool/tests/output_data/output.bin"
                             : SOURCE_DIR "/03_data/output_data/output.bin";

    an_args[an_argc++] = output_path;

    for (int i = 3; i < pipeline_argc; i++) {
        an_args[an_argc++] = pipeline_argv[i];
    }

    if (mode == MODE_STATS)
        an_args[an_argc++] = "--stats";
    else if (mode == MODE_ANOMALIES)
        an_args[an_argc++] = "--anomalies";

    analyzer(an_argc, an_args);
    analyzer_ok = 1;

    /* ---------------- PLOT DATA ---------------- */
    char cmd_plot[512];
    snprintf(cmd_plot, sizeof(cmd_plot),
             "%s %s/02_host_tool/src/data_reading_plotting/plot_data.py",
             PYTHON_EXECUTABLE,
             SOURCE_DIR);
    ret = system(cmd_plot);
    plot_ok = (ret == 0);

    printf(COL_GREEN "[OK] Pipeline: pipeline ran successfully.\n\n" COL_RESET);

    /* ---------------- SUMMARY ---------------- */
    printf(COL_CYAN "==================== SUMMARY ====================\n" COL_RESET);
    printf(sensor_ok ? COL_GREEN "[OK] Sensor Reader\n" COL_RESET
                     : COL_RED "[ERROR] Sensor Reader\n" COL_RESET);
    printf(plot_ok ? COL_GREEN "[OK] Plot Data\n" COL_RESET
                   : COL_RED "[ERROR] Plot Data\n" COL_RESET);
    printf(analyzer_ok ? COL_GREEN "[OK] Analyzer\n" COL_RESET
                       : COL_RED "[ERROR] Analyzer\n" COL_RESET);
    printf(COL_GREEN "[OK] Pipeline\n" COL_RESET);

    return 0;
}