# SD Logging Architecture Overview

This module provides persistent CSV logging for environmental data collected from
the BME680 and CCS811 sensors. Logged data is stored in the folder:

`/sdcard/real_output/data.csv`

---

## 1. Module Responsibilities

### Provides
- SD card initialization (SPI or SDMMC)
- Filesystem mounting
- CSV file creation and append operations
- Timestamped environmental data logging
- Automatic directory creation (`real_output/`)
- Error reporting for card failures or write issues

### Used By
- `main.c` measurement loop
- Any future analytics or upload modules

---

## 2. CSV Format

Each row follows:
`timestamp,temperature_c,humidity_pct,pressure_hpa,air_quality_index,co2_ppm,voc_ppb`

Example:
`2026-08-15T16:17:00,24.51,48.22,1008.33,41,612,23`

---

## 3. Initialization Layer

### `sd_logger_init()`

**Responsibilities**
- Configure SD card interface (SPI or SDMMC)
- Mount FAT filesystem
- Create `/sdcard/real_output/` directory if missing
- Create CSV file if it does not exist
- Write header row if file is new

**Error Handling**
- Card missing → return error
- Mount failure → return error
- Directory creation failure → return error

---

## 4. Logging Layer

### `sd_logger_append()`

**Responsibilities**
- Generate timestamp string
- Compute air quality index (AQI) from BME680 gas resistance
- Format CSV row
- Append row to `data.csv`
- Flush file to ensure persistence

**Error Handling**
- If write fails → log warning
- If card removed → return error

---

## 5. Data Flow Diagram

+------------------------+
|      app_main()        |
+------------------------+
|
v
+----------------------------------+
|   sd_logger_init()               |
+----------------------------------+
|
v
+----------------------------------+
|   Periodic Measurement Loop      |
+----------------------------------+
|
v
+----------------------------------+
| Read BME680 + CCS811 data        |
+----------------------------------+
|
v
+----------------------------------+
| sd_logger_append()               |
+----------------------------------+
|
v
+----------------------------------+
| CSV row written to SD card       |
+----------------------------------+

---

## 6. Public API Summary

### Header (`sd_logger.h`)
```c
esp_err_t sd_logger_init(void);
esp_err_t sd_logger_append(float temp, float hum, float press,
                           float gas_res, uint16_t eco2, uint16_t tvoc);
```
## 7. External References
- ESP‑IDF SDMMC documentation
- ESP‑IDF FATFS documentation
- Bosch BME680 datasheet
- ScioSense CCS811 datasheet