# Day 1
## Tasks Completed
- Created the github repository and README with goals, impact, and a parts list [x]
- Added a project roadmap and acceptable criteria [x]
- Created a structured repository (firmware/, host/, docs/, examples/) [x]
- Added CI skeleton (clang format, cppcheck, warnings as errors) [x]
- Added a sample CSV [x]
# Day 2
## Tasks Completed
- placeholder plots [x]
- Implement the CSV parser [x]
- Add error handling for corrupted logs [x]
# Day 3
## Tasks Completed
- I missed today due to work
# Day 4
## Tasks Completed
- Completed trend analysis [x]
- Completed analyzer integration [x]
- Completed anomaly detection [x]
- Added CMake integration [x]
- Multi-module build system [x]
- Ninja build + run pipeline [x]
- Host tool fully integrated with firmware [x]
- End-to-end analysis pipeline operational [x]
- Toolchain debugging + MSYS2 environment setup [x]
- Host tool architecture stabilized [x]
- Project structure elevated to professional quality [x]
# Day 5
## Tasks Completed
- Example outputs and plots [x]
- - a real trend plot
- - a real anomaly plot
- "Host Tool Showcase" section [x]
- - sample input
- - sample output
- - anomaly explanation
- - screenshot of plot
- - short explanation of algorithm 
- Add a CLI flag parser [x]
- - a --stats and --anomalies
# Day 6
## Tasks Completed
- Add pipeline tests  
  - Create end‑to‑end tests that feed sample CSV logs into the host pipeline  
  - Validate trend detection, anomaly detection, and CLI flag behavior (`--stats`, `--anomalies`)  
  - Add golden‑file comparisons for stable output verification  
- Add ASCII sparkline visualization 
  - Implement lightweight terminal sparklines using normalized values and block characters
  - Integrate sparklines into `--stats` mode for temperature, humidity, pressure, AQI, CO₂, and VOC  
- Add host‑side performance benchmark (parse speed, memory use)
  - Add a `--benchmark` flag to measure CSV parsing throughput, trend/anomaly compute time, and memory footprint  
  - Print structured benchmark results for reproducibility  
- Add unit tests (Unity)
  - Expand pure‑logic tests for trend computation, anomaly scoring, min/max helpers, and environmental assessments  
  - Add a combined test runner and integrate with CI  