# Firmware Overview

This document provides a high-level overview of the ESP32 Climate Monitoring System firmware.  
It explains the system architecture, initialization sequence, sensor integration, timing model,  
and runtime behavior of the firmware.

---

## 1. Firmware Purpose

The firmware is responsible for:

- Initializing the I2C bus
- Configuring and validating environmental sensors
- Executing a stable, drift-free measurement loop
- Collecting temperature, humidity, pressure, gas resistance, eCO₂, and TVOC data
- Logging unified environmental metrics
- Handling initialization and runtime errors gracefully

The design emphasizes robustness, predictability, and long-term stability.

---

## 2. System Architecture

The firmware is organized around a single top-level entry point (`app_main()`), which orchestrates:

### Core Components
- FreeRTOS  
  - Task scheduling  
  - Precise periodic timing (`vTaskDelayUntil`)

- ESP-IDF Core  
  - Logging (`esp_log.h`)  
  - System utilities (`esp_system.h`)

- I2C Bus Layer  
  - Shared bus initialization  
  - Error-checked setup for all sensors

- Sensor Drivers  
  - BME680 (temperature, humidity, pressure, gas resistance)  
  - CCS811 (eCO₂, TVOC)

Each sensor driver encapsulates its own initialization, configuration, and readout logic.

---

## 3. Initialization Sequence

The firmware performs a strict, ordered initialization:

1. Initialize I2C Bus  
   - Sets up the shared bus  
   - Aborts on failure

2. Initialize BME680  
   - Reads chip ID  
   - Loads calibration data  
   - Applies configuration  
   - Aborts on failure

3. Initialize CCS811  
   - Switches to application mode  
   - Configures measurement mode  
   - Aborts on failure

4. Stabilization Delay  
   - 100 ms warm-up before first measurement

This ensures both sensors are fully ready before entering the measurement loop.

---

## 4. Measurement Loop

The firmware uses a 2-minute drift-free loop driven by:
`   
    TickType_t last_wake = xTaskGetTickCount();
    vTaskDelayUntil(&last_wake, period);
`

### Loop Responsibilities

- Read BME680 values:
  - Temperature
  - Humidity
  - Pressure
  - Gas resistance

- Read CCS811 values:
  - eCO₂
  - TVOC

- Replace NAN values with 0.0f for logging clarity
- Log unified environmental metrics in a single formatted line
- Continue operation even if one sensor read fails

This design ensures predictable sampling intervals and continuous operation.

---

## 5. Error Handling Strategy

### Initialization Errors
If any subsystem fails during startup:
- Error is logged
- Firmware aborts early
- Prevents undefined behavior

### Runtime Errors
If a sensor read fails:
- Warning is logged
- Loop continues
- System remains operational

This approach prioritizes long-term uptime and graceful degradation.

---

## 6. Data Flow Overview
`
    app_main()
        ↓
    Initialize I2C Bus
        ↓
    Initialize BME680
        ↓
    Initialize CCS811
        ↓
    Stabilization Delay
        ↓
    Periodic Measurement Loop
        ↓
    Read BME680 + CCS811 Data
        ↓
    Log Environmental Metrics
        ↓
    Wait 2 Minutes (No Drift)
`

---

## 7. Module Responsibilities Summary

### Provides
- Central application control
- Sensor initialization sequencing
- Periodic measurement scheduling
- Unified logging of environmental data

### Used By
- All sensor drivers
- I2C bus module
- Any future modules requiring periodic execution

---

## 8. External References

- ESP-IDF FreeRTOS API  
- ESP-IDF Logging API  
- Bosch BME680 documentation  
- ScioSense CCS811 documentation

---

## 9. Summary

The firmware ties together the entire sensor system, providing a clean, predictable, and robust execution loop.  
It ensures that both sensors are initialized correctly, read at stable intervals, and logged in a unified format.  
This architecture supports long-term environmental monitoring with consistent timing and graceful error handling.
