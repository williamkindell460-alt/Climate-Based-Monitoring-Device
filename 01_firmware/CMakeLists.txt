add_library(firmware
    src/sensors.c
    src/logging.c
    main/main.c
    src/esp32_firmware/app_main.c
)

target_include_directories(firmware PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
