#!/usr/bin/env bash
set -euo pipefail

INPUT="${1:-examples/basic.txt}"
QUANTUM="${2:-4}"

echo "==> Building CPU Scheduling Simulator..."

if command -v cmake >/dev/null 2>&1; then
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build
  echo "==> Running: ./build/scheduler $INPUT $QUANTUM"
  ./build/scheduler "$INPUT" "$QUANTUM"
elif command -v make >/dev/null 2>&1; then
  make
  echo "==> Running: ./scheduler $INPUT $QUANTUM"
  ./scheduler "$INPUT" "$QUANTUM"
elif command -v g++ >/dev/null 2>&1; then
  g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude -o scheduler \
      src/main.cpp src/scheduler.cpp src/process.cpp
  echo "==> Running: ./scheduler $INPUT $QUANTUM"
  ./scheduler "$INPUT" "$QUANTUM"
else
  echo "Error: no supported build tool found (cmake, make, or g++)."
  exit 1
fi
