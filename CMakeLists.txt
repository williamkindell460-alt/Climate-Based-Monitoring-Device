cmake_minimum_required(VERSION 3.10)
project(ClimateMonitor C)

set(CMAKE_C_STANDARD 11)

add_executable(climate_monitor 01_firmware/src/main.c)