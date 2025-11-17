# Makefile for Lossless Bayesian Network
# Copyright (C) 2025, Shyamal Chandra

CXX = g++                                    # C++ compiler
CXXFLAGS = -std=c++17 -Wall -Wextra -O2     # Compiler flags: C++17, warnings, optimization
TARGET = bayesian_network                    # Executable name
SOURCES = main.cpp                           # Source files (headers are included)
OBJECTS = $(SOURCES:.cpp=.o)                # Object files

# Default target
all: $(TARGET)                               # Build the executable

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets (not files)
.PHONY: all clean run
