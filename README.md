# Lossless Bayesian Network Implementation

Copyright (C) 2025, Shyamal Chandra

## Overview

This project implements a lossless Bayesian network in C++ with exact inference capabilities. The implementation maintains all probability information without approximation, providing precise probabilistic reasoning.

## Features

- **Lossless Representation**: All probabilities stored and computed exactly
- **Exact Inference**: Variable elimination algorithm for precise inference
- **DAG Validation**: Automatic cycle detection and topological sorting
- **Flexible Structure**: Support for arbitrary DAG structures
- **CPT Management**: Efficient storage and access of conditional probability tables
- **File I/O**: Network serialization and loading capabilities

## Building

### Requirements

- C++17 compatible compiler (g++, clang++)
- Make

### Build Instructions

```bash
make
```

This will compile the project and create the `bayesian_network` executable.

### Running

```bash
make run
```

Or directly:

```bash
./bayesian_network
```

## Project Structure

```
lossless_bayesian_networks/
├── node.hpp                    # Node class definition
├── cpt.hpp                     # Conditional Probability Table class
├── bayesian_network.hpp        # Main Bayesian network class
├── main.cpp                    # Example usage and demonstrations
├── Makefile                    # Build configuration
├── README.md                   # This file
└── docs/
    ├── lossless_bayesian_networks.tex    # Main documentation
    ├── presentation.tex                   # Beamer presentation
    └── reference_manual.tex              # Complete API reference
```

## Documentation

The project includes comprehensive documentation:

1. **Main Documentation** (`docs/lossless_bayesian_networks.tex`): Complete guide covering architecture, algorithms, and usage
2. **Presentation** (`docs/presentation.tex`): Beamer slides for presentations
3. **Reference Manual** (`docs/reference_manual.tex`): Complete API documentation

To compile the LaTeX documents:

```bash
cd docs
pdflatex lossless_bayesian_networks.tex
pdflatex presentation.tex
pdflatex reference_manual.tex
```

## Quick Start

```cpp
#include "bayesian_network.hpp"

// Create network
BayesianNetwork network;

// Add nodes
network.addNode("Disease", "Disease", {"None", "Cold", "Flu"});
network.addNode("Symptom", "Fever", {"No", "Yes"});

// Add edge
network.addEdge("Disease", "Symptom");

// Create and set CPT
std::vector<size_t> dims = {3, 2};
ConditionalProbabilityTable cpt(dims);
cpt.setProbability({0}, 0, 0.9);  // P(Fever=No | Disease=None)
cpt.setProbability({0}, 1, 0.1);  // P(Fever=Yes | Disease=None)
// ... set other probabilities
cpt.normalize();
network.setCPT("Symptom", cpt);

// Perform inference
std::map<std::string, std::string> evidence;
evidence["Symptom"] = "Yes";
std::vector<std::string> query = {"Disease"};
auto results = network.variableElimination(query, evidence);
```

## Examples

The `main.cpp` file includes two complete examples:

1. **Medical Diagnosis Network**: Demonstrates disease diagnosis based on symptoms
2. **Alarm Network**: Classic Bayesian network example with burglary, earthquake, and alarm

## License

Copyright (C) 2025, Shyamal Chandra

## Contact

For questions or issues, please refer to the documentation or contact the author.
