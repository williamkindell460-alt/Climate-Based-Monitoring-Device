cmake_minimum_required(VERSION 3.15)
project(ClimateMonitor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_compile_definitions(PYTHON_EXECUTABLE="${Python3_EXECUTABLE}")

enable_testing()

add_subdirectory(01_firmware)
add_subdirectory(02_host_tool)
