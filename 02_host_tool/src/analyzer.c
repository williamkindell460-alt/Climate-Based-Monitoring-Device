#include "../include/analyzer.h"
#include "../../01_firmware/include/logging.h"
#include "../include/anomaly_detection.h"
#include "../include/trend_detection.h"
#include <math.h> // <-- added for isnan() / isinf()
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // <-- needed for strcmp()

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

/*
   Count rows in binary file
 */
int count_rows(FILE *fp) {
    printf("---------------------- count_rows ----------------------\n\n");
    printf("[INFO] Analyzer: count_rows is running.\n\n");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size % (7 * sizeof(double)) != 0) {
        printf("[ERROR] File size is not divisible by expected row size. Possible corruption.\n");
    }

    printf("[OK] Analyzer: count_rows function ran successfully\n\n");
    return size / (7 * sizeof(double));
}

/*
   Load all sensor rows from file
 */
int load_rows(FILE *fp, SensorRow *rows, int max_rows) {
    printf("---------------------- load_rows ----------------------\n\n");
    printf("[INFO] Analyzer: load_rows is running.\n\n");

    double values[7];
    int count = 0;
    double last_time = -1.0;

    while (fread(values, sizeof(double), 7, fp) == 7 && count < max_rows) {

        int invalid = 0;
        for (int i = 0; i < 7; i++) {
            if (isnan(values[i]) || isinf(values[i])) {
                printf("[WARN] Invalid numeric value (NaN/Inf) detected in row %d\n", count);
                invalid = 1;
            }
        }

        if (values[0] < last_time) {
            printf("[WARN] Timestamp anomaly: non-monotonic time at row %d\n", count);
        }
        last_time = values[0];

        if (values[1] < -50 || values[1] > 150) printf("[WARN] Temperature out of realistic range at row %d\n", count);
        if (values[2] < 0 || values[2] > 100) printf("[WARN] Humidity out of range at row %d\n", count);
        if (values[3] < 300 || values[3] > 1100) printf("[WARN] Pressure out of range at row %d\n", count);
        if (values[4] < 0) printf("[WARN] AQI negative at row %d\n", count);
        if (values[5] < 0) printf("[WARN] CO2 negative at row %d\n", count);
        if (values[6] < 0) printf("[WARN] VOC negative at row %d\n", count);

        rows[count].time = values[0];
        rows[count].temp = values[1];
        rows[count].humidity = values[2];
        rows[count].pressure = values[3];
        rows[count].air_qual = values[4];
        rows[count].co2 = values[5];
        rows[count].voc = values[6];

        count++;
    }

    if (count == max_rows) {
        printf("[WARN] Maximum row limit reached. File may contain more data.\n");
    }

    printf("[OK] Analyzer: load_rows function ran successfully\n\n");
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
    printf("---------------------- print_summary ----------------------\n\n");
    printf("[INFO] Analyzer: print_summary is running.\n\n");

    fprintf(out,
            "Climate-Based Monitoring Device — Host Tool Analysis\n"
            "====================================================\n\n"
            "Input file: %s\n"
            "Samples processed: %d\n"
            "Time range: %.2f minutes\n"
            "----------------------------------------------------\n"
            "Summary Statistics\n"
            "----------------------------------------------------\n",
            filename,
            count,
            rows[count - 1].time);

    fprintf(out,
            "Temperature (C)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            temp_min, temp_max, temp_avg);

    fprintf(out,
            "Humidity (%%)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            hum_min, hum_max, hum_avg);

    fprintf(out,
            "Pressure (hPa)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            press_min, press_max, press_avg);

    fprintf(out,
            "Air Quality Index (AQI)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            aqi_min, aqi_max, aqi_avg);

    fprintf(out,
            "CO2 (ppm)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            co2_min, co2_max, co2_avg);

    fprintf(out,
            "VOC (ppb)\nMin: %.2f\nMax: %.2f\nAverage: %.2f\n\n",
            voc_min, voc_max, voc_avg);

    printf("[OK] Analyzer: print_summary ran successfully\n\n");
}

/*
   Analyzer Main Function
*/
int analyzer(int argc, const char *argv[]) {
    printf("-------------------- ANALYZER --------------------\n\n");
    printf("[INFO] Analyzer: analyzer is running.\n");

    if (argc < 3) {
        printf("[ERROR] Usage: analyzer <input_file> <output_file> [--stats] [--anomalies]\n");
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    int flag_stats = has_flag(argc, argv, "--stats");
    int flag_anom = has_flag(argc, argv, "--anomalies");

    if (!flag_stats && !flag_anom) {
        flag_stats = 1;
        flag_anom = 1;
    }

    printf("[INFO] Flags: stats=%d anomalies=%d\n", flag_stats, flag_anom);

    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        printf("[ERROR] Could not open file\n");
        return 1;
    }

    int rows = count_rows(fp);

    if (rows <= 0) {
        printf("[ERROR] Invalid row count. File may be corrupted.\n");
        fclose(fp);
        return 1;
    }

    SensorRow *rows_array = calloc(rows, sizeof(SensorRow));
    if (!rows_array) {
        printf("[ERROR] Memory allocation failed.\n");
        fclose(fp);
        return 1;
    }

    int count = load_rows(fp, rows_array, rows);
    if (count == 0) {
        printf("[ERROR] No valid samples found.\n");
        free(rows_array);
        fclose(fp);
        return 1;
    }
    fclose(fp);

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

    if (flag_stats) {
        FILE *out = fopen(output_file, "w");
        if (!out) {
            printf("[ERROR] Could not create output file: %s\n", output_file);
            free(rows_array);
            return 1;
        }

        print_summary(out, input_file, count, rows_array,
                      temp_min, temp_max, temp_avg,
                      hum_min, hum_max, hum_avg,
                      press_min, press_max, press_avg,
                      aqi_min, aqi_max, aqi_avg,
                      co2_min, co2_max, co2_avg,
                      voc_min, voc_max, voc_avg);

        fclose(out);
    }

    if (flag_anom) {
        trend_detection(rows_array, count, output_file);
        anomaly_detection(rows_array, count, output_file);
    }

    free(rows_array);
    printf("[OK] Analyzer: analyzer ran successfully\n\n");
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
