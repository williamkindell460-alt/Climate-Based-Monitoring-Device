#include "anomaly_detection.h"
#include "analyzer.h"
#include "color.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

void anomaly_detection(SensorRow *rows, int count, const char *output_file) {
    printf(COL_CYAN "===================== Anomaly Detection =====================\n\n" COL_RESET);
    printf(COL_YELLOW "[INFO] Anomaly Detection: anomaly_detection is running.\n\n" COL_RESET);
    FILE *out = fopen(output_file, "a");
    if (!out) {
        printf(COL_RED "[ERROR] Could not open output file for appending.\n\n" COL_RESET);
        return;
    }

    fprintf(out,
            "----------------------------------------------------\n"
            "Anomaly Detection\n"
            "----------------------------------------------------\n\n");

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
    printf(COL_GREEN "[OK] Anomaly Detection: anomaly_detection ran successfully\n\n");
}

int hard_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows) {
    printf(COL_CYAN "---------------------- HARD ANOMALIES ----------------------\n\n" COL_RESET);
    printf(COL_YELLOW "[INFO] Anomaly Detection: hard_anomalies is running.\n\n" COL_RESET);
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

        if (rows[i].air_qual < 0 || rows[i].air_qual > 100) {
            fprintf(out, "Hard anomaly: Impossible air quality value at sample %d (%.2f)\n", i, rows[i].air_qual);
            anomalies_found = 1;
        }

        if (rows[i].co2 < 300 || rows[i].co2 > 50000) {
            fprintf(out, "Hard anomaly: Impossible CO2 at sample %d (%.2f ppm)\n", i, rows[i].co2);
            anomalies_found = 1;
        }

        if (rows[i].voc < 0 || rows[i].voc > 1000000) {
            fprintf(out, "Hard anomaly: Impossible VOC at sample %d (%.2f ppb)\n", i, rows[i].voc);
            anomalies_found = 1;
        }
    }
    printf(COL_GREEN "[OK] Anomaly Detection: hard_anomalies ran successfully.\n\n");
    return anomalies_found;
}

int soft_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows) {
    printf(COL_CYAN "---------------------- SOFT ANOMALIES ----------------------\n\n" COL_RESET);
    printf(COL_YELLOW "[INFO] Anomaly Detection: soft_anomalies is running.\n\n" COL_RESET);
    for (int i = 0; i < count; i++) {

        // Air quality: 0–100 scale (higher = cleaner)
        if (rows[i].air_qual < 40) {
            fprintf(out, "Environmental anomaly: Poor air quality at sample %d (%.2f)\n", i, rows[i].air_qual);
            anomalies_found = 1;
        }

        // VOC: typical outdoor range 0–200 ppb, >300 indicates industrial or traffic influence
        if (rows[i].voc > 300) {
            fprintf(out, "Environmental anomaly: Elevated VOC at sample %d (%.2f ppb)\n", i, rows[i].voc);
            anomalies_found = 1;
        }

        // CO2: global baseline ≈420 ppm, >1000 ppm indicates trapped or polluted air
        if (rows[i].co2 > 1000) {
            fprintf(out, "Environmental anomaly: High CO2 at sample %d (%.2f ppm)\n", i, rows[i].co2);
            anomalies_found = 1;
        }

        // Pressure: <980 hPa often signals storm fronts or low-pressure systems
        if (rows[i].pressure < 980) {
            fprintf(out, "Environmental anomaly: Low pressure at sample %d (%.2f hPa) — storm conditions possible\n", i, rows[i].pressure);
            anomalies_found = 1;
        }
    }
    printf(COL_GREEN "[OK] Anomaly Detection: soft_anomalies ran successfully.\n\n" COL_RESET);
    return anomalies_found;
}

int trend_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows) {
    printf(COL_CYAN "--------------------- TREND ANOMALIES ---------------------\n\n" COL_RESET);
    printf(COL_YELLOW "[INFO] Anomaly Detection: trend_anomalies is running.\n\n" COL_RESET);
    for (int i = 1; i < count; i++) {

        double temp_jump = rows[i].temp - rows[i - 1].temp;
        double hum_jump = rows[i].humidity - rows[i - 1].humidity;
        double press_jump = rows[i].pressure - rows[i - 1].pressure;
        double air_qual_jump = rows[i].air_qual - rows[i - 1].air_qual;
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

        if (fabs(air_qual_jump) > 7.0) {
            fprintf(out, "Trend anomaly: Air quality jump at sample %d (Δ = %.2f)\n", i, air_qual_jump);
            anomalies_found = 1;
        }

        if (fabs(co2_jump) > 50.0) {
            fprintf(out, "Trend anomaly: CO2 jump at sample %d (Δ = %.2f ppm)\n", i, co2_jump);
            anomalies_found = 1;
        }

        if (fabs(voc_jump) > 30.0) {
            fprintf(out, "Trend anomaly: VOC jump at sample %d (Δ = %.2f ppb)\n", i, voc_jump);
            anomalies_found = 1;
        }
    }

    if (!anomalies_found) {
        fprintf(out, "No anomalies detected across all samples.\n\n");
    }
    printf(COL_GREEN "[OK] Anomaly Detection: trend_anomalies ran successfully.\n\n" COL_RESET);
    return anomalies_found;
}