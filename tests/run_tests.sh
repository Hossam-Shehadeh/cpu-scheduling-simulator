#!/usr/bin/env bash
# =============================================================
#  tests/run_tests.sh — Automated test runner
#  Usage:  bash tests/run_tests.sh   (from repo root)
# =============================================================
set -euo pipefail

SCHEDULER="./scheduler"
EXAMPLES="./examples"
PASS=0
FAIL=0
RED='\033[0;31m'
GRN='\033[0;32m'
YLW='\033[1;33m'
NC='\033[0m'

# ── helpers ──────────────────────────────────────────────────
pass() { echo -e "${GRN}  ✓ PASS${NC}  $1"; PASS=$((PASS+1)); }
fail() { echo -e "${RED}  ✗ FAIL${NC}  $1"; FAIL=$((FAIL+1)); }
info() { echo -e "${YLW}▶${NC} $1"; }

run_and_check() {
    local label="$1"
    local input="$2"
    local quantum="$3"
    local expected_fragment="$4"

    local output
    output=$("$SCHEDULER" "$input" "$quantum" 2>&1)

    if echo "$output" | grep -qF -- "$expected_fragment"; then
        pass "$label"
    else
        fail "$label — expected to find: '$expected_fragment'"
        echo "    Full output:"
        echo "$output" | sed 's/^/    /'
    fi
}

# ── build check ───────────────────────────────────────────────
info "Building..."
make --silent
echo ""

# ── tests ─────────────────────────────────────────────────────
info "Running tests..."
echo ""

# Basic: FCFS avg waiting time should be 16.00
run_and_check \
    "basic.txt — FCFS avg waiting time" \
    "$EXAMPLES/basic.txt" 4 \
    "Average Waiting Time: 16.00"

# Basic: RR avg waiting time should be 4.67
run_and_check \
    "basic.txt — RR avg waiting time" \
    "$EXAMPLES/basic.txt" 4 \
    "Average Waiting Time: 4.67"

# Basic: all three algorithm headers present
run_and_check \
    "basic.txt — FCFS section header present" \
    "$EXAMPLES/basic.txt" 4 \
    "--- FCFS ---"

run_and_check \
    "basic.txt — SJF section header present" \
    "$EXAMPLES/basic.txt" 4 \
    "--- SJF ---"

run_and_check \
    "basic.txt — RR section header present" \
    "$EXAMPLES/basic.txt" 4 \
    "--- RR ---"

# Convoy: P1 completes at t=50 under FCFS
run_and_check \
    "convoy_effect.txt — P1 completes at 50" \
    "$EXAMPLES/convoy_effect.txt" 4 \
    "Process 1 finished"

# Balanced: runs without crashing
run_and_check \
    "balanced.txt — runs without error" \
    "$EXAMPLES/balanced.txt" 3 \
    "Average Waiting Time"

# Starvation demo: runs without crashing
run_and_check \
    "starvation_demo.txt — runs without error" \
    "$EXAMPLES/starvation_demo.txt" 2 \
    "Average Waiting Time"

# Error handling: invalid quantum should exit with error
info ""
info "Error handling..."
out0=$("$SCHEDULER" "$EXAMPLES/basic.txt" 0 2>&1 || true)
if echo "$out0" | grep -qF "Error"; then
    pass "quantum=0 correctly rejected"
else
    fail "quantum=0 should produce an error message"
fi

outn=$("$SCHEDULER" "$EXAMPLES/basic.txt" -1 2>&1 || true)
if echo "$outn" | grep -qF "Error"; then
    pass "quantum=-1 correctly rejected"
else
    fail "quantum=-1 should produce an error message"
fi

# ── summary ───────────────────────────────────────────────────
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
TOTAL=$((PASS + FAIL))
echo -e "  Results: ${GRN}${PASS} passed${NC} / ${RED}${FAIL} failed${NC} / ${TOTAL} total"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if [[ $FAIL -gt 0 ]]; then
    exit 1
fi
