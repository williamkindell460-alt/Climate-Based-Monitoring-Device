add_library(firmware
    src/sensors.c
    src/init_dev.c
)

target_include_directories(firmware PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
