add_library(firmware
    src/sensors.c
)

target_include_directories(firmware PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../02_host_tool/include
)

add_executable(main
    src/main.c
)

target_link_libraries(main PRIVATE firmware host_tool)

add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
)
