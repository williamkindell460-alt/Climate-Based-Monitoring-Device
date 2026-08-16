# main.c Architecture Overview

This document outlines the architecture, responsibilities, and runtime flow of the `main.c` module.  
It serves as the top‑level orchestrator for initializing the I2C bus, configuring sensors, and performing periodic environmental measurements.

---

## 1. File Purpose

`main.c` acts as the **application entry point** for the ESP32 firmware.  
Its responsibilities include:

- Initializing the I2C bus  
- Initializing all connected sensors  
- Running a periodic measurement loop  
- Logging sensor data  
- Handling errors gracefully  

---

## 2. Included Modules

The file integrates several subsystems:

- **FreeRTOS**  
  - Task scheduling  
  - Timing (`vTaskDelay`, `vTaskDelayUntil`)  

- **ESP‑IDF Core**  
  - Logging (`esp_log.h`)  
  - System utilities (`esp_system.h`)  

- **I2C Bus Module**  
  - `i2c_master_init()`  
  - Provides shared bus for all sensors  

- **Sensor Drivers**  
  - `bme680.h`  
  - `ccs811.h`  

---

## 3. Application Initialization Flow

### `app_main()`

The initialization sequence is:

1. **Initialize I2C bus**
   - Calls `i2c_master_init()`
   - Logs and aborts on failure

2. **Initialize BME680**
   - Calls `bme680_init()`
   - Validates chip ID  
   - Loads calibration  
   - Applies configuration  
   - Aborts on failure

3. **Initialize CCS811**
   - Calls `ccs811_init()`
   - Ensures sensor is in application mode  
   - Configures measurement mode  
   - Aborts on failure

4. **Stabilization delay**
   - `vTaskDelay(100 ms)`  
   - Allows sensors to warm up before first read

---

## 4. Main Measurement Loop

The loop runs every **2 minutes** using `vTaskDelayUntil()` for precise timing.

### Responsibilities

- Read BME680 data:
  - Temperature  
  - Humidity  
  - Pressure  
  - Gas resistance  

- Read CCS811 data:
  - eCO2
  - TVOC  

- Handle read errors gracefully:
  - Log warnings  
  - Continue loop  

- Log combined sensor output:
- `T=xx°C H=xx% P=xx hPa Gas=xxΩ eCO2=xx ppm TVOC=xx ppb`


### NAN Handling

If any BME680 value is `NAN`, the code substitutes `0.0f` for logging clarity.

---

## 5. Timing Architecture

### Tick Management

- `TickType_t last_wake = xTaskGetTickCount();`
- `vTaskDelayUntil(&last_wake, period);`

This ensures:
- **Stable 2‑minute intervals**
- No drift over time  
- Predictable sampling cadence  

---

## 6. Error Handling Strategy

### Initialization Errors
- Any failure in I2C, BME680, or CCS811 initialization results in:
- Error log  
- Immediate return from `app_main()`  

### Runtime Errors
- Sensor read failures:
- Logged as warnings  
- Loop continues  
- Prevents system lockup  

This design ensures robustness in long‑running deployments.

---

## 7. Data Flow Diagram

+------------------+
|    app_main()    |
+------------------+
|
v
+-----------------------------+
|   Initialize I2C bus        |
+-----------------------------+
|
v
+-----------------------------+
|   Initialize BME680         |
+-----------------------------+
|
v
+-----------------------------+
|   Initialize CCS811         |
+-----------------------------+
|
v
+-----------------------------+
|   Stabilization Delay       |
+-----------------------------+
|
v
+-----------------------------+
|   Periodic Measurement Loop |
+-----------------------------+
|
v
+-----------------------------+
| Read BME680 + CCS811 data   |
+-----------------------------+
|
v
+-----------------------------+
| Log environmental metrics   |
+-----------------------------+
|
v
+-----------------------------+
| Wait 2 minutes (no drift)   |
+-----------------------------+


---

## 8. Module Responsibilities Summary

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

## 9. External References

- ESP‑IDF FreeRTOS API  
- ESP‑IDF Logging API  
- Bosch BME680 documentation  
- ScioSense CCS811 documentation  

---

## 10. Summary

`main.c` ties together the entire sensor system, providing a clean, predictable, and robust execution loop.  
It ensures that both sensors are initialized correctly, read at stable intervals, and logged in a unified format.