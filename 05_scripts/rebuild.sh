#!/bin/bash

set -e

echo "Rebuilding started..."
cd "$(dirname "$0")/.."

echo "build folder deleted"
rm -rf build

echo "build folder created"
mkdir build
cd build

echo "cmake ran"
cmake ..

echo "Rebuilding complete"
ninja