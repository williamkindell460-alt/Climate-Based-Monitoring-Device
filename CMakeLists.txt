cmake_minimum_required(VERSION 3.15)
project(ClimateMonitor C)

set(CMAKE_C_STANDARD 11)

# Add subdirectories for firmware and host tool
add_subdirectory(01_firmware)
add_subdirectory(02_host_tool)
