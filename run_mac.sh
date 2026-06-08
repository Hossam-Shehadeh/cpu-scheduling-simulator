#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 2 ]]; then
  echo "Usage: ./run_mac.sh <input_file> <time_quantum>"
  exit 1
fi

if command -v cmake >/dev/null 2>&1; then
  cmake -S . -B build
  cmake --build build
  ./build/scheduler "$1" "$2"
elif command -v make >/dev/null 2>&1; then
  make
  ./scheduler "$1" "$2"
elif command -v g++ >/dev/null 2>&1; then
  g++ -std=c++17 -Wall -Wextra -pedantic -o scheduler main.cpp scheduler.cpp process.cpp
  ./scheduler "$1" "$2"
else
  echo "Error: no supported build tool found (cmake, make, or g++)."
  exit 1
fi
