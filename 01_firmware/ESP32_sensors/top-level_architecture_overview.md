# ESP32 Sensors Project — Top‑Level Architecture Summary

This project implements a modular environmental monitoring system using an ESP32.  
It collects temperature, humidity, pressure, gas resistance, eCO₂, and TVOC data from  
the BME680 and CCS811 sensors, then logs the results to a CSV file on an SD card.

The project follows ESP‑IDF’s recommended component architecture, with each hardware  
interface and subsystem isolated into its own module for clarity, maintainability,  
and testability.

---

## Project Directory Structure

ESP32_sensors/
│
├── CMakeLists.txt                # Top-level ESP-IDF project file
├── sdkconfig                     # ESP-IDF configuration
├── hardware_checklist.md         # Hardware wiring + setup notes
│
├── .vscode/                      # Editor settings
│   └── settings.json
│
├── main/                         # Application entry point
│   ├── main.c
│   ├── main.h
│   ├── main_architecture.md
│   └── CMakeLists.txt
│
└── components/                   # Modular ESP-IDF components
├── bme680/
│   ├── bme680.c
│   ├── bme680.h
│   ├── BME680_architecture.md
│   └── CMakeLists.txt
│
├── ccs811/
│   ├── ccs811.c
│   ├── ccs811.h
│   ├── CCS811_architecture.md
│   └── CMakeLists.txt
│
├── i2c_bus/
│   ├── i2c_bus.c
│   ├── i2c_bus.h
│   ├── i2c_bus_architecture.md
│   └── CMakeLists.txt
│
├── spi_bus/
│   ├── spi_bus.c
│   ├── spi_bus.h
│   ├── spi_bus_architecture.md
│   └── CMakeLists.txt
│
└── sd_logger/
├── sd_logger.c
├── sd_logger.h
├── sd_logger_architecture.md
└── CMakeLists.txt

---

## Component Overview

### **main/**
The application entry point.  
Coordinates initialization of all subsystems and runs the measurement loop.

Responsibilities:
- Initialize I2C bus  
- Initialize sensors  
- Initialize SPI SD bus  
- Initialize CSV logger  
- Read sensor data every 2 minutes  
- Append data to SD card  

---

### **i2c_bus/**
Hardware abstraction layer for the ESP32 I2C peripheral.

Responsibilities:
- Configure I2C pins and frequency  
- Provide `i2c_master_init()`  
- Provide read/write wrappers used by sensor drivers  

---

### **bme680/**
Driver for Bosch BME680 environmental sensor.

Provides:
- Temperature  
- Humidity  
- Pressure  
- Gas resistance  

Uses:
- `i2c_bus`  
- ESP-IDF I2C driver  

---

### **ccs811/**
Driver for CCS811 air quality sensor.

Provides:
- eCO2
- TVOC  

Uses:
- `i2c_bus`  
- ESP-IDF I2C driver  

---

### **spi_bus/**
SPI + SDSPI host interface for SD card access.

Provides:
- SPI bus initialization  
- SD card mounting at `/sdcard`  
- Automatic creation of `/sdcard/real_output/`  
- Clean unmounting  

Used by:
- `sd_logger`  
- `main.c`  

---

### **sd_logger/**
High-level CSV logging module.

Provides:
- CSV file creation  
- Header generation  
- Row append function  
- AQI computation  
- Timestamp generation  

Writes to:
`/sdcard/real_output/data.csv`

---

## Build System Summary

### Top-level CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ESP32_sensors)
```
### main/CMakeLists.txt
Declares dependencies on all components:
```idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES i2c_bus bme680 ccs811 spi_bus sd_logger)
```
## System Data Flow
+------------------------+
|        app_main()      |
+------------------------+
            |
            v
+------------------------+
|   i2c_bus_init()       |
+------------------------+
            |
            v
+------------------------+
|   bme680_init()        |
|   ccs811_init()        |
+------------------------+
            |
            v
+------------------------+
|   sd_bus_init()        |
+------------------------+
            |
            v
+------------------------+
|   sd_logger_init()     |
+------------------------+
            |
            v
+------------------------+
| Periodic Measurement   |
| Loop (120s)            |
+------------------------+
            |
            v
+------------------------+
| Read Sensors           |
+------------------------+
            |
            v
+------------------------+
| sd_logger_append()     |
+------------------------+
            |
            v
+------------------------+
| CSV written to SD card |
+------------------------+
## Summary
This project is a clean, modular ESP32 firmware system designed for long-term
environmental monitoring. Each hardware interface is isolated into its own
component, and the SD logging system provides persistent, structured data storage
for later analysis.

The architecture is scalable, maintainable, and ready for future additions such as:
- Wi‑Fi upload
- MQTT streaming
- Daily CSV rotation
- Real-time clock timestamps
- Additional sensors