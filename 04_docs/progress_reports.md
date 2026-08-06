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
- "Host Tool Showcase" section [x]
  - This is the examples folder in 02_host_tool
- Add a CLI flag parser [x]
  - Route to the build folder.
  - To run only anomalies: ```bash ninja run_anomalies``` or ```bash ./02_host_tool/pipeline.exe ../03_data/output_data/output.bin ../03_data/output_data/analysis_output.txt --anomalies```
    - These results are outputed in the normal 03_data/output_data folder
  - To run only statistics: ```bash ninja run_stats``` or ```bash ./02_host_tool/pipeline.exe ../03_data/output_data/output.bin ../03_data/output_data/analysis_output.txt --stats ```
    - These results are outputed in the normal 03_data/output_data folder
  - To run both: ```bash ninja run``` or ```bash ./02_host_tool/pipeline.exe ../03_data/output_data/output.bin ../03_data/output_data/analysis_output.txt``` or ```bash ./..05_scripts/run_pipeline.sh```
    - These results are outputed in the normal ```bash 03_data/output_data folder```
# Day 6
## Tasks Completed
- Add unit tests (Unity) [x]
- Add pipeline tests [x]
  - To run this pipeline tests, you need the MINW64 bash terminal.
  - Route to the build folder.
  - To run the test, do ```bash ninja run_tests``` or ```bash ./../05_scripts/run_tests.sh```
    - These results are outputed in the ```bash 02_host_tool\tests\output_data``` folder
- Add host‑side performance benchmark (parse speed, memory use) [x]
  - To run the benchmark, you need the MINW64 bash terminal.
  - Route to the build folder.
  - Benchmark by running: ```bash ninja run_benchmark``` or ```bash ./02_host_tool/pipeline.exe ../03_data/output_data/output.bin ../03_data/output_data/analysis_output.txt --benchmark```
- Add ASCII sparkline visualization  [x]
# Day 7
## Tasks completed
- Altered some of the logic to better reflect the goals of the project [x]
- Cleaned up the unity unit tests and got it working properly [x]
- Made color visualization in the console [x]
- Generally, today was just polishing features [x]
- The host tool is now 100% complete [x]
# Day 8-12
## Tasks completed
- Set up firmware structure [x]
- Analyzed the previous code to ensure reliability [x]
- Added a generate_telemetry.py script for stress testing the host tool. The efficiency of the host tool was discovered to be around 1.7 seconds for about 80 years of data or around 900 MBs of telemetry that was analyzed. Funnily enough, the analysis document was around 580 MB. [x]
- These past few days have been less productive since: 1. I do not currently have the hardware to test the code and 2. the API for the sensors and device itself has been difficult to fully understand. [x]