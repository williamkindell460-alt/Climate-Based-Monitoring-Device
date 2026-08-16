#ifndef TREND_DETECTION_H
#define TREND_DETECTION_H
#include "analyzer.h"
#include <stdio.h>

void trend_detection(SensorRow *rows, int count, const char *output_file);
void env_stress_indecators(FILE *out, double air_qual_avg, double co2_avg, double voc_avg);
void comfort_assessment(FILE *out, double temp_avg, double hum_avg);
void air_assessment(FILE *out, double air_qual_avg, double co2_avg, double voc_avg);
void env_stability(FILE *out, double temp_delta, double hum_delta, double press_delta, double air_qual_delta, double voc_delta, double co2_delta);
void change_in_values(FILE *out, double temp_delta, double hum_delta, double press_delta, double air_qual_delta, double voc_delta, double co2_delta);
void weather_interpretation(FILE *out, double press_delta, double temp_delta, double hum_delta);
void env_stability(FILE *out, double temp_delta, double hum_delta, double press_delta, double air_qual_delta, double voc_delta, double co2_delta);

#endif