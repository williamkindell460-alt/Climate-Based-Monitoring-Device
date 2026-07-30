from datetime import datetime
import csv
import struct
import os
import glob

# Determine project root dynamically
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "../.."))

# Build portable paths
CSV_DIR = os.path.join(PROJECT_ROOT, "01_firmware", "real_output")
OUT_BIN = os.path.join(PROJECT_ROOT, "03_data", "output_data", "output.bin")

def load_csv(filename):
    with open(filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)
        rows = []
        for row in csvreader:
            rows.append(row)
    return fields, rows

def parse_timestamps(rows):
    timestamps = [datetime.strptime(row[0], "%Y-%m-%dT%H:%M:%SZ") for row in rows]
    start = timestamps[0]
    return [(t - start).total_seconds() / 60 for t in timestamps]

def parse_values(rows):
    temp = [float(row[1]) for row in rows]
    humid = [float(row[2]) for row in rows]
    pressure = [float(row[3]) for row in rows]
    air_qual = [float(row[4]) for row in rows]
    co2 = [float(row[5]) for row in rows]
    voc = [float(row[6]) for row in rows]
    return temp, humid, pressure, air_qual, co2, voc

def export_binary(filename, timestamps, temp, humid, pressure, air_qual, co2, voc):
    os.makedirs(os.path.dirname(filename), exist_ok=True)

    with open(filename, "wb") as f:
        for i in range(len(timestamps)):
            f.write(struct.pack(
                "7d",
                timestamps[i], temp[i], humid[i], pressure[i],
                air_qual[i], co2[i], voc[i]
            ))

print("============== Sensor Reader ==============")
print("[INFO] sensor_reader.py is running")
# Find newest CSV
csv_files = glob.glob(os.path.join(CSV_DIR, "*.csv"))
if not csv_files:
    print(f"[ERROR] No CSV files found in {CSV_DIR}")
    exit(1)

latest_csv = max(csv_files, key=os.path.getmtime)
print(f"Newest CSV detected: {latest_csv}")

fields, rows = load_csv(latest_csv)
time = parse_timestamps(rows)
temp, humid, pressure, air_qual, co2, voc = parse_values(rows)

export_binary(OUT_BIN, time, temp, humid, pressure, air_qual, co2, voc)

print("[OK] sensor_reader.py ran successfully")
