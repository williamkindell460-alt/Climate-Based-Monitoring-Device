import matplotlib.pyplot as plt
from datetime import datetime
import csv
import os
import glob

COL_RESET      = "\033[0m"
COL_RED        = "\033[31m"
COL_GREEN      = "\033[32m"
COL_YELLOW     = "\033[33m"
COL_CYAN       = "\033[36m"
COL_LIGHT_RED  = "\033[91m"
COL_LIGHT_BLUE = "\033[94m"

# Determine project root dynamically
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "../../.."))

TEST_DIR = os.getenv("CBMD_TEST_DATA_DIR")
TEST_MODE = os.getenv("CBMD_TEST_DATA_DIR") is not None

DEFAULT_CSV_DIR = os.path.join(PROJECT_ROOT, "01_firmware", "real_output")
CSV_DIR = TEST_DIR if TEST_DIR else DEFAULT_CSV_DIR
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

print(COL_CYAN + "==================== Plot Data ====================\n" + COL_RESET)
print(COL_YELLOW + "[INFO] plot_data.py is running\n" + COL_RESET)

def plot_data(filename):
    with open(filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        fields = next(csvreader)
        for row in csvreader:
            rows.append(row)

        print(COL_LIGHT_BLUE + "[OUTPUT] Total no. of rows: %d\n" % csvreader.line_num + COL_RESET)

    print(COL_LIGHT_BLUE + '[OUTPUT] Field names are: ' + ', '.join(fields) + "\n" + COL_RESET)

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

    date = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

    output_dir = os.path.join(
        PROJECT_ROOT,
        "03_data",
        "output_data",
        "plots"
    )

    os.makedirs(output_dir, exist_ok=True)

    output_path = os.path.join(output_dir, f"plot_{date}.png")

    if not TEST_MODE:
        plt.savefig(output_path)
    else:
        print(COL_GREEN + "[OK] plot_data.py ran successfully (test mode, no plot shown)" + COL_RESET)

csv_files = glob.glob(os.path.join(CSV_DIR, "*.csv"))
if not csv_files:
    print(COL_RED + f"\n\n[ERROR] No CSV files found in {CSV_DIR}\n\n" + COL_RESET)
    exit(1)

latest_csv = max(csv_files, key=os.path.getmtime)
print(COL_CYAN + f"Newest CSV detected: {latest_csv}" + COL_RESET)

filename = os.path.join(CSV_DIR, latest_csv)
plot_data(filename)
print(COL_GREEN + "[OK] plot_data.py successfully ran\n" + COL_RESET)