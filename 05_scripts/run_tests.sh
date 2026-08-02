#!/bin/bash

set -e

echo "Testing started..."
cd "$(dirname "$0")/.."

echo "Accessing build file"
cd build

echo "Running all tests"
ninja run_tests