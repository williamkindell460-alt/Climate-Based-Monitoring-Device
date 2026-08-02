#include "../third_party/unity/unity.h"
#include "run_pipeline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static char cbmd_env_var[512];
static char cbmd_output_env[512];

/* Override CSV directory for tests */
static void use_test_csvs(void) {
    snprintf(cbmd_env_var,
             sizeof(cbmd_env_var),
             "CBMD_TEST_DATA_DIR=%s/02_host_tool/examples/sample_input",
             SOURCE_DIR);

    putenv(cbmd_env_var);
}

/* Override analyzer output directory for tests */
static void use_test_output_dir(void) {
    snprintf(cbmd_output_env,
             sizeof(cbmd_output_env),
             "CBMD_TEST_OUTPUT_DIR=%s/02_host_tool/tests/output_data",
             SOURCE_DIR);

    putenv(cbmd_output_env);
}

void test_pipeline_default_mode(void) {
    use_test_csvs();
    use_test_output_dir();

    int result = run_pipeline(MODE_DEFAULT);
    TEST_ASSERT_EQUAL(0, result);

    FILE *f = fopen(SOURCE_DIR "/02_host_tool/tests/output_data/analysis_output.txt", "r");
    TEST_ASSERT_NOT_NULL(f);

    char buffer[65536] = {0}; // 64 KB

    fread(buffer, 1, sizeof(buffer) - 1, f);
    fclose(f);

    TEST_ASSERT_TRUE(strstr(buffer, "Trend Detection") != NULL);
    TEST_ASSERT_TRUE(strstr(buffer, "Summary Statistics") != NULL);
    TEST_ASSERT_TRUE(strstr(buffer, "Anomaly Detection") != NULL);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_pipeline_default_mode);
    return UNITY_END();
}