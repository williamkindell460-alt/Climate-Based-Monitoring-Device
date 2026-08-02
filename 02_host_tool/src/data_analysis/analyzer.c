#include "analyzer.h"
#include "../../01_firmware/include/logging.h"
#include "anomaly_detection.h"
#include "color.h"
#include "trend_detection.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Used to benchmark
static double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}
/*
   CLI Flag Parser
*/
int has_flag(int argc, const char *argv[], const char *flag) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0)
            return 1;
    }
    return 0;
}

/*
   Selector Functions
 */
double select_temp(SensorRow r) { return r.temp; }
double select_humidity(SensorRow r) { return r.humidity; }
double select_pressure(SensorRow r) { return r.pressure; }
double select_air_qual(SensorRow r) { return r.air_qual; }
double select_co2(SensorRow r) { return r.co2; }
double select_voc(SensorRow r) { return r.voc; }

static const char *SPARK_BLOCKS[] = {
    "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"};
static const int SPARK_LEVELS = 8;

/* Generate sparkline for any metric */
void print_sparkline(FILE *out, SensorRow *rows, int count,
                     double (*selector)(SensorRow)) {

    double min = selector(rows[0]);
    double max = selector(rows[0]);

    for (int i = 1; i < count; i++) {
        double v = selector(rows[i]);
        if (v < min) min = v;
        if (v > max) max = v;
    }

    double range = max - min;
    if (range == 0) range = 1.0;

    fprintf(out, "Sparkline: ");

    for (int i = 0; i < count; i++) {
        double v = selector(rows[i]);
        double norm = (v - min) / range;
        int idx = (int)(norm * (SPARK_LEVELS - 1));
        if (idx < 0) idx = 0;
        if (idx >= SPARK_LEVELS) idx = SPARK_LEVELS - 1;
        fprintf(out, "%s", SPARK_BLOCKS[idx]);
    }

    fprintf(out, "\n\n");
}

/*
   Count rows in binary file
*/
int count_rows(FILE *fp) {
    printf(COL_CYAN "---------------------- count_rows ----------------------\n\n");
    printf(COL_YELLOW "[INFO] Analyzer: count_rows is running.\n\n" COL_RESET);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size % (7 * sizeof(double)) != 0) {
        printf(COL_RED "[ERROR] File size is not divisible by expected row size. Possible corruption.\n");
    }

    printf(COL_GREEN "[OK] Analyzer: count_rows function ran successfully\n\n");
    return size / (7 * sizeof(double));
}

/*
   Load all sensor rows from file
 */
int load_rows(FILE *fp, SensorRow *rows, int max_rows) {
    printf(COL_CYAN "---------------------- load_rows ----------------------\n\n");
    printf(COL_YELLOW "[INFO] Analyzer: load_rows is running.\n\n");

    double values[7];
    int count = 0;
    double last_time = -1.0;

    while (fread(values, sizeof(double), 7, fp) == 7 && count < max_rows) {

        int invalid = 0;
        for (int i = 0; i < 7; i++) {
            if (isnan(values[i]) || isinf(values[i])) {
                printf(COL_LIGHT_RED "[WARN] Invalid numeric value (NaN/Inf) detected in row %d\n", count);
                invalid = 1;
            }
        }

        if (values[0] < last_time) {
            printf(COL_LIGHT_RED "[WARN] Timestamp anomaly: non-monotonic time at row %d\n", count);
        }
        last_time = values[0];

        if (values[1] < -50 || values[1] > 150) printf(COL_LIGHT_RED "[WARN] Temperature out of realistic range at row %d\n", count);
        if (values[2] < 0 || values[2] > 100) printf(COL_LIGHT_RED "[WARN] Humidity out of range at row %d\n", count);
        if (values[3] < 300 || values[3] > 1100) printf(COL_LIGHT_RED "[WARN] Pressure out of range at row %d\n", count);
        if (values[4] < 0) printf(COL_LIGHT_RED "[WARN] AQI negative at row %d\n", count);
        if (values[5] < 0) printf(COL_LIGHT_RED "[WARN] CO2 negative at row %d\n", count);
        if (values[6] < 0) printf(COL_LIGHT_RED "[WARN] VOC negative at row %d\n", count);

        rows[count].time = values[0];
        rows[count].temp = values[1];
        rows[count].humidity = values[2];
        rows[count].pressure = values[3];
        rows[count].air_qual = values[4];
        rows[count].co2 = values[5];
        rows[count].voc = values[6];

        count++;
    }

    long pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);

    if (pos < end) {
        printf(COL_LIGHT_RED "[WARN] File contains more data than expected.\n");
    }

    printf(COL_GREEN "[OK] Analyzer: load_rows function ran successfully\n\n");
    return count;
}

/*
   Print summary block to file
*/
void print_summary(FILE *out,
                   const char *filename,
                   int count,
                   SensorRow *rows,
                   double temp_min, double temp_max, double temp_avg,
                   double hum_min, double hum_max, double hum_avg,
                   double press_min, double press_max, double press_avg,
                   double aqi_min, double aqi_max, double aqi_avg,
                   double co2_min, double co2_max, double co2_avg,
                   double voc_min, double voc_max, double voc_avg) {
    printf(COL_CYAN "---------------------- print_summary ----------------------\n\n");
    printf(COL_YELLOW "[INFO] Analyzer: print_summary is running.\n\n");

    fprintf(out,
            "Climate-Based Monitoring Device — Host Tool Analysis\n"
            "====================================================\n\n"
            "Input file: %s\n"
            "Samples processed: %d\n"
            "Time range: %.2f minutes\n"
            "----------------------------------------------------\n"
            "Summary Statistics\n"
            "----------------------------------------------------\n\n",
            filename,
            count,
            rows[count - 1].time);

    fprintf(out,
            "Temperature (C)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            temp_min, temp_max, temp_avg);
    print_sparkline(out, rows, count, select_temp);

    fprintf(out,
            "Humidity (%%)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            hum_min, hum_max, hum_avg);
    print_sparkline(out, rows, count, select_humidity);

    fprintf(out,
            "Pressure (hPa)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            press_min, press_max, press_avg);
    print_sparkline(out, rows, count, select_pressure);

    fprintf(out,
            "Air Quality Index (AQI)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            aqi_min, aqi_max, aqi_avg);
    print_sparkline(out, rows, count, select_air_qual);

    fprintf(out,
            "CO2 (ppm)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            co2_min, co2_max, co2_avg);
    print_sparkline(out, rows, count, select_co2);

    fprintf(out,
            "VOC (ppb)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            voc_min, voc_max, voc_avg);
    print_sparkline(out, rows, count, select_voc);

    printf(COL_GREEN "[OK] Analyzer: print_summary ran successfully\n\n");
}

/*
   Analyzer Main Function
*/
int analyzer(int argc, const char *argv[]) {
    printf(COL_CYAN "-------------------- ANALYZER --------------------\n\n");
    printf(COL_YELLOW "[INFO] Analyzer: analyzer is running.\n");

    if (argc < 3) {
        printf(COL_RED "[ERROR] Usage: analyzer <input_file> <output_file> [--stats] [--anomalies] [--benchmark]\n");
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    int flag_stats = has_flag(argc, argv, "--stats");
    int flag_anom = has_flag(argc, argv, "--anomalies");
    int flag_bench = has_flag(argc, argv, "--benchmark");

    printf(COL_YELLOW "[INFO] Flags: stats=%d anomalies=%d\n benchmark%d\n", flag_stats, flag_anom, flag_bench);

    double t_parse = 0;
    double t_stats = 0;
    double t_trends = 0;
    double t_anoms = 0;

    double t0 = now_ms();

    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        printf(COL_RED "[ERROR] Could not open file\n");
        return 1;
    }

    int rows = count_rows(fp);

    if (rows <= 0) {
        printf(COL_RED "[ERROR] Invalid row count. File may be corrupted.\n");
        fclose(fp);
        return 1;
    }

    SensorRow *rows_array = calloc(rows, sizeof(SensorRow));
    if (!rows_array) {
        printf(COL_RED "[ERROR] Memory allocation failed.\n");
        fclose(fp);
        return 1;
    }

    int count = load_rows(fp, rows_array, rows);
    if (count == 0) {
        printf(COL_RED "[ERROR] No valid samples found.\n");
        free(rows_array);
        fclose(fp);
        return 1;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        printf(COL_RED "[ERROR] Could not create output file: %s\n", output_file);
        free(rows_array);
        return 1;
    }
    fclose(fp);

    t_parse = now_ms() - t0;

    double temp_min = find_min(rows_array, count, select_temp);
    double temp_max = find_max(rows_array, count, select_temp);
    double temp_avg = find_avg(rows_array, count, select_temp);

    double hum_min = find_min(rows_array, count, select_humidity);
    double hum_max = find_max(rows_array, count, select_humidity);
    double hum_avg = find_avg(rows_array, count, select_humidity);

    double press_min = find_min(rows_array, count, select_pressure);
    double press_max = find_max(rows_array, count, select_pressure);
    double press_avg = find_avg(rows_array, count, select_pressure);

    double aqi_min = find_min(rows_array, count, select_air_qual);
    double aqi_max = find_max(rows_array, count, select_air_qual);
    double aqi_avg = find_avg(rows_array, count, select_air_qual);

    double co2_min = find_min(rows_array, count, select_co2);
    double co2_max = find_max(rows_array, count, select_co2);
    double co2_avg = find_avg(rows_array, count, select_co2);

    double voc_min = find_min(rows_array, count, select_voc);
    double voc_max = find_max(rows_array, count, select_voc);
    double voc_avg = find_avg(rows_array, count, select_voc);

    print_summary(out, input_file, count, rows_array,
                  temp_min, temp_max, temp_avg,
                  hum_min, hum_max, hum_avg,
                  press_min, press_max, press_avg,
                  aqi_min, aqi_max, aqi_avg,
                  co2_min, co2_max, co2_avg,
                  voc_min, voc_max, voc_avg);
    fclose(out);
    if (!flag_stats) {
        double t1 = now_ms();
        t_trends = now_ms() - t1;

        anomaly_detection(rows_array, count, output_file);
    }

    if (!flag_anom) {

        double t2 = now_ms();
        trend_detection(rows_array, count, output_file);
        t_stats = now_ms() - t2;
    }

    /* Benchmark output */
    if (flag_bench) {
        printf("\n==================== BENCHMARK ====================\n");
        printf("Rows processed:      %d\n", count);
        printf("Parse time:          %.2f ms\n", t_parse);
        printf("Stats time:          %.2f ms\n", t_stats);
        printf("Trend time:          %.2f ms\n", t_trends);
        printf("Anomaly time:        %.2f ms\n", t_anoms);
        printf("Memory (rows array): %zu bytes\n", sizeof(SensorRow) * count);
        printf("====================================================\n\n");
    }
    free(rows_array);
    printf(COL_GREEN "[OK] Analyzer: analyzer ran successfully\n\n");
    return 0;
}

/*
   Min / Max / Avg
 */
double find_min(SensorRow arr[], int length, double (*selector)(SensorRow)) {
    double min = selector(arr[0]);
    for (int i = 1; i < length; i++) {
        double v = selector(arr[i]);
        if (v < min) min = v;
    }
    return min;
}

double find_max(SensorRow arr[], int length, double (*selector)(SensorRow)) {
    double max = selector(arr[0]);
    for (int i = 1; i < length; i++) {
        double v = selector(arr[i]);
        if (v > max) max = v;
    }
    return max;
}

double find_avg(SensorRow arr[], int length, double (*selector)(SensorRow)) {
    double sum = 0.0;
    for (int i = 0; i < length; i++)
        sum += selector(arr[i]);
    return sum / length;
}
