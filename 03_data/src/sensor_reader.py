from datetime import datetime
import csv

def load_csv(filename):
    with open(filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)
        rows = []
        for row in csvreader:
            rows.append(row)
    return fields, row
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