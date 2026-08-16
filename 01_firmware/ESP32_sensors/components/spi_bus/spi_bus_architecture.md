# SPI Bus Architecture Overview

This module provides the hardware‑level SPI interface and filesystem mounting required to use an SD card with the ESP32. It is responsible for configuring the SPI bus, attaching the SD card via the SDSPI host driver, and exposing the card’s FAT filesystem at the mount point:

`/sdcard`

The SD logger module (`sd_logger`) builds on top of this layer to write CSV data.

---

## 1. Module Responsibilities

### Provides
- SPI bus initialization for SD card communication  
- SDSPI host configuration (CS pin, host ID)  
- FAT filesystem mounting via `esp_vfs_fat_sdspi_mount()`  
- Automatic creation of the `/sdcard/real_output/` directory  
- Clean unmounting and SPI bus teardown  

### Used By
- `sd_logger` (for CSV creation and append operations)  
- `main.c` (to initialize SD card access before logging begins)  

---

## 2. Hardware Interface

The HiLetgo Micro SD module uses a standard 4‑wire SPI interface:

| SD Module Pin | ESP32 Pin |
|---------------|-----------|
| MISO          | GPIO 19   |
| MOSI          | GPIO 23   |
| CLK           | GPIO 18   |
| CS            | GPIO 5    |

These pins are defined in the module:

```c
#define SD_MISO   19
#define SD_MOSI   23
#define SD_CLK    18
#define SD_CS     5
```
The ESP32 communicates with the SD card using the **SDSPI host**, a lightweight SPI‑based SD/MMC driver provided by ESP‑IDF. This module configures the SPI bus, mounts the SD card’s FAT filesystem, and exposes it at:

`/sdcard`

---

## 3. Initialization Layer

### `sd_bus_init()`

This function prepares the SD card for use by the rest of the system.

### Responsibilities

#### **Configure SPI bus**
- Sets MOSI, MISO, and CLK pins  
- Allocates DMA resources  
- Calls `spi_bus_initialize()` to activate the SPI peripheral  

#### **Configure SD card slot**
- Sets the CS (chip select) pin  
- Assigns SDSPI host ID  
- Uses `sdspi_device_config_t` to bind the SD card to the SPI bus  

#### **Mount FAT filesystem**
- Calls `esp_vfs_fat_sdspi_mount()`  
- Exposes SD card contents at `/sdcard`  
- Enables standard C file operations (`fopen`, `fprintf`, `fclose`, etc.)  

#### **Create output directory**
- Ensures `/sdcard/real_output/` exists  
- Required by the `sd_logger` module for CSV storage  

### Error Handling
- **SPI bus initialization failure** → return error  
- **SD card mount failure** → free SPI bus and return error  
- **Directory creation failure** → logged but not fatal  

---

## 4. Deinitialization Layer

### `sd_bus_deinit()`

This function safely shuts down SD card access.

### Responsibilities
- Unmount FAT filesystem  
- Free SDSPI host resources  
- Free SPI bus  

### Error Handling
- Unmount failures are logged but not fatal  
- SPI bus is always freed to prevent resource leaks  

---

## 5. Data Flow Diagram

+------------------------+
|      app_main()        |
+------------------------+
            |
            v
+----------------------------------+
|          sd_bus_init()           |
+----------------------------------+
            |
            v
+----------------------------------+
| SD card mounted at /sdcard       |
+----------------------------------+
            |
            v
+----------------------------------+
| sd_logger_init() creates CSV     |
+----------------------------------+
            |
            v
+----------------------------------+
| sd_logger_append() writes rows   |
+----------------------------------+

---

## 6. Public API Summary

### Header (`spi_bus.h`)

```c
esp_err_t sd_bus_init(void);
esp_err_t sd_bus_deinit(void);
```
## 7. External References
- ESP‑IDF SDSPI Host Driver
- ESP‑IDF FATFS VFS Layer
- HiLetgo Micro SD SPI Module Documentation
- ESP32 Technical Reference Manual (SPI Peripheral)