cmake_minimum_required(VERSION 3.15)
project(HostTool C)

add_compile_definitions(TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/02_host_tool/examples/sample_input")
add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_library(host_tool STATIC
    src/data_analysis/analyzer.c
    src/data_analysis/trend_detection.c
    src/data_analysis/anomaly_detection.c
    src/data_analysis/run_pipeline.c
)

target_include_directories(host_tool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Unit Testing
add_executable(test_minmax
    tests/test_minmax.c
    third_party/unity/unity.c
)
add_executable(test_anomalies
    tests/test_anomalies.c
    third_party/unity/unity.c
)

add_executable(test_trends
    tests/test_trends.c
    third_party/unity/unity.c
)

add_executable(test_pipeline_default
    tests/test_pipeline_default.c
    third_party/unity/unity.c
)

add_executable(test_pipeline_trends_anomalies
    tests/test_pipeline_trends_anomalies.c
    third_party/unity/unity.c
)

target_link_libraries(test_pipeline_default PRIVATE host_tool)
target_link_libraries(test_pipeline_trends_anomalies PRIVATE host_tool)


target_include_directories(test_minmax PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity
)
target_include_directories(test_anomalies PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity
)

target_include_directories(test_trends PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity
)

target_include_directories(test_pipeline_default PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity
)

target_include_directories(test_pipeline_trends_anomalies PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity
)


target_link_libraries(test_minmax PRIVATE host_tool)
target_link_libraries(test_anomalies PRIVATE host_tool)
target_link_libraries(test_trends PRIVATE host_tool)

add_executable(pipeline
    src/data_analysis/main.c
)

target_include_directories(pipeline PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WIN32)
    set_target_properties(pipeline PROPERTIES LINK_FLAGS "-mconsole")
endif()

target_link_libraries(pipeline PRIVATE host_tool)


