#include "analyzer.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_find_min_temperature(void) {
    SensorRow rows[3] = {
        {.time = 0, .temp = 10, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0},
        {.time = 1, .temp = 5, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0},
        {.time = 2, .temp = 7, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0}};

    // Min value is located at time 1
    double result = find_min(rows, 3, select_temp);
    TEST_ASSERT_EQUAL_FLOAT(5.0, result);
}

void test_find_max_temperature(void) {
    SensorRow rows[3] = {
        {.time = 0, .temp = 10, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0},
        {.time = 1, .temp = 5, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0},
        {.time = 2, .temp = 7, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0}};

    // Max value is located at time 0
    double result = find_max(rows, 3, select_temp);
    TEST_ASSERT_EQUAL_FLOAT(10.0, result);
}
void test_find_avg_temperature(void) {
    SensorRow rows[3] = {
        {.time = 0, .temp = 10, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0},
        {.time = 1, .temp = 5, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0},
        {.time = 2, .temp = 7, .humidity = 0, .pressure = 0, .air_qual = 0, .co2 = 0, .voc = 0}};

    double result = find_avg(rows, 3, select_temp);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 7.3333, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_find_min_temperature);
    RUN_TEST(test_find_max_temperature);
    RUN_TEST(test_find_avg_temperature);
    return UNITY_END();
}
