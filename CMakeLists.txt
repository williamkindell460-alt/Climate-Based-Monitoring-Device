cmake_minimum_required(VERSION 3.15)
project(ClimateMonitor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sample logs for tests
set(SAMPLE_INPUT_DIR "${CMAKE_SOURCE_DIR}/02_host_tool/examples/sample_input")
set(SAMPLE_INPUT_FILE "${SAMPLE_INPUT_DIR}/sample_log_01.csv")
set(TEST_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/02_host_tool/tests/output_data")

# Real firmware output for actual runs
set(REAL_BIN "${CMAKE_SOURCE_DIR}/03_data/output_data/output.bin")
set(REAL_OUTPUT_TXT "${CMAKE_SOURCE_DIR}/03_data/output_data/analysis_output.txt")

find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_compile_definitions(PYTHON_EXECUTABLE="${Python3_EXECUTABLE}")

enable_testing()

add_subdirectory(01_firmware)
add_subdirectory(02_host_tool)



# -------------------- Register Tests Here --------------------
add_test(NAME minmax_tests
         COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/test_minmax)
add_test(NAME anomalies_tests
         COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/test_anomalies)
add_test(NAME trends_tests
         COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/test_trends)
add_test(NAME pipeline_default
         COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/test_pipeline_default)
add_test(NAME pipeline_trends_anomalies
         COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/test_pipeline_trends_anomalies)


# -------------------- Unified Test Runner --------------------
add_custom_target(host_tool_run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
#The above test runs ctest --output-on-failure, and tests all tests defined in the Register Tests Here section
# -------------------- Individual Pipeline Mode Targets (TEST MODE) --------------------
add_custom_target(host_tool_run_test_pipeline_default
    COMMAND ${CMAKE_CTEST_COMMAND} -R pipeline_default --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_target(host_tool_run_test_pipeline_trends_anomalies
    COMMAND ${CMAKE_CTEST_COMMAND} -R pipeline_trends_anomalies  --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
# Above tests run in ctest, below tests run the sample outputs without ctest testing
add_custom_target(host_tool_test_only_pipeline_default
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E env
            CBMD_TEST_DATA_DIR=${SAMPLE_INPUT_DIR}
            CBMD_TEST_OUTPUT_DIR=${TEST_OUTPUT_DIR}
            ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${SAMPLE_INPUT_FILE}
            ${TEST_OUTPUT_DIR}/analysis_output.txt
)

add_custom_target(host_tool_test_only_pipeline_stats
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E env
            CBMD_TEST_DATA_DIR=${SAMPLE_INPUT_DIR}
            CBMD_TEST_OUTPUT_DIR=${TEST_OUTPUT_DIR}
            ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${SAMPLE_INPUT_FILE}
            ${TEST_OUTPUT_DIR}/analysis_output.txt
            --stats
)

add_custom_target(host_tool_test_only_pipeline_anomalies
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E env
            CBMD_TEST_DATA_DIR=${SAMPLE_INPUT_DIR}
            CBMD_TEST_OUTPUT_DIR=${TEST_OUTPUT_DIR}
            ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${SAMPLE_INPUT_FILE}
            ${TEST_OUTPUT_DIR}/analysis_output.txt
            --anomalies
)

# -------------------- Real Pipeline Run Targets --------------------
add_custom_target(host_tool_run
    COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${REAL_BIN}
            ${REAL_OUTPUT_TXT}
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_target(host_tool_run_stats
    COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${REAL_BIN}
            ${REAL_OUTPUT_TXT}
            --stats
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(host_tool_run_anomalies
    COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${REAL_BIN}
            ${REAL_OUTPUT_TXT}
            --anomalies
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(host_tool_run_benchmark
    COMMAND ${CMAKE_BINARY_DIR}/02_host_tool/pipeline.exe
            ${REAL_BIN}
            ${REAL_OUTPUT_TXT}
            --benchmark
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_dependencies(run_tests
    test_minmax
    test_anomalies
    test_trends
    test_pipeline_default
    test_pipeline_trends_anomalies
)
add_dependencies(run_test_pipeline_default
    test_pipeline_default
)
add_dependencies(run_test_pipeline_trends_anomalies
    test_pipeline_trends_anomalies
)