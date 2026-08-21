# Climate Monitoring System – Full Project Architecture Overview

This document provides a complete, top‑level overview of the Climate Monitoring System,  
covering both the **ESP32 firmware** and the **host‑side analysis pipeline**.  
It also describes the repository structure and acceptance criteria for the project.

The system is designed for long‑term environmental monitoring, robust data collection,  
and automated host‑side analysis.

---

# 1. System Purpose

The Climate Monitoring System collects environmental data using onboard sensors,  
stores readings on the ESP32, and processes them on a host machine to produce:

- Basic statistics  
- Trend detection  
- Anomaly detection  
- Multi‑channel time‑series plots  
- A human‑readable environmental summary  

The architecture emphasizes modularity, reliability, and extensibility.

---

# 2. High‑Level Architecture

The project consists of two major subsystems:

## 2.1 Embedded Firmware (ESP32)
Responsible for:
- Initializing sensors  
- Performing periodic measurements  
- Logging environmental data  
- Exporting CSV logs for host‑side processing  

## 2.2 Host‑Side Analysis Pipeline (Python)
Responsible for:
- Ingesting CSV logs  
- Converting data into structured binary  
- Performing statistical and anomaly analysis  
- Generating synchronized plots  
- Producing a complete summary report  

These subsystems communicate through a shared CSV → binary → analysis → plot → summary flow.

---

# 3. Repository Structure

## 3.1 Firmware
Contains all code running on the ESP32, including:
- Sensor drivers (BME680, CCS811)
- I2C bus initialization
- Logging utilities
- `main.c` (application entry point)

## 3.2 Host Tool
Contains all Python modules used for:
- Trend analysis  
- Anomaly detection  
- Plot generation  
- Summary report creation  
- Test harnesses and pipeline automation

## 3.3 Data
Stores:
- Raw CSV logs from the ESP32  
- Binary files (`output.bin`)  
- Generated plots  
- Analysis summaries  

## 3.4 Docs
Contains all documentation:
- Hardware setup (wiring diagrams)  
- Firmware overview  
- Analysis pipeline overview  
- Architecture documents  
- Climate relevance and engineering tradeoffs  

## 3.5 Scripts
Automation helpers:
- Firmware flashing script  
- CSV conversion script  
- Development utilities  

---

# 4. Embedded Firmware Architecture (ESP32)

## 4.1 Firmware Responsibilities

The firmware:

- Initializes the I2C bus  
- Configures and validates sensors  
- Executes a stable, drift‑free measurement loop  
- Collects temperature, humidity, pressure, gas resistance, eCO₂, and TVOC  
- Logs unified environmental metrics  
- Handles initialization and runtime errors gracefully  

## 4.2 Initialization Sequence

1. Initialize I2C bus  
2. Initialize BME680  
3. Initialize CCS811  
4. 100 ms stabilization delay  

All failures abort early to prevent undefined behavior.

## 4.3 Measurement Loop

Runs every **2 minutes** using:
`
    TickType_t last_wake = xTaskGetTickCount();
    vTaskDelayUntil(&last_wake, period);
`

Loop tasks:
- Read BME680 values  
- Read CCS811 values  
- Replace NAN values with 0.0f  
- Log unified metrics  
- Continue even if one sensor read fails  

## 4.4 Error Handling

- Initialization errors → abort  
- Runtime errors → warn + continue  

This ensures long‑term uptime and graceful degradation.

---

# 5. Host‑Side Analysis Pipeline Architecture

The host tool processes raw sensor data into a complete analysis package.

Pipeline stages (from **analysis_pipeline.md**):

## 5.1 Sensor Data Ingestion

- CSV discovery  
- CSV parsing  
- Value extraction  
- Binary export (`output.bin`)  

Output: normalized binary stream of sensor readings.

## 5.2 Data Analysis

- Basic statistics  
- Trend detection  
- Anomaly detection  

Output: structured analysis file with all computed metrics.

## 5.3 Plot Generation

- CSV loading  
- Data normalization  
- Multi‑channel plot rendering  
- Timestamped PNG export  

Output: synchronized six‑channel time‑series plot.

## 5.4 Summary Output

- Summary statistics  
- Anomaly results  
- Trend overview  
- Environmental interpretation  
- Pipeline status + artifact paths  

Output: human‑readable summary report.

---

# 6. End‑to‑End Data Flow
`
ESP32 Firmware
    |
    v
Periodic Sensor Measurements
    |
    v
CSV Log Generation
    |
    v
Host Tool: Sensor Data Ingestion
    |
    v
output.bin (structured binary)
    |
    v
Host Tool: Data Analysis
    |
    v
Computed statistics, trends, anomalies
    |
    v
Host Tool: Plot Generation
    |
    v
Timestamped PNG plot
    |
    v
Host Tool: Summary Output
    |
    v
Final human-readable analysis package
`
This flow ensures raw device data becomes a complete environmental assessment.

---

# 7. Acceptance Criteria

## 7.1 Software
- Code compiles cleanly  
- All relevant tests pass  
- Host tool successfully processes at least one real sensor log  

## 7.2 Hardware
- Full demo runs from flashing → data collection → host analysis → final results  

## 7.3 Documentation
Includes:
- Climate relevance  
- Engineering tradeoffs  
- Sensor selection rationale  
- ESP32 justification  
- CSV format justification  
- Firmware overview  
- Host‑side pipeline overview  
- Architecture documentation  

---

# 8. Summary

The Climate Monitoring System integrates a robust ESP32 firmware with a modular host‑side analysis pipeline.  
The firmware ensures stable, long‑term environmental data collection, while the host tool transforms raw logs  
into meaningful insights through statistics, trends, anomalies, plots, and summaries.

This architecture supports reliable climate monitoring, extensible analysis, and clear documentation for future development.
