#ifndef UTILS_H
#define UTILS_H
//temperature
void trend_temp_add_sample(float temp);
float trend_temp_delta(void);
float trend_temp_average(void);
//humidity
void trend_humidity_add_sample(float humidity);
float trend_humidity_delta(void);
float trend_humidity_average(void);
//pressure
void trend_pressure_add_sample(float pressure);
float trend_pressure_delta(void);
float trend_pressure_average(void);
//air quality
void trend_air_quality_add_sample(float air_quality);
float trend_air_quality_delta(void);
float trend_air_quality_average(void);
//Report
void trend_print_report(void);

#endif
