import matplotlib.pyplot as plt
from datetime import datetime
import csv
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
SAMPLES_DIR = os.path.join(BASE_DIR, "..", "samples")

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

print("[INFO] plot_data.py is running\n")

def plot_data(filename):
    with open(filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)
        for row in csvreader:
            rows.append(row)

        print("Total no. of rows: %d" % csvreader.line_num)

    print('Field names are: ' + ', '.join(fields))

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
filename = os.path.join(SAMPLES_DIR, "sample_log_01.csv")
plot_data(filename)
print("[OK] plot_data.py successfully ran\n")

