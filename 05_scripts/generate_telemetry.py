import csv
import math
import random
from datetime import datetime, timedelta

# ~1 GB target: adjust this if needed
TARGET_BYTES = 1_000_000_000

start_time = datetime(2026, 7, 26, 11, 15)
interval = timedelta(minutes=2)

def seasonal_temp(day_of_year, seconds_in_day):
    T_avg = 12.0
    A_season = 15.0
    season = T_avg + A_season * math.sin(2 * math.pi * day_of_year / 365.0)
    A_daily = 5.0
    daily = A_daily * math.sin(2 * math.pi * seconds_in_day / (24 * 3600))
    noise = random.uniform(-0.3, 0.3)
    return season + daily + noise

def humidity(day_of_year, seconds_in_day):
    if 80 <= day_of_year <= 260:  # warmer months
        base = 60
    else:
        base = 40
    daily = 5 * math.sin(2 * math.pi * seconds_in_day / (24 * 3600))
    noise = random.uniform(-1, 1)
    hum = base + daily + noise
    return max(10, min(100, hum))

def pressure(day_of_year, seconds_in_day):
    base = 1013.0
    daily = 1.0 * math.sin(2 * math.pi * seconds_in_day / (24 * 3600))
    weather = 5.0 * math.sin(2 * math.pi * day_of_year / 30.0)
    noise = random.uniform(-0.2, 0.2)
    return base + daily + weather + noise

def gas_values(temp, hum, pres):
    gas1 = 40 + (hum - 50) * 0.3 + random.uniform(-3, 3)
    gas2 = 400 + (temp - 20) * 5 + random.uniform(-10, 10)
    gas3 = 420 + (hum - 50) * 0.5 + random.uniform(-10, 10)
    return max(0, gas1), max(0, gas2), max(0, gas3)

def simulate_row(current_time):
    day_of_year = current_time.timetuple().tm_yday
    seconds_in_day = current_time.hour * 3600 + current_time.minute * 60 + current_time.second

    temp = seasonal_temp(day_of_year, seconds_in_day)
    hum = humidity(day_of_year, seconds_in_day)
    pres = pressure(day_of_year, seconds_in_day)
    gas1, gas2, gas3 = gas_values(temp, hum, pres)

    timestamp = current_time.strftime("%Y-%m-%dT%H:%M:%SZ")
    return [
        timestamp,
        round(temp, 1),
        round(hum, 1),
        round(pres, 1),
        int(round(gas1)),
        int(round(gas2)),
        int(round(gas3)),
    ]

def main():
    filename = "telemetry_1GB.csv"
    current_time = start_time
    total_bytes = 0

    with open(filename, "w", newline="") as f:
        while total_bytes < TARGET_BYTES:
            row = simulate_row(current_time)
            line = ",".join(map(str, row)) + "\n"
            f.write(line)
            total_bytes += len(line)
            current_time += interval

    print(f"Finished: {filename}, approx size: {total_bytes / (1024 * 1024):.2f} MB")

if __name__ == "__main__":
    main()