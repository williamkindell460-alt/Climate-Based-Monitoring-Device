#include "../include/anomaly_detection.h"
#include "../include/analyzer.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

void anomaly_detection(SensorRow *rows, int count, const char *output_file) {
    printf("=====================Anomaly Detection=====================\n\n");
    printf("[INFO] Anomaly Detection: anomaly_detection is running.\n\n");
    FILE *out = fopen(output_file, "a");
    if (!out) {
        printf("[ERROR] Could not open output file for appending.\n\n");
        return;
    }

    fprintf(out,
            "----------------------------------------------------\n"
            "Anomaly Detection\n"
            "----------------------------------------------------\n");

    int anomalies_found = 0;

    /*
       HARD ANOMALIES — Physically impossible values
    */
    hard_anomalies(anomalies_found, out, count, rows);

    /*
       SOFT ANOMALIES — Environmentally abnormal values
    */
    soft_anomalies(anomalies_found, out, count, rows);

    /*
       TREND ANOMALIES — Sudden jumps between samples
    */
    trend_anomalies(anomalies_found, out, count, rows);

    fprintf(out, "\n");
    fclose(out);
    printf("[OK] Anomaly Detections: anomaly_detection ran successfully\n\n");
}
int hard_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows) {
    printf("----------------------HARD ANOMALIES----------------------\n\n");
    printf("[INFO] Anomaly Detection: hard_anomalies is running.\n\n");
    for (int i = 0; i < count; i++) {

        if (rows[i].temp < -100 || rows[i].temp > 60) {
            fprintf(out, "Hard anomaly: Impossible temperature at sample %d (%.2f°C)\n", i, rows[i].temp);
            anomalies_found = 1;
        }

        if (rows[i].humidity < 0 || rows[i].humidity > 100) {
            fprintf(out, "Hard anomaly: Impossible humidity at sample %d (%.2f%%)\n", i, rows[i].humidity);
            anomalies_found = 1;
        }

        if (rows[i].pressure < 870 || rows[i].pressure > 1085) {
            fprintf(out, "Hard anomaly: Impossible pressure at sample %d (%.2f hPa)\n", i, rows[i].pressure);
            anomalies_found = 1;
        }

        if (rows[i].air_qual < 0 || rows[i].air_qual > 500) {
            fprintf(out, "Hard anomaly: Impossible AQI at sample %d (%.2f)\n", i, rows[i].air_qual);
            anomalies_found = 1;
        }

        if (rows[i].co2 < 300 || rows[i].co2 > 50000) {
            fprintf(out, "Hard anomaly: Impossible CO₂ at sample %d (%.2f ppm)\n", i, rows[i].co2);
            anomalies_found = 1;
        }

        if (rows[i].voc < 0 || rows[i].voc > 1000000) {
            fprintf(out, "Hard anomaly: Impossible VOC at sample %d (%.2f ppb)\n", i, rows[i].voc);
            anomalies_found = 1;
        }
    }
    printf("[OK] Anomaly Detection: hard_anomalies ran successfully.\n\n");
    return anomalies_found;
}

int soft_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows) {
    printf("----------------------SOFT ANOMALIES----------------------\n\n");
    printf("[INFO] Anomaly Detection: soft_anomalies is running.\n\n");
    for (int i = 0; i < count; i++) {

        if (rows[i].air_qual > 150) {
            fprintf(out, "Environmental anomaly: High AQI at sample %d (%.2f)\n", i, rows[i].air_qual);
            anomalies_found = 1;
        }

        if (rows[i].voc > 200) {
            fprintf(out, "Environmental anomaly: Elevated VOC at sample %d (%.2f ppb)\n", i, rows[i].voc);
            anomalies_found = 1;
        }

        if (rows[i].co2 > 1500) {
            fprintf(out, "Environmental anomaly: High CO₂ at sample %d (%.2f ppm)\n", i, rows[i].co2);
            anomalies_found = 1;
        }

        if (rows[i].pressure < 980) {
            fprintf(out, "Environmental anomaly: Low pressure at sample %d (%.2f hPa) — storm conditions possible\n", i, rows[i].pressure);
            anomalies_found = 1;
        }
    }
    printf("[OK] Anomaly Detection: soft_anomalies ran successfully.\n\n");
    return anomalies_found;
}

int trend_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows) {
    printf("---------------------TREND ANOMALIES---------------------\n\n");
    printf("[INFO] Anomaly Detection: trend_anomalies is running.\n\n");
    for (int i = 1; i < count; i++) {

        double temp_jump = rows[i].temp - rows[i - 1].temp;
        double hum_jump = rows[i].humidity - rows[i - 1].humidity;
        double press_jump = rows[i].pressure - rows[i - 1].pressure;
        double aqi_jump = rows[i].air_qual - rows[i - 1].air_qual;
        double co2_jump = rows[i].co2 - rows[i - 1].co2;
        double voc_jump = rows[i].voc - rows[i - 1].voc;

        if (fabs(temp_jump) > 2.0) {
            fprintf(out, "Trend anomaly: Temperature jump at sample %d (Δ = %.2f°C)\n", i, temp_jump);
            anomalies_found = 1;
        }

        if (fabs(hum_jump) > 5.0) {
            fprintf(out, "Trend anomaly: Humidity jump at sample %d (Δ = %.2f%%)\n", i, hum_jump);
            anomalies_found = 1;
        }

        if (fabs(press_jump) > 1.5) {
            fprintf(out, "Trend anomaly: Pressure jump at sample %d (Δ = %.2f hPa)\n", i, press_jump);
            anomalies_found = 1;
        }

        if (fabs(aqi_jump) > 20.0) {
            fprintf(out, "Trend anomaly: AQI jump at sample %d (Δ = %.2f)\n", i, aqi_jump);
            anomalies_found = 1;
        }

        if (fabs(co2_jump) > 50.0) {
            fprintf(out, "Trend anomaly: CO₂ jump at sample %d (Δ = %.2f ppm)\n", i, co2_jump);
            anomalies_found = 1;
        }

        if (fabs(voc_jump) > 30.0) {
            fprintf(out, "Trend anomaly: VOC jump at sample %d (Δ = %.2f ppb)\n", i, voc_jump);
            anomalies_found = 1;
        }
    }
    /*
       NO ANOMALIES FOUND
    */
    if (!anomalies_found) {
        fprintf(out, "No anomalies detected across all samples.\n\n");
    }
    printf("[OK] Anomaly Detection: trend_anomalies ran successfully.\n\n");
    return anomalies_found;
}