#include "anomaly_detection.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_hard_anomalies_detects_impossible_temperature(void) {
    SensorRow rows[1] = {
        {.temp = 200.0, .humidity = 50, .pressure = 1000, .air_qual = 50, .co2 = 500, .voc = 100}};

    const char *outfile = "test_output.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    int anomalies_found = 0;
    FILE *out = fopen(outfile, "a");

    int result = hard_anomalies(anomalies_found, out, 1, rows);
    fclose(out);

    TEST_ASSERT_EQUAL(1, result);

    char buffer[256] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer), in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer, "Impossible temperature") != NULL);
    remove(outfile);
}

void test_soft_anomalies_detects_high_co2(void) {
    SensorRow rows[1] = {
        {.temp = 20, .humidity = 40, .pressure = 1000, .air_qual = 50, .co2 = 5000, .voc = 100}};

    const char *outfile = "test_output.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    int anomalies_found = 0;
    FILE *out = fopen(outfile, "a");

    int result = soft_anomalies(anomalies_found, out, 1, rows);
    fclose(out);

    TEST_ASSERT_EQUAL(1, result);

    char buffer[256] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer), in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer, "High CO₂") != NULL);
    remove(outfile);
}

void test_trend_anomalies_detects_temp_jump(void) {
    SensorRow rows[2] = {
        {.temp = 20, .humidity = 40, .pressure = 1000, .air_qual = 50, .co2 = 500, .voc = 100},
        {.temp = 30, .humidity = 40, .pressure = 1000, .air_qual = 50, .co2 = 500, .voc = 100}};

    const char *outfile = "test_output.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    int anomalies_found = 0;
    FILE *out = fopen(outfile, "a");

    int result = trend_anomalies(anomalies_found, out, 2, rows);
    fclose(out);

    TEST_ASSERT_EQUAL(1, result);

    char buffer[256] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer), in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer, "Temperature jump") != NULL);
    remove(outfile);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hard_anomalies_detects_impossible_temperature);
    RUN_TEST(test_soft_anomalies_detects_high_co2);
    RUN_TEST(test_trend_anomalies_detects_temp_jump);
    return UNITY_END();
}