# BME680 Driver Architecture Overview

This document describes the architecture, responsibilities, and data flow of the `bme680.c` driver implementation.  
It is intended as a high‑level reference for maintainers, contributors, and anyone integrating the sensor into an ESP32 project.

---

## 1. File Structure Overview

The driver is organized into the following functional layers:

1. **Initialization Layer**
   - `bme680_init()`
   - Chip ID validation
   - Calibration coefficient loading
   - Sensor configuration (oversampling, IIR filter, heater profile)

2. **Calibration Data Layer**
   - `bme680_read_calibration()`
   - Reads two blocks of factory calibration coefficients
   - Populates `bme680_calib_data_t`

3. **Measurement Control Layer**
   - `bme680_trigger_measurement()`
   - `bme680_is_measuring()`
   - `bme680_new_data_ready()`

4. **Raw Data Acquisition Layer**
   - `bme680_read_raw()`
   - Reads raw temperature, pressure, humidity, gas resistance, and gas range

5. **Compensation Layer**
   - `bme680_compensate_temperature()`
   - `bme680_compensate_pressure()`
   - `bme680_compensate_humidity()`
   - `bme680_compensate_gas()`

6. **Public API**
   - `bme680_read()`
   - Returns fully compensated environmental data

---

## 2. Initialization Layer

### `bme680_init()`
Responsibilities:
- Read chip ID (`0xD0`) and verify it equals `0x61`
- Load calibration coefficients:
  - Temp/pressure block (`0x89–0xA1`)
  - Humidity/gas block (`0xE1–0xF0`)
- Apply sensor configuration:
  - Humidity oversampling (8×)
  - Temperature oversampling (8×)
  - Pressure oversampling (8×)
  - Forced mode
  - IIR filter (coefficient = 7)
  - Heater temperature (300°C)
  - Heater duration (150 ms)
  - Gas sensor enable

---

## 3. Calibration Data Layer

### `bme680_calib_data_t`
Stores all factory calibration coefficients required for Bosch’s compensation formulas.

Calibration is read from two register blocks:

- **Block 1:** `0x89–0xA1`  
  Contains temperature and pressure coefficients.

- **Block 2:** `0xE1–0xF0`  
  Contains humidity, gas, heater range, heater value, and range switching error.

These values are decoded according to the datasheet tables.

---

## 4. Measurement Control Layer

### `bme680_trigger_measurement()`
Writes `CTRL_MEAS` (`0x74`) with:
- Temperature oversampling = 8×  
- Pressure oversampling = 8×  
- Mode = forced

### `bme680_is_measuring()`
Checks `MEAS_STATUS` (`0x1B`) bit 5.

### `bme680_new_data_ready()`
Checks `STATUS` (`0x1D`) bit 7.

---

## 5. Raw Data Acquisition Layer

### `bme680_read_raw()`
Reads raw sensor values from register block `0x1F–0x2E`:

- Pressure: 20‑bit  
- Temperature: 20‑bit  
- Humidity: 16‑bit  
- Gas resistance: 10‑bit  
- Gas range: 4‑bit

These raw values are stored in `bme680_raw_data_t`.

---

## 6. Compensation Layer

Bosch provides fixed‑point formulas to convert raw ADC values into physical units.

### Temperature
Uses `par_t1`, `par_t2`, `par_t3`  
Produces °C  
Stores intermediate `t_fine` used by pressure/humidity formulas.

### Pressure
Uses `par_p1`–`par_p10`  
Produces hPa

### Humidity
Uses `par_h1`–`par_h7`  
Produces %RH (clamped to 0–100%)

### Gas Resistance
Uses:
- `par_g1`, `par_g2`, `par_g3`
- `range_sw_err`
- `gas_range_table[]`

Produces resistance in ohms.

---

## 7. Public API

### `bme680_read(float *temperature, float *humidity, float *pressure, float *gas_resistance)`
High‑level function that:
1. Triggers a measurement  
2. Waits for measurement completion  
3. Waits for new data  
4. Reads raw values  
5. Applies compensation formulas  
6. Returns fully compensated values

This is the primary function used by application code.

---

## 8. Register Map Summary

### Key Registers
| Purpose | Register |
|--------|----------|
| Chip ID | `0xD0` |
| Status | `0x1D` |
| Measurement Status | `0x1B` |
| Raw Pressure MSB | `0x1F` |
| Raw Temperature MSB | `0x22` |
| Raw Humidity MSB | `0x25` |
| Raw Gas MSB | `0x2A` |
| Humidity Oversampling | `0x72` |
| Temp/Pressure Oversampling + Mode | `0x74` |
| IIR Filter | `0x75` |
| Gas Control | `0x71` |
| Heater Temperature | `0x5A` |
| Heater Duration | `0x64` |

---

## 9. Data Flow Diagram

+------------------+
|  bme680_init()   |
+------------------+
|
v
+-----------------------------+
| bme680_read_calibration()   |
+-----------------------------+
|
v
+-----------------------------+
|   bme680_configure()        |
+-----------------------------+
|
v
+-----------------------------+
|     bme680_read()           |
+-----------------------------+
|
v
+-----------------------------+
|   bme680_read_raw()         |
+-----------------------------+
|
v
+-----------------------------+
| Compensation Functions      |
+-----------------------------+
|
v
+-----------------------------+
|  Temperature, Humidity,     |
|  Pressure, Gas Resistance   |
+-----------------------------+

---

## 10. External References

- Bosch BME680 Datasheet  
- Pimoroni DeepWiki Register Map  
- Zanduino BME680 documentation  

---

## 11. Public Header Summary

The header defines:
- Sensor I2C address  
- Register map  
- Calibration block addresses  
- Raw data block addresses  
- Public API prototypes  
- Datasheet references