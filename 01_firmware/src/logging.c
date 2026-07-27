#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "logging.h"
#include "trend_detection.h"


static int is_valid_float(float x)
{
    if (isnan(x)) return 0;
    if (!isfinite(x)) return 0;
    if (x < -10000.0f || x > 10000.0f) return 0;
    return 1;
}

static int parse_float(const char* s, float* out)
{
    char* end;
    float val = strtof(s, &end);

    if (end == s) return 0;          // not a number
    if (!is_valid_float(val)) return 0;

    *out = val;
    return 1;
}

void load_csv_and_feed_trends(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error: cannot open CSV file %s\n", filename);
        return;
    }

    char line[256];
    int corrupted = 0;

    // Skip header
    fgets(line, sizeof(line), f);

    while (fgets(line, sizeof(line), f))
    {
        char* token;
        char* rest = line;

        float temp, hum, pres, aqi, co2, voc;

        // Timestamp (ignored for now)
        token = strtok_r(rest, ",", &rest);

        // Temperature
        token = strtok_r(NULL, ",", &rest);
        if (!parse_float(token, &temp)) { corrupted++; continue; }

        // Humidity
        token = strtok_r(NULL, ",", &rest);
        if (!parse_float(token, &hum)) { corrupted++; continue; }

        // Pressure
        token = strtok_r(NULL, ",", &rest);
        if (!parse_float(token, &pres)) { corrupted++; continue; }

        // AQI
        token = strtok_r(NULL, ",", &rest);
        if (!parse_float(token, &aqi)) { corrupted++; continue; }

        // CO2
        token = strtok_r(NULL, ",", &rest);
        if (!parse_float(token, &co2)) { corrupted++; continue; }

        // VOC
        token = strtok_r(NULL, ",", &rest);
        if (!parse_float(token, &voc)) { corrupted++; continue; }

        // Feed into trend analyzer
        trend_temp_add_sample(temp);
        trend_hum_add_sample(hum);
        trend_pres_add_sample(pres);
        trend_gas_add_sample(voc);   // or AQI/CO2/VOC depending on your mapping
    }

    fclose(f);

    printf("Corrupted log entries ignored: %d\n", corrupted);
}
