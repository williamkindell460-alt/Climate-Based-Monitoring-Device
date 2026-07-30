#ifndef ANALYZER_H
#define ANALYZER_H
#include <stdio.h>

typedef struct {
    double time;
    double temp;
    double humidity;
    double pressure;
    double air_qual;
    double co2;
    double voc;
} SensorRow;

double select_temp(SensorRow row);
double select_humidity(SensorRow row);
double select_pressure(SensorRow row);
double select_air_qual(SensorRow row);
double select_co2(SensorRow row);
double select_voc(SensorRow row);

double find_min(SensorRow arr[], int length, double (*selector)(SensorRow));
double find_max(SensorRow arr[], int length, double (*selector)(SensorRow));
double find_avg(SensorRow arr[], int length, double (*selector)(SensorRow));

int analyzer(const char *input_file, const char *output_file);

#endif
