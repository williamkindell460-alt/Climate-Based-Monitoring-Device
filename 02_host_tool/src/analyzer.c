#include "../../01_firmware/include/logging.h"
#include "../include/analyzer.h"
#include "../include/trend_detection.h"
#include "../include/anomaly_detection.h"
#include <stdio.h>
#include <stdlib.h>

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
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size / (7 * sizeof(double));
}

/* 
   Load all sensor rows from file
 */
int load_rows(FILE *fp, SensorRow *rows, int max_rows) {
    double values[7];
    int count = 0;

    while (fread(values, sizeof(double), 7, fp) == 7 && count < max_rows) {
        rows[count].time = values[0];
        rows[count].temp = values[1];
        rows[count].humidity = values[2];
        rows[count].pressure = values[3];
        rows[count].air_qual = values[4];
        rows[count].co2 = values[5];
        rows[count].voc = values[6];
        count++;
    }

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
                   double hum_min,  double hum_max,  double hum_avg,
                   double press_min,double press_max,double press_avg,
                   double aqi_min,  double aqi_max,  double aqi_avg,
                   double co2_min,  double co2_max,  double co2_avg,
                   double voc_min,  double voc_max,  double voc_avg)
{


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
        rows[count - 1].time
    );

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
}

/*
   Analyzer Main Function
*/
int analyzer(const char *input_file, const char *output_file) {
    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        printf("Could not open file\n");
        return 1;
    }

    int rows = count_rows(fp);
    SensorRow *rows_array = calloc(rows, sizeof(SensorRow));
    int count = load_rows(fp, rows_array, rows);
    if (count == 0) {
        printf("No valid samples found.\n");
        free(rows_array);
        return 1;
    }
    fclose(fp);

    /* Compute statistics */
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


    /* Write report */
    FILE *out = fopen(output_file, "w");
    if (!out) {
        printf("Could not create output file: %s\n", output_file);
        free(rows_array);
        return 1;
    }

    print_summary(out, input_file, count, rows_array, temp_min, temp_max, temp_avg, hum_min, hum_max, hum_avg, press_min, press_max, press_avg, aqi_min, aqi_max, aqi_avg, co2_min, co2_max, co2_avg, voc_min, voc_max, voc_avg);

    fclose(out);
    trend_detection(rows_array, count, output_file);
    anomaly_detection(rows_array, count, output_file);

    free(rows_array);
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
