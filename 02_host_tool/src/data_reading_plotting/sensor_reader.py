from datetime import datetime
import csv
import struct
import os
import glob
import sys

COL_RESET      = "\033[0m"
COL_RED        = "\033[31m"
COL_GREEN      = "\033[32m"
COL_YELLOW     = "\033[33m"
COL_CYAN       = "\033[36m"
COL_LIGHT_RED  = "\033[91m"
COL_LIGHT_BLUE = "\033[94m"

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "../../.."))

TEST_DIR = os.getenv("CBMD_TEST_DATA_DIR")
TEST_OUT_DIR = os.getenv("CBMD_TEST_OUTPUT_DIR")

DEFAULT_CSV_DIR = os.path.join(PROJECT_ROOT, "01_firmware", "real_output")
CSV_DIR = TEST_DIR if TEST_DIR else DEFAULT_CSV_DIR

DEFAULT_OUT_BIN = os.path.join(PROJECT_ROOT, "03_data", "output_data", "output.bin")

if TEST_OUT_DIR:
    OUT_BIN = os.path.join(TEST_OUT_DIR, "output.bin")
else:
    OUT_BIN = DEFAULT_OUT_BIN

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

def main():
    print(COL_CYAN + "============== Sensor Reader ==============" + COL_RESET)
    print(COL_YELLOW + "[INFO] sensor_reader.py is running" + COL_RESET)

    # Normalize CSV_DIR for Windows (handles forward/back slashes)
    csv_dir = os.path.normpath(CSV_DIR)

    if not os.path.isdir(csv_dir):
        print(COL_RED + f"[ERROR] CSV directory does not exist: {csv_dir}" + COL_RESET)
        sys.exit(1)

    csv_files = glob.glob(os.path.join(csv_dir, "*.csv"))
    if not csv_files:
        print(COL_RED + f"[ERROR] No CSV files found in {csv_dir}" + COL_RESET)
        sys.exit(1)

    latest_csv = max(csv_files, key=os.path.getmtime)
    print(COL_CYAN + f"Newest CSV detected: {latest_csv}" + COL_RESET)

    fields, rows = load_csv(latest_csv)
    if fields is None or not rows:
        print(COL_RED + "[ERROR] CSV file is empty or malformed" + COL_RESET)
        sys.exit(1)

    timestamps = parse_timestamps(rows)
    if not timestamps:
        print(COL_RED + "[ERROR] Failed to parse timestamps" + COL_RESET)
        sys.exit(1)

    temp, humid, pressure, air_qual, co2, voc = parse_values(rows)

    try:
        export_binary(OUT_BIN, timestamps, temp, humid, pressure, air_qual, co2, voc)
    except Exception as e:
        print(COL_RED + f"[ERROR] Failed to write binary: {e}" + COL_RESET)
        sys.exit(1)


    print(COL_GREEN + "[OK] sensor_reader.py ran successfully" + COL_RESET)

if __name__ == "__main__":
    main()