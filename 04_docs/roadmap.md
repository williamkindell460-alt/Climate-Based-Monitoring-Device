# First week
- Create the GitHub repository and the README with goals, impact, and a parts list
    [x]
- Add the project roadmap and acceptance criteria
    [x]
- Scaffold the repository
    [x]
- add CI skeleton
    [x]
- add a sample CSV + placeholder plots 
    [x]
- Implement the CSV parser 
    [x]
- Implement trend analysis functions (temp/humidity/VOC) 
    [x]
- Implement anomaly detection (heat spikes, VOC surges) 
    [x]
- Finalize host MVP 
    [x]
- Add example outputs + plots 
    [x]
- Add host-side CLI flags (-plot, -stats, -anomalies) 
    [x]
- Add documentation for host tool 
    [x]
- Add pipeline tests 
    [x]
- Add host-side performance benchmark (parse speed, memory use) 
    [x]
- Add unit tests (Unity) 
    [x]
- Add ASCII sparkline visualization 
    [x]
- **Host tool completion** [x]

# Second week
- Add error handling for corrupted logs
- Scaffold firmware core (drivers, sleep cycle, error codes)
- Wire sensors (BME680 + CCS811)
- Validate raw readings
- Add design notes (sensor placement, airflow)
- Implement SD logging
- Capture first real climate log
- Add SD write failure recovery
- Add UART telemetry stream (live sensor data)
- Add packet framing + CRC
- Add smoothing filter (moving average / exponential)
- Add sensor self-test mode
- Add sleep/wake cycle (light sleep)
- Add low-power benchmark plan
- Measure sleep current + active current
- Estimate battery life
- Document power budget
- **Add simple Wi‑Fi log transfer (ESP32 HTTP endpoint serving /latest.csv)**
- **Firmware completion**
- **Hardware completion**

# Third week
- Add CONTRIBUTING + LICENSE
- Add schematic + hardware photos
- Add wiring diagram
- Firmware architecture cleanup (modules, drivers, HAL separation)
- Add watchdog timer + recovery
- Improve code style & comments
- Add clang format + clang tidy CI gates
- Add design notes (sampling rate, power budget, noise sources)
- Add hardware stress tests (thermal drift, noise, airflow)
- Add documentation for failure modes
- Add real world comparison: NOAA data vs your logs
- **Software completion (host + firmware integrated)**

# Fourth week
- Add host-side visualization polish (PNG plots)
- Write technical summary (design, algorithms, power, telemetry)
- Add “Lessons Learned” section
- Add “Future Work” section (cloud upload, enclosure, Kalman filter)
- Optional stretch goal: BH1750 light sensor OR Kalman filter OR MQTT cloud upload
- Final documentation pass
- Add engineering tradeoff discussion
- Resume optimization pass
- Add project bullets
- Final GitHub cleanup
- Tag release candidate
- Final polish + roadmap planning
- **Documentation completion**