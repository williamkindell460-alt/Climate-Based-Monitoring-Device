#ifndef ANOMALY_DETECTION_H
#define ANOMALY_DETECTION_H
#include "analyzer.h"
#include <stdio.h>

void anomaly_detection(SensorRow *rows, int count, const char *output_file);
int hard_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows);
int soft_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows);
int trend_anomalies(int anomalies_found, FILE *out, int count, SensorRow *rows);

#endif
