# I2C Bus Module Architecture Overview

This document outlines the architecture, responsibilities, and data flow of the `i2c_bus.c` module.  
It provides a clear reference for how the ESP32’s I²C controller is configured, initialized, and deinitialized for use by sensors such as the BME680 and CCS811.

---

## 1. File Structure Overview

The module is organized into two primary functional layers:

1. **Initialization Layer**
   - `i2c_master_init()`
   - Applies I²C configuration
   - Installs the I²C driver

2. **Deinitialization Layer**
   - `i2c_master_deinit()`
   - Safely removes the I²C driver

The header file defines:
- Pin assignments  
- Controller selection  
- Bus frequency  
- Public API prototypes  

---

## 2. Header Definitions

### Pin Configuration
- `I2C_MASTER_SCL_IO = 22`  
- `I2C_MASTER_SDA_IO = 21`  
These map directly to ESP‑WROOM‑32 GPIO pins.

### Controller Selection
- `I2C_MASTER_NUM = I2C_NUM_0`  
ESP32 has two controllers; controller 0 is used here.

### Bus Frequency
- `I2C_MASTER_FREQ_HZ = 100000`  
Standard‑mode I²C (100 kHz).

### Public API
`   esp_err_t i2c_master_init(void);
    esp_err_t i2c_master_deinit(void);
`

## 3. Initialization Layer

### `i2c_master_init()`

**Responsibilities**
- Populate an `i2c_config_t` structure:
  - `mode`: master
  - `sda_io_num`: SDA pin
  - `scl_io_num`: SCL pin
  - `sda_pullup_en`: enable internal pull‑up
  - `scl_pullup_en`: enable internal pull‑up
  - `master.clk_speed`: 100 kHz bus frequency
- Apply configuration using:
  - `i2c_param_config()`
- Install driver using:
  - `i2c_driver_install()`
  - RX/TX buffers disabled (master mode does not require them)
- Log initialization success

**Error Handling**
- If configuration fails → return error
- If driver installation fails → return error
- Logs include human‑readable error names


## 4. Deinitialization Layer

### `i2c_master_deinit()`

**Responsibilities**
- Call `i2c_driver_delete()`
- Handle cases where the driver is not installed (`ESP_ERR_INVALID_STATE`)
- Log successful deletion

This ensures clean shutdown and prevents resource leaks.


## 5. I²C Configuration Structure

### `i2c_config_t conf`

| Field              | Purpose                     |
|--------------------|-----------------------------|
| `mode`             | Master mode                 |
| `sda_io_num`       | SDA pin                     |
| `scl_io_num`       | SCL pin                     |
| `sda_pullup_en`    | Enable internal pull‑up     |
| `scl_pullup_en`    | Enable internal pull‑up     |
| `master.clk_speed` | Bus frequency               |

**Optional**
- `clk_flags` for advanced timing control (ESP‑IDF 4.4+)

## 6. Data Flow Diagram
          +------------------------+
          |   i2c_master_init()    |
          +------------------------+
                     |
                     v
     +----------------------------------+
     | Configure I2C parameters          |
     | (i2c_param_config)                |
     +----------------------------------+
                     |
                     v
     +----------------------------------+
     | Install I2C driver                |
     | (i2c_driver_install)              |
     +----------------------------------+
                     |
                     v
     +----------------------------------+
     | Sensors use I2C bus               |
     | (BME680, CCS811, etc.)            |
     +----------------------------------+
                     |
                     v
          +------------------------+
          |   i2c_master_deinit()  |
          +------------------------+
## 7. Module Responsibilities Summary

### Provides
- A stable, reusable I²C bus foundation  
- Clean initialization and teardown  
- Logging for debugging  
- Centralized pin and frequency definitions  

### Used By
- BME680 driver  
- CCS811 driver  
- Any additional I²C sensors or peripherals  


## 8. External References
- ESP‑IDF I²C API documentation  
- ESP‑WROOM‑32 datasheet  
- Bosch BME680 datasheet  
- ScioSense CCS811 datasheet  


## 9. Public Header Summary

The header defines:
- Pin assignments  
- Controller selection  
- Bus frequency  
- Public API prototypes  
- Documentation links for further reference  
