## Climate-Based-Monitoring-Device
This project develops a low power, ESP32 based climate monitoring system that collects temperature, humidity, pressure, and air quality data using integrated environmental sensors, logs these measurements for long term analysis, and processes them through a robust C based host tool to identify trends and anomalies. By combining embedded firmware, data analysis algorithms, and thorough documentation, the project demonstrates how accessible hardware and efficient software design can create meaningful, reliable insights into local climate conditions and environmental change.
## Parts List
- ESP32 dev board 
- BME680 (Temperature, Humidity, Pressure, VOC baseline)
- CCS811 air quality sensor (TVOC (Total Volatile Organic Compounds), eCO₂ (estimated CO₂))
- MicroSD module + microSD card
- Li ion/LiPo battery
- TP4056 charger
- Breadboard + jumper wires
- Optional: BH1750 light sensor (Ambient light intensity (lux)), perfboard, 3D printed enclosure
## Impact
This project is one born out of the current state of the climate, specifically the change it had gone through in recent years. While many credible sources have made the same conclusion regarding its change in average temperature, I wanted to make a device that attempts to find the reason for the warmth. Is it the CO2 in the air? Is it the humidity? Is it a change in pressure? What is causing this warmth?
## Requirements
### Required Programs
- **C compiler**
  - Linux: gcc or clang  
  - macOS: clang  
  - Windows: MinGW or MSVC  
- **CMake ≥ 3.15**
- **Python 3**  
  Automatically detected by CMake via:
  ```cmake
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
  add_compile_definitions(PYTHON_EXECUTABLE="${Python3_EXECUTABLE}")
### 1. Clone the Repository
- ```bash git clone https://github.com/williamkindell460-alt/Climate-Based-Monitoring-Device.git
  cd Climate-Based-Monitoring-Device

### 2. Configure with CMake
- Generate build files
- - ```bash 
cmake -B build -DCMAKE_BUILD_TYPE=Release
- CMake will:
- - Detect your Python 3 interpreter
- - Define PYTHON_EXECUTABLE automatically
- - Generate platform-specific build files
### 3. Build the project
- ```bash cmake --build build --config Release
- This compiles:
- -  The ESP32 firmware logic
- - The C-based host analysis tool
### 4. Run the Full Pipeline (`main`)
- The full pipeline is executed using Ninja:
- - ```bash cd build
ninja run
- This will:
- - Read CSV files from 01_firmware/real_output/
- - Generate output.bin
- - Analyze the binary data
- - produce analysis_output.txt
- - Generate plots
- - Print a summary of all stages
