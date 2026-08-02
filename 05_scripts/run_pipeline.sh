#!/bin/bash

set -e

echo "Testing started..."
cd "$(dirname "$0")/.."

echo "Accessing build file"
cd build

echo "Running entire pipeline (both stats and anomalies)"
ninja run