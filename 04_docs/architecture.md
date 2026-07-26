# Description of file design
## Firmware
The firmware folder will house the code used for the ESP32 board itself, such as the sensor drivers, logging, and main C file
## Host tool
The host tool folder will house the code used for trend analysis, anomaly detection, and other troubleshooting/analysis programs that would prove useful.
## Data
The data folder will house the data collected from the ESP32 board and the host tool.
## Docs
This folder will house all documentation collected during the making of this project, including hardware setup (wiring diagrams and the like), firmware overview (descriptions of each program on the ESP32 and their uses), analysis pipeline (how data flows through the host tool), architecture (a collection of high-level overviews of certain aspects of the project).
## Scripts
This folder will house the automation helper I will be using throughout development, including a script to flash the firmware and a script that converts raw logs into CSV.
# Acceptance criteria
## Software
The code complies cleanly.
All relevant tests are passed, including host tests.
A climate log measuring at least one real sensor collected from the ESP32 system.
## Hardware
The demo runs from flashing to results
## Documentation
Documentation describing climate relevance (why these sensors were chosen, what the system provides, how long term monitoring helps understand local climate patters) and engineering tradeoffs (Why the ESP32 was chosen, why low-power monitoring was important, why CSV was chosen for storage format).
