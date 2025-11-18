#!/bin/bash
# run_all_tests.sh - Run all test suites
# Copyright (C) 2025, Shyamal Chandra

echo "=========================================="
echo "Running All Test Suites"
echo "=========================================="
echo ""

cd "$(dirname "$0")/.."

# Compile all tests
make tests

# Run each test suite
echo "Running Unit Tests..."
./tests/unit_tests
UNIT_EXIT=$?

echo ""
echo "Running Regression Tests..."
./tests/regression_tests
REGRESSION_EXIT=$?

echo ""
echo "Running A-B Comparison Tests..."
./tests/ab_tests
AB_EXIT=$?

echo ""
echo "Running UX Tests..."
./tests/ux_tests
UX_EXIT=$?

echo ""
echo "Running Blackbox Tests..."
./tests/blackbox_tests
BLACKBOX_EXIT=$?

echo ""
echo "=========================================="
echo "Test Summary"
echo "=========================================="
echo "Unit Tests:        $([ $UNIT_EXIT -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
echo "Regression Tests:  $([ $REGRESSION_EXIT -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
echo "A-B Tests:        $([ $AB_EXIT -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
echo "UX Tests:         $([ $UX_EXIT -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
echo "Blackbox Tests:   $([ $BLACKBOX_EXIT -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
echo ""

TOTAL_EXIT=$((UNIT_EXIT + REGRESSION_EXIT + AB_EXIT + UX_EXIT + BLACKBOX_EXIT))

if [ $TOTAL_EXIT -eq 0 ]; then
    echo "All tests PASSED!"
    exit 0
else
    echo "Some tests FAILED!"
    exit 1
fi
