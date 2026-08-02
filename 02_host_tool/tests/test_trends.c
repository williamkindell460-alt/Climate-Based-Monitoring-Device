#include "trend_detection.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_change_in_values_detects_rising_temperature(void) {
    const char *outfile = "tmp_change_values.txt";

    // Create empty file
    FILE *f = fopen(outfile, "w");
    fclose(f);

    FILE *out = fopen(outfile, "a");

    change_in_values(out,
                     1.0,  // temp_delta
                     0.0,  // hum_delta
                     0.0,  // press_delta
                     0.0,  // aqi_delta
                     0.0,  // voc_delta
                     0.0); // co2_delta

    fclose(out);

    char buffer[512] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer, "Rising (Δ = +1.00°C)") != NULL);

    remove(outfile);
}

void test_env_stability_detects_unstable_environment(void) {
    const char *outfile = "tmp_env_stability.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    FILE *out = fopen(outfile, "a");

    env_stability(out,
                  1.0, // temp_delta (unstable)
                  0.0,
                  0.0,
                  0.0,
                  0.0,
                  0.0);

    fclose(out);

    char buffer[512] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer,
                            "Environment shows notable changes") != NULL);

    remove(outfile);
}

void test_weather_interpretation_detects_falling_pressure(void) {
    const char *outfile = "tmp_weather.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    FILE *out = fopen(outfile, "a");

    weather_interpretation(out,
                           -2.0, // press_delta
                           0.0,
                           0.0);

    fclose(out);

    char buffer[512] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer,
                            "Pressure is falling") != NULL);

    remove(outfile);
}

void test_air_assessment_unhealthy_aqi(void) {
    const char *outfile = "tmp_air.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    FILE *out = fopen(outfile, "a");

    air_assessment(out,
                   180,  // aqi_avg
                   500,  // co2_avg
                   100); // voc_avg

    fclose(out);

    char buffer[512] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer,
                            "AQI unhealthy") != NULL);

    remove(outfile);
}

void test_comfort_assessment_detects_high_humidity(void) {
    const char *outfile = "tmp_comfort.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    FILE *out = fopen(outfile, "a");

    comfort_assessment(out,
                       22.0,  // temp_avg
                       70.0); // hum_avg (high)

    fclose(out);

    char buffer[512] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer,
                            "Humidity high; may feel muggy") != NULL);

    remove(outfile);
}

void test_env_stress_indicators_detects_pollution(void) {
    const char *outfile = "tmp_stress.txt";

    FILE *f = fopen(outfile, "w");
    fclose(f);

    FILE *out = fopen(outfile, "a");

    env_stress_indecators(out,
                          120,  // aqi_avg (high)
                          500,  // co2_avg
                          300); // voc_avg (high)

    fclose(out);

    char buffer[512] = {0};
    FILE *in = fopen(outfile, "r");
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    TEST_ASSERT_TRUE(strstr(buffer,
                            "Elevated pollution levels") != NULL);

    remove(outfile);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_change_in_values_detects_rising_temperature);
    RUN_TEST(test_env_stability_detects_unstable_environment);
    RUN_TEST(test_weather_interpretation_detects_falling_pressure);
    RUN_TEST(test_air_assessment_unhealthy_aqi);
    RUN_TEST(test_comfort_assessment_detects_high_humidity);
    RUN_TEST(test_env_stress_indicators_detects_pollution);
    return UNITY_END();
}