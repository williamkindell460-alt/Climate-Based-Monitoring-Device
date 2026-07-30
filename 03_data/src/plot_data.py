import matplotlib.pyplot as plt
from datetime import datetime
import csv
import os
import glob

# Determine project root dynamically
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "../.."))

# Build portable paths
CSV_DIR = os.path.join(PROJECT_ROOT, "01_firmware", "real_output")
OUT_BIN = os.path.join(PROJECT_ROOT, "03_data", "output_data", "output.bin")

filename = "" 
fields = []
rows = []
time = []
temp = []
humid = []
pressure= []
air_qual= []
co2= []
voc= []

print("==================== Plot Data ====================\n")
print("[INFO] plot_data.py is running\n")

def plot_data(filename):
    with open(filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)
        for row in csvreader:
            rows.append(row)

        print("[OUTPUT] Total no. of rows: %d" % csvreader.line_num + "\n")

    print('[OUTPUT] Field names are: ' + ', '.join(fields) + "\n")

    timestamps = [datetime.strptime(row[0], "%Y-%m-%dT%H:%M:%SZ") for row in rows]

    start = timestamps[0]
    time = [(t - start).total_seconds() / 60 for t in timestamps]


    temp = [float(row[1]) for row in rows]
    humid = [float(row[2]) for row in rows]
    pressure = [float(row[3]) for row in rows]
    air_qual = [float(row[4]) for row in rows]
    co2 = [float(row[5]) for row in rows]
    voc = [float(row[6]) for row in rows]

    plt.figure(figsize=(8,10))

    plt.subplot(611)
    plt.plot(time, temp)
    plt.ylabel("Temperature")

    plt.subplot(612)
    plt.plot(time, humid)
    plt.ylabel("Humidity")

    plt.subplot(613)
    plt.plot(time, pressure)
    plt.ylabel("Pressure")

    plt.subplot(614)
    plt.plot(time, air_qual)
    plt.ylabel("Air Quality")

    plt.subplot(615)
    plt.plot(time, co2)
    plt.ylabel("CO2")

    plt.subplot(616)
    plt.plot(time, voc)
    plt.ylabel("VOC")
    plt.xlabel("time")


    plt.tight_layout()
    plt.show()

csv_files = glob.glob(os.path.join(CSV_DIR, "*.csv"))
if not csv_files:
    print(f"[ERROR] No CSV files found in {CSV_DIR}")
    exit(1)

latest_csv = max(csv_files, key=os.path.getmtime)
print(f"Newest CSV detected: {latest_csv}")

filename = os.path.join(CSV_DIR, latest_csv)
plot_data(filename)
print("[OK] plot_data.py successfully ran\n")

