# ESP32 + BME680 + CCS811 Hardware Checklist

## Power
- Stable **3.3 V** supply  
- Regulator capable of handling ESP32 + sensor peak current  
- Avoid noisy USB power sources  
- Common ground between ESP32 and sensors  

---

## Decoupling
- **0.1 µF ceramic capacitor** near each sensor VCC  
- **10 µF electrolytic** near sensor cluster  
- Short traces to capacitors  

## I2C Bus
- External pull‑ups: **4.7 kΩ – 10 kΩ** on SDA/SCL  
- Keep wires short and routed together  
- Avoid long untwisted wires  
- Optional **22–47 Ω** series resistors to reduce ringing  
- Bus speed: **100 kHz** (safe), **400 kHz** (short wires only)  


## Address Pins
- CCS811 ADDR → **0x5A** or **0x5B**
- BME680 SDO → **0x76** or **0x77**
- Confirm with an I2C scan  


## Sensor Placement

### BME680
- Keep away from heat sources  
- Avoid direct airflow  
- Provide ventilation for humidity and gas readings  
- Thermally isolate from PCB if possible  

### CCS811
- Requires **warm‑up (minutes)** for stable gas readings  
- Avoid contamination sources (solvents, aerosols)  
- Ensure stable airflow  


## Mechanical
- Solid solder joints or high‑quality connectors  
- No strain on sensor pins  
- Avoid flexing PCB near sensors  


## ESD & Protection
- ESD‑safe handling  
- Optional TVS diode if exposed to user contact  
- Avoid touching sensor openings  


## Testing Checklist
- Power up and wait **1–10 minutes**  
- Run I2C scan  
- Verify CCS811 **FW_MODE** and **DATA_READY**  
- Verify BME680 **chip ID = 0x61**  
- Test reads at **5–10 second interval**  
- Switch to **2‑minute interval** after validation  
- Log early drift (normal for gas sensors)