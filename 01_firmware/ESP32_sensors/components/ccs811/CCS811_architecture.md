# CCS811 Driver Architecture Overview

This document outlines the architecture, responsibilities, and data flow of the `ccs811.c` driver implementation.  
It serves as a high‑level reference for maintainers and anyone integrating the CCS811 gas sensor into an ESP32 project.

---

## 1. File Structure Overview

The driver is organized into the following functional layers:

1. **Initialization Layer**
   - `ccs811_init()`
   - Boot‑mode detection
   - Application start command
   - Measurement mode configuration

2. **Status & Error Layer**
   - Reading `STATUS` register
   - Checking FW mode, data readiness, and error flags
   - Reading `ERROR_ID` when needed

3. **Measurement Layer**
   - `ccs811_read()`
   - Polling for new data
   - Reading algorithm result block
   - Extracting eCO2 and TVOC values

4. **I2C Communication Layer**
   - Register read/write helpers via ESP‑IDF I2C API
   - Timeout handling
   - Error propagation

5. **Public API**
   - `ccs811_init()`
   - `ccs811_read()`

---

## 2. Initialization Layer

### `ccs811_init()`
Responsibilities:
- Read `STATUS` register (`0x00`)
- Check **FW_MODE** bit (`0x10`)
  - If FW_MODE = 0 → sensor is in **bootloader mode**
  - Send `APP_START` command (`0xF4`)
  - Poll until FW_MODE becomes 1 (application mode)
- Configure measurement mode:
  - Writes to `MEAS_MODE` (`0x01`)
  - Example: `0x10` → Drive Mode 1 (1‑second measurement interval)

This ensures the sensor transitions from bootloader → application mode and begins producing air‑quality data.

---

## 3. Status & Error Layer

### Status Bits (`STATUS`, register `0x00`)
| Bit | Meaning |
|-----|---------|
| `0x10` | FW_MODE (1 = application mode) |
| `0x08` | DATA_READY (new measurement available) |
| `0x01` | ERROR (error present) |

### Error Handling
If ERROR bit is set:
- Read `ERROR_ID` (`0xE0`)
- Log the specific error code
- Continue or fail depending on application needs

---

## 4. Measurement Layer

### `ccs811_read(uint16_t *eco2, uint16_t *tvoc)`
Steps:
1. Read `STATUS`
2. Check `DATA_READY`
   - If not ready → return `ESP_ERR_INVALID_STATE`
3. Read `ALG_RESULT_DATA` (`0x02`)
   - 4 bytes:
     - eCO2 MSB
     - eCO2 LSB
     - TVOC MSB
     - TVOC LSB
4. Combine bytes into 16‑bit values
5. Check ERROR bit and optionally read `ERROR_ID`

This function returns:
- **eCO2** (ppm)
- **TVOC** (ppb)

---

## 5. I2C Communication Layer

The driver uses ESP‑IDF’s high‑level I2C functions:

- `i2c_master_write_read_device()`
- `i2c_master_write_to_device()`

Features:
- 1‑byte register addressing
- 1–4 byte reads depending on register
- 1000 ms timeout
- Error propagation via `ESP_ERROR_CHECK` or return codes

This layer abstracts raw I2C transactions from sensor logic.

---

## 6. Public API

### `esp_err_t ccs811_init(void)`
Initializes the sensor, transitions to application mode, and configures measurement mode.

### `esp_err_t ccs811_read(uint16_t *eco2, uint16_t *tvoc)`
Reads compensated air‑quality values from the sensor.

These two functions form the complete public interface for the CCS811 driver.

---

## 7. Register Map Summary

### Key Registers
| Purpose | Register |
|---------|----------|
| Status | `0x00` |
| Measurement Mode | `0x01` |
| Algorithm Result Data | `0x02` |
| Error ID | `0xE0` |
| Application Start Command | `0xF4` |

### Algorithm Result Data Format (`0x02`)
| Byte | Meaning |
|------|---------|
| 0 | eCO2 MSB |
| 1 | eCO2 LSB |
| 2 | TVOC MSB |
| 3 | TVOC LSB |

---

## 8. Data Flow Diagram

+------------------+
|  ccs811_init()   |
+------------------+
|
v
+-----------------------------+
| Read STATUS register        |
+-----------------------------+
|
v
+-----------------------------+
| If FW_MODE=0 → APP_START    |
+-----------------------------+
|
v
+-----------------------------+
| Poll until FW_MODE=1        |
+-----------------------------+
|
v
+-----------------------------+
| Configure MEAS_MODE         |
+-----------------------------+
|
v
+-----------------------------+
|        ccs811_read()        |
+-----------------------------+
|
v
+-----------------------------+
| Check DATA_READY            |
+-----------------------------+
|
v
+-----------------------------+
| Read ALG_RESULT_DATA        |
+-----------------------------+
|
v
+-----------------------------+
| Return eCO2 and TVOC        |
+-----------------------------+


---

## 9. External References

- CCS811 Datasheet (AMS / ScioSense)  
- SparkFun CCS811 documentation  
- Bosch BME680 documentation (for combined air‑quality systems)

---

## 10. Public Header Summary

The header defines:
- Sensor I2C address  
- Register map  
- Status bits  
- Application start command  
- Public API prototypes  

This provides all constants needed for low‑level driver operation.