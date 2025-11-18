# Test Suite Documentation

## Overview

This directory contains a comprehensive test suite for the Lossless Bayesian Network implementation, including:

- **Unit Tests**: Test individual components (Node, CPT, BayesianNetwork)
- **Regression Tests**: Ensure existing functionality continues to work
- **A-B Tests**: Compare different inference algorithms for consistency
- **UX Tests**: Test API usability and user experience
- **Blackbox Tests**: Test the system as a whole without knowing internals

## Test Framework

The test suite uses a lightweight custom testing framework (`test_framework.hpp`) that provides:

- Test execution and timing
- Assertion helpers (assertTrue, assertEqual, assertThrows, etc.)
- Test result reporting
- Summary statistics

## Running Tests

### Run All Tests

```bash
make test
```

or

```bash
./tests/run_all_tests.sh
```

### Run Individual Test Suites

```bash
# Unit tests
make test-unit

# Regression tests
make test-regression

# A-B comparison tests
make test-ab

# UX tests
make test-ux

# Blackbox tests
make test-blackbox
```

### Build Tests Only

```bash
make tests
```

## Test Suites

### 1. Unit Tests (`unit_tests.cpp`)

Tests individual components in isolation:

- **Node Tests**: Construction, state lookup, parent management
- **CPT Tests**: Probability setting/getting, normalization, validation
- **BayesianNetwork Tests**: Node/edge addition, cycle detection, CPT setting, joint probability

**Example:**
```cpp
suite.runTest("Node state index lookup", []() {
    Node node("TestNode", {"State1", "State2", "State3"});
    return TestSuite::assertEqual(node.getStateIndex("State1"), 0);
});
```

### 2. Regression Tests (`regression_tests.cpp`)

Ensures existing functionality continues to work:

- Medical diagnosis example regression
- Alarm network example regression
- Topological sort consistency
- CPT normalization consistency

**Purpose**: Prevent regressions when making changes to the codebase.

### 3. A-B Comparison Tests (`ab_tests.cpp`)

Compares different inference algorithms to ensure consistency:

- Variable Elimination vs Belief Propagation
- Belief Propagation vs Reverse Belief Propagation
- All inference methods produce normalized results

**Purpose**: Ensure different algorithms produce consistent results.

### 4. UX Tests (`ux_tests.cpp`)

Tests API usability and user experience:

- Simple network construction API
- Intuitive error messages
- CPT setup workflow
- Inference API simplicity
- API completeness
- Consistent naming conventions
- Helpful error messages

**Purpose**: Ensure the API is easy to use and intuitive.

### 5. Blackbox Tests (`blackbox_tests.cpp`)

Tests the system as a whole without knowing internal implementation:

- Complete workflow tests
- Multiple inference methods produce valid results
- Edge case handling
- Complex network handling
- Performance tests

**Purpose**: Test the system from a user's perspective.

## Test Framework API

### Assertion Methods

```cpp
// Boolean assertions
TestSuite::assertTrue(condition, "message");
TestSuite::assertFalse(condition, "message");

// Equality assertions
TestSuite::assertEqual(a, b, tolerance, "message");  // For doubles
TestSuite::assertEqual(a, b, "message");             // For strings, ints, size_t

// Null assertions
TestSuite::assertNotNull(ptr, "message");

// Exception assertions
TestSuite::assertThrows([]() { /* code */ }, "message");
```

### Running Tests

```cpp
TestSuite suite("Suite Name");

suite.runTest("Test Name", []() {
    // Test code
    return TestSuite::assertTrue(condition);
});

suite.printSummary();
bool allPassed = suite.allPassed();
```

## Test Coverage

The test suite covers:

- ✅ Node construction and state management
- ✅ CPT probability operations
- ✅ Network construction (nodes, edges)
- ✅ Cycle detection
- ✅ CPT normalization and validation
- ✅ Variable elimination inference
- ✅ Belief propagation inference
- ✅ Reverse belief propagation inference
- ✅ Joint probability computation
- ✅ Error handling
- ✅ API usability
- ✅ End-to-end workflows
- ✅ Performance characteristics

## Adding New Tests

To add a new test:

1. Choose the appropriate test file (unit, regression, ab, ux, or blackbox)
2. Add a test function or add to an existing function
3. Use the TestSuite API to run and assert

**Example:**
```cpp
void runNewTests(TestSuite& suite) {
    suite.runTest("New feature test", []() {
        // Test implementation
        return TestSuite::assertTrue(condition);
    });
}
```

## Continuous Integration

The test suite is designed to be run in CI/CD pipelines:

```bash
make test
```

Returns exit code 0 if all tests pass, non-zero otherwise.

## Test Results

Each test suite provides:

- Individual test results (✓ or ✗)
- Execution time per test
- Summary statistics (total, passed, failed, time)
- Failed test details

## Maintenance

- Keep tests up to date with code changes
- Add regression tests when bugs are fixed
- Add unit tests for new features
- Update A-B tests when new algorithms are added
- Ensure all tests pass before committing
