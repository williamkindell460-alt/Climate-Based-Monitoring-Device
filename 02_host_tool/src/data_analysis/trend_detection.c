#include "trend_detection.h"
#include "analyzer.h"
#include "color.h"
#include <stdint.h>
#include <stdio.h>

void trend_detection(SensorRow *rows, int count, const char *output_file) {
    printf(COL_CYAN "==================== TREND DETECTION ====================\n\n" COL_RESET);
    printf(COL_YELLOW "[INFO] Trend Detection: trend_detection is running.\n\n" COL_RESET);
    double temp_delta = rows[count - 1].temp - rows[0].temp;
    double hum_delta = rows[count - 1].humidity - rows[0].humidity;
    double press_delta = rows[count - 1].pressure - rows[0].pressure;
    double air_qual_delta = rows[count - 1].air_qual - rows[0].air_qual;
    double co2_delta = rows[count - 1].co2 - rows[0].co2;
    double voc_delta = rows[count - 1].voc - rows[0].voc;

    double total_time = rows[count - 1].time;

    double temp_avg = 0, hum_avg = 0, press_avg = 0;
    double air_qual_avg = 0, co2_avg = 0, voc_avg = 0;

    for (int i = 0; i < count; i++) {
        temp_avg += rows[i].temp;
        hum_avg += rows[i].humidity;
        press_avg += rows[i].pressure;
        air_qual_avg += rows[i].air_qual;
        co2_avg += rows[i].co2;
        voc_avg += rows[i].voc;
    }

    temp_avg /= count;
    hum_avg /= count;
    press_avg /= count;
    air_qual_avg /= count;
    co2_avg /= count;
    voc_avg /= count;

    FILE *out = fopen(output_file, "a");
    if (!out) {
        printf(COL_RED "Could not open output file for appending.\n\n" COL_RESET);
        return;
    }

    fprintf(out,
            "----------------------------------------------------\n"
            "Trend Detection\n"
            "----------------------------------------------------\n\n"
            "Change over %.2f minutes:\n\n",
            total_time);

    change_in_values(out, temp_delta, hum_delta, press_delta, air_qual_delta, voc_delta, co2_delta);
    /*
       1. General Stability Assessment
    */
    env_stability(out, temp_delta, hum_delta, press_delta, air_qual_delta, voc_delta, co2_delta);
    /*
       2. Weather Interpretation (Pressure + Temp + Humidity)
    */
    weather_interpretation(out, press_delta, temp_delta, hum_delta);
    /*
       3. Air Quality & Pollution Classification
    */
    air_assessment(out, air_qual_avg, co2_avg, voc_avg);
    /*
       4. Comfort Assessment
    */
    comfort_assessment(out, temp_avg, hum_avg);
    /*
       5. Environmental Stress Indicators
    */
    env_stress_indecators(out, air_qual_avg, co2_avg, voc_avg);

    fclose(out);
    printf(COL_GREEN "[OK] Trend Detection: trend_detection ran successfully\n\n" COL_RESET);
}

void change_in_values(FILE *out, double temp_delta, double hum_delta, double press_delta, double air_qual_delta, double voc_delta, double co2_delta) {
    printf(COL_YELLOW "[INFO] Trend Detection: change_in_values is running.\n\n" COL_RESET);
    // Temperature
    fprintf(out, "Temperature: ");
    if (temp_delta > 0.2)
        fprintf(out, "Rising (Δ = +%.2f°C)\n", temp_delta);
    else if (temp_delta < -0.2)
        fprintf(out, "Falling (Δ = %.2f°C)\n", temp_delta);
    else
        fprintf(out, "Stable (Δ = %.2f°C)\n", temp_delta);

    // Humidity
    fprintf(out, "Humidity: ");
    if (hum_delta > 0.5)
        fprintf(out, "Increasing (Δ = +%.2f%%)\n", hum_delta);
    else if (hum_delta < -0.5)
        fprintf(out, "Decreasing (Δ = %.2f%%)\n", hum_delta);
    else
        fprintf(out, "Stable (Δ = %.2f%%)\n", hum_delta);

    // Pressure
    fprintf(out, "Pressure: ");
    if (press_delta > 0.5)
        fprintf(out, "Rising (Δ = +%.2f hPa)\n", press_delta);
    else if (press_delta < -0.5)
        fprintf(out, "Falling (Δ = %.2f hPa)\n", press_delta);
    else
        fprintf(out, "Stable (Δ = %.2f hPa)\n", press_delta);

    // Air Quality
    fprintf(out, "Air Quality: ");
    if (air_qual_delta > 2)
        fprintf(out, "Increasing (Δ = +%.2f)\n", air_qual_delta);
    else if (air_qual_delta < -2)
        fprintf(out, "Decreasing (Δ = %.2f)\n", air_qual_delta);
    else
        fprintf(out, "Stable (Δ = %.2f)\n", air_qual_delta);

    // CO2
    fprintf(out, "CO2: ");
    if (co2_delta > 3)
        fprintf(out, "Increasing (Δ = +%.2f ppm)\n", co2_delta);
    else if (co2_delta < -3)
        fprintf(out, "Decreasing (Δ = %.2f ppm)\n", co2_delta);
    else
        fprintf(out, "Stable (Δ = %.2f ppm)\n", co2_delta);

    // VOC
    fprintf(out, "VOC: ");
    if (voc_delta > 3)
        fprintf(out, "Increasing (Δ = +%.2f ppb)\n", voc_delta);
    else if (voc_delta < -3)
        fprintf(out, "Decreasing (Δ = %.2f ppb)\n", voc_delta);
    else
        fprintf(out, "Stable (Δ = %.2f ppb)\n", voc_delta);

    fprintf(out, "\n");

    fprintf(out,
            "----------------------------------------------------\n"
            "Overall Assessment\n"
            "----------------------------------------------------\n");
    printf(COL_GREEN "[OK] Trend Detection: change_in_values ran successfully.\n\n" COL_RESET);
}

void env_stability(FILE *out, double temp_delta, double hum_delta, double press_delta, double air_qual_delta, double voc_delta, double co2_delta) {
    printf(COL_YELLOW "[INFO] Trend Detection: env_stability is running.\n\n" COL_RESET);
    int stable = 1;

    if (temp_delta > 0.5 || temp_delta < -0.5) stable = 0;
    if (hum_delta > 1.0 || hum_delta < -1.0) stable = 0;
    if (press_delta > 1.0 || press_delta < -1.0) stable = 0;
    if (air_qual_delta > 5 || air_qual_delta < -5) stable = 0;
    if (co2_delta > 10 || co2_delta < -10) stable = 0;
    if (voc_delta > 10 || voc_delta < -10) stable = 0;

    if (stable) {
        fprintf(out,
                "Environment stable with minor fluctuations across all metrics.\n");
    } else {
        fprintf(out,
                "Environment shows notable changes in one or more metrics.\n");
    }
    printf(COL_GREEN "[OK] Trend Detection: env_stability ran successfully.\n\n" COL_RESET);
}

void weather_interpretation(FILE *out, double press_delta, double temp_delta, double hum_delta) {
    printf(COL_YELLOW "[INFO] Trend Detection: weather_interpretation is running.\n\n" COL_RESET);
    fprintf(out, "Weather Interpretation:\n");

    if (press_delta > 1.0)
        fprintf(out, "- Atmospheric pressure is rising, indicating stabilizing or improving weather.\n");
    else if (press_delta < -1.0)
        fprintf(out, "- Pressure is falling, suggesting incoming unstable weather or storms.\n");
    else
        fprintf(out, "- Pressure stable; no significant weather changes expected.\n");

    if (temp_delta > 0.5)
        fprintf(out, "- Temperature increasing slightly.\n");
    else if (temp_delta < -0.5)
        fprintf(out, "- Temperature decreasing slightly.\n");
    else
        fprintf(out, "- Temperature stable.\n");

    if (hum_delta > 1.0)
        fprintf(out, "- Humidity rising; air may feel more humid.\n");
    else if (hum_delta < -1.0)
        fprintf(out, "- Humidity decreasing; air may feel drier.\n");
    else
        fprintf(out, "- Humidity stable.\n");
    printf(COL_GREEN "[OK] Trend Detection: weather_interpretation ran successfully.\n\n" COL_RESET);
}

void air_assessment(FILE *out, double air_qual_avg, double co2_avg, double voc_avg) {
    printf(COL_YELLOW "[INFO] Trend Detection: air_assessment is running.\n\n" COL_RESET);
    fprintf(out, "Air Quality & Pollution:\n");

    if (air_qual_avg >= 80)
        fprintf(out, "- Air quality excellent — clean, fresh outdoor air.\n");
    else if (air_qual_avg >= 60)
        fprintf(out, "- Air quality good — typical outdoor conditions.\n");
    else if (air_qual_avg >= 40)
        fprintf(out, "- Air quality moderate — noticeable pollution or humidity.\n");
    else if (air_qual_avg >= 20)
        fprintf(out, "- Air quality poor — stagnant or polluted air.\n");
    else
        fprintf(out, "- Air quality very poor — avoid prolonged exposure.\n");


    if (co2_avg <= 430)
        fprintf(out, "- CO₂ levels match clean outdoor baseline.\n");
    else if (co2_avg <= 550)
        fprintf(out, "- CO₂ levels suggest suburban traffic influence.\n");
    else if (co2_avg <= 700)
        fprintf(out, "- CO₂ elevated; likely urban or industrial influence.\n");
    else
        fprintf(out, "- CO₂ high; poor ventilation or strong pollution source.\n");

    if (voc_avg <= 50)
        fprintf(out, "- VOC levels typical of rural outdoor air.\n");
    else if (voc_avg <= 100)
        fprintf(out, "- VOC levels typical of suburban outdoor air.\n");
    else if (voc_avg <= 200)
        fprintf(out, "- VOC levels typical of urban environments.\n");
    else
        fprintf(out, "- VOC levels elevated; industrial pollution or chemical sources likely.\n");
    printf(COL_GREEN "[OK] Trend Detection: air_assessment ran successfully.\n\n" COL_RESET);
}

void comfort_assessment(FILE *out, double temp_avg, double hum_avg) {
    printf(COL_YELLOW "[INFO] Trend Detection: comfort_assessment is running.\n\n" COL_RESET);
    fprintf(out, "\nComfort Assessment:\n");

    if (temp_avg >= 20 && temp_avg <= 25)
        fprintf(out, "- Temperature within comfortable indoor range.\n");
    else
        fprintf(out, "- Temperature outside typical comfort range.\n");

    if (hum_avg >= 30 && hum_avg <= 50)
        fprintf(out, "- Humidity within comfortable indoor range.\n");
    else if (hum_avg > 60)
        fprintf(out, "- Humidity high; may feel muggy.\n");
    else
        fprintf(out, "- Humidity low; air may feel dry.\n");
    printf(COL_GREEN "[OK] Trend Detection: comfort_assessment ran successfully.\n\n" COL_RESET);
}

void env_stress_indecators(FILE *out, double air_qual_avg, double co2_avg, double voc_avg) {
    printf(COL_YELLOW "[INFO] Trend Detection: env_stress_indecators is running.\n\n" COL_RESET);
    fprintf(out, "\nEnvironmental Stress Indicators:\n");

    if (air_qual_avg >= 80 && co2_avg <= 430 && voc_avg <= 50)
        fprintf(out, "- No signs of environmental stress detected.\n");
    else if (air_qual_avg < 40 || voc_avg > 200)
        fprintf(out, "- Elevated pollution levels may cause irritation or discomfort.\n");
    else if (co2_avg > 800)
        fprintf(out, "- High CO₂ indicates poor ventilation or stagnant air.\n");
    else
        fprintf(out, "- Minor environmental stress detected; monitor air quality trends.\n");

    fprintf(out, "\n");
    printf(COL_GREEN "[OK] Trend Detection: env_stress_indecators ran successfully.\n\n" COL_RESET);
}