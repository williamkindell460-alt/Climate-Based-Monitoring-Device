add_library(host_tool STATIC
    src/analyzer.c
    src/trend_detection.c
    src/anomaly_detection.c
    src/run_pipeline.c
)

target_include_directories(host_tool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(pipeline
    src/main.c   # calls run_pipeline()
)

target_link_libraries(pipeline PRIVATE host_tool)

# Optional convenience target
add_custom_target(run
    COMMAND pipeline
    DEPENDS pipeline
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
