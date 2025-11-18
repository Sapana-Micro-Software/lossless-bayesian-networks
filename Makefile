# Makefile for Lossless Bayesian Network
# Copyright (C) 2025, Shyamal Chandra

CXX = g++                                    # C++ compiler
CXXFLAGS = -std=c++17 -Wall -Wextra -O2     # Compiler flags: C++17, warnings, optimization
TARGET = bayesian_network                    # Executable name
SOURCES = main.cpp                           # Source files (headers are included)
OBJECTS = $(SOURCES:.cpp=.o)                # Object files

# Test executables
TEST_TARGETS = tests/unit_tests tests/regression_tests tests/ab_tests tests/ux_tests tests/blackbox_tests
TEST_SOURCES = tests/unit_tests.cpp tests/regression_tests.cpp tests/ab_tests.cpp tests/ux_tests.cpp tests/blackbox_tests.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Default target
all: $(TARGET)                               # Build the executable

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build all tests
tests: $(TEST_TARGETS)

# Build individual test executables
tests/unit_tests: tests/unit_tests.o
	$(CXX) $(CXXFLAGS) -o $@ $<

tests/regression_tests: tests/regression_tests.o
	$(CXX) $(CXXFLAGS) -o $@ $<

tests/ab_tests: tests/ab_tests.o
	$(CXX) $(CXXFLAGS) -o $@ $<

tests/ux_tests: tests/ux_tests.o
	$(CXX) $(CXXFLAGS) -o $@ $<

tests/blackbox_tests: tests/blackbox_tests.o
	$(CXX) $(CXXFLAGS) -o $@ $<

# Compile test object files
tests/%.o: tests/%.cpp tests/test_framework.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run all tests
test: tests
	@./tests/run_all_tests.sh

# Run individual test suites
test-unit: tests/unit_tests
	./tests/unit_tests

test-regression: tests/regression_tests
	./tests/regression_tests

test-ab: tests/ab_tests
	./tests/ab_tests

test-ux: tests/ux_tests
	./tests/ux_tests

test-blackbox: tests/blackbox_tests
	./tests/blackbox_tests

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f $(TEST_OBJECTS) $(TEST_TARGETS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets (not files)
.PHONY: all clean run tests test test-unit test-regression test-ab test-ux test-blackbox
