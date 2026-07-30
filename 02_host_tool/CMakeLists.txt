add_library(host_tool STATIC
    src/analyzer.c
    src/trend_detection.c
    src/anomaly_detection.c
)

target_include_directories(host_tool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/01_firmware/include
)
