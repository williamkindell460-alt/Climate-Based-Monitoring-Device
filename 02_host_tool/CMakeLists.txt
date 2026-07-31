cmake_minimum_required(VERSION 3.15)
project(HostTool C)

add_library(host_tool STATIC
    src/analyzer.c
    src/trend_detection.c
    src/anomaly_detection.c
    src/run_pipeline.c
)

target_include_directories(host_tool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_minmax
    tests/test_minmax.c
    tests/unity.c
)

target_include_directories(test_minmax PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(test_minmax PRIVATE host_tool)

add_test(NAME minmax_tests COMMAND test_minmax)

add_executable(pipeline
    src/main.c
)

target_include_directories(pipeline PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WIN32)
    set_target_properties(pipeline PROPERTIES LINK_FLAGS "-mconsole")
endif()

target_link_libraries(pipeline PRIVATE host_tool)


add_custom_target(run
    COMMAND pipeline
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
