# Climate-Based Monitoring Device — Host Tool Analysis (Anomalous Sample)

**Input file:** `../../03_data/output_data/output.bin`  
**Samples processed:** 10  
**Time range:** 18.00 minutes

---

## Summary Statistics

**Temperature (°C)**  
- Min: -10.00  
- Max: 200.00  
- Average: 62.86

**Humidity (%)**  
- Min: 0.50  
- Max: 130.00  
- Average: 44.83

**Pressure (hPa)**  
- Min: 870.00  
- Max: 1200.00  
- Average: 991.26

**Air Quality Index (AQI)**  
- Min: 5.00  
- Max: 999.00  
- Average: 321.30

**CO₂ (ppm)**  
- Min: 150.00  
- Max: 10000.00  
- Average: 3012.40

**VOC (ppb)**  
- Min: 2.00  
- Max: 20000.00  
- Average: 6463.60

---

## Trend Detection

**Change over 18.00 minutes:**  
- **Temperature:** Falling (Δ = -0.50 °C)  
- **Humidity:** Increasing (Δ = +1.40 %)  
- **Pressure:** Rising (Δ = +1.30 hPa)  
- **AQI:** Decreasing (Δ = -5.00)  
- **CO₂:** Increasing (Δ = +9.00 ppm)  
- **VOC:** Increasing (Δ = +9.00 ppb)

---

## Overall Assessment

**Weather Interpretation**  
- Atmospheric pressure rising suggests stabilizing conditions in general, but some pressure readings are outside plausible ranges and require validation.  
- Temperature readings include values outside realistic environmental bounds; treat as suspect until sensor and wiring are checked.  
- Humidity shows extreme values in some samples; verify sensor calibration.

**Air Quality and Pollution**  
- Several AQI, CO₂, and VOC values are extremely high and likely indicate sensor saturation, miscalibration, or a real severe pollution event. Treat as high-priority for verification.

**Comfort Assessment**  
- Many temperature and humidity values fall outside normal comfort ranges; do not use these values for control decisions until validated.

**Environmental Stress Indicators**  
- Elevated AQI, CO₂, and VOC readings indicate potential health risk if validated. Immediate investigation recommended.

---

## Anomaly Detection

**Hard anomalies (sensor plausibility checks)**  
- Impossible temperature at sample 1: 85.00 °C  
- Impossible AQI at sample 2: 600.00  
- Impossible temperature at sample 3: 200.00 °C  
- Impossible humidity at sample 3: 130.00 %  
- Impossible pressure at sample 3: 1200.00 hPa  
- Impossible CO₂ at sample 3: 200.00 ppm  
- Impossible AQI at sample 4: 999.00  
- Impossible temperature at sample 6: 70.00 °C  
- Impossible pressure at sample 7: 1100.00 hPa  
- Impossible CO₂ at sample 7: 150.00 ppm  
- Impossible temperature at sample 8: 150.00 °C  
- Impossible AQI at sample 8: 700.00

**Environmental anomalies (high/low environmental indicators)**  
- High AQI at sample 1: 300.00  
- Elevated VOC at sample 1: 9000.00 ppb  
- High CO₂ at sample 1: 2500.00 ppm  
- Low pressure at sample 1: 950.00 hPa — storm conditions possible  
- High AQI at sample 2: 600.00  
- Elevated VOC at sample 2: 12000.00 ppb  
- High CO₂ at sample 2: 5000.00 ppm  
- Low pressure at sample 2: 870.00 hPa — storm conditions possible  
- High AQI at sample 4: 999.00  
- Elevated VOC at sample 4: 15000.00 ppb  
- High CO₂ at sample 4: 10000.00 ppm  
- High AQI at sample 6: 450.00  
- Elevated VOC at sample 6: 8000.00 ppb  
- High CO₂ at sample 6: 3000.00 ppm  
- Low pressure at sample 6: 890.00 hPa — storm conditions possible  
- Elevated VOC at sample 7: 500.00 ppb  
- High AQI at sample 8: 700.00  
- Elevated VOC at sample 8: 20000.00 ppb  
- High CO₂ at sample 8: 8000.00 ppm  
- Low pressure at sample 8: 880.00 hPa — storm conditions possible

**Trend anomalies (sudden jumps between consecutive samples)**  
- Temperature jump at sample 1: Δ = 60.20 °C  
- Humidity jump at sample 1: Δ = -31.20 %  
- Pressure jump at sample 1: Δ = -63.50 hPa  
- AQI jump at sample 1: Δ = 251.00  
- CO₂ jump at sample 1: Δ = 2080.00 ppm  
- VOC jump at sample 1: Δ = 8960.00 ppb  
- (additional trend jumps listed per sample)

![Trend and anomaly plots showing temperature, humidity, pressure, AQI, CO₂, and VOC over 18 minutes](..\images\anomalous_sample_plot.png)

*Figure 1 — Trend plot: six stacked time series (Temperature, Humidity, Pressure, AQI, CO₂, VOC) for the sample input (18.00 minutes).*
