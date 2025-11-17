# Bayesian Network Visualization

Copyright (C) 2025, Shyamal Chandra

## Overview

This directory contains an Objective-C wrapper and macOS visualization application for the Lossless Bayesian Network implementation. The visualization provides exact precision display of network structure, probabilities, and inference results.

## Features

- **Exact Precision Visualization**: All probabilities displayed with full precision (6 decimal places)
- **Interactive Graph View**: Click nodes to select them, view network structure
- **Real-time Inference**: Perform inference queries and see results immediately
- **Color-coded Nodes**: 
  - Blue: Query nodes
  - Green: Evidence nodes (observed)
  - Orange: Selected nodes
  - Gray: Default nodes
- **Force-directed Layout**: Automatic node positioning for optimal visualization

## Building the macOS App

### Requirements

- macOS 10.13 or later
- Xcode Command Line Tools
- clang++ with Objective-C++ support

### Build Instructions

```bash
make -f Makefile.macos
```

This will create `BayesianNetworkVisualizer.app` in the current directory.

### Running the App

```bash
make -f Makefile.macos run
```

Or double-click the `BayesianNetworkVisualizer.app` bundle.

## Project Structure

### Objective-C Wrapper

- `BayesianNetworkWrapper.h` / `.mm`: Objective-C++ bridge to C++ implementation
- Provides high-level API for network manipulation and inference

### Visualization Components

- `NetworkGraphView.h` / `.m`: Custom NSView for rendering the network graph
- `ViewController.h` / `.m`: Main view controller managing the UI
- `AppDelegate.h` / `.m`: Application delegate

### Usage Example

```objc
// Create network
BayesianNetworkWrapper *network = [[BayesianNetworkWrapper alloc] init];

// Add nodes
[network addNodeWithId:@"Disease" name:@"Disease" states:@[@"None", @"Cold", @"Flu"]];
[network addNodeWithId:@"Symptom" name:@"Fever" states:@[@"No", @"Yes"]];

// Add edge
[network addEdgeFromParent:@"Disease" toChild:@"Symptom"];

// Perform inference
NSArray<NSString *> *queryNodes = @[@"Disease"];
NSDictionary<NSString *, NSString *> *evidence = @{@"Symptom": @"Yes"};
BNInferenceResult *result = [network performInferenceWithQueryNodes:queryNodes 
                                                             evidence:evidence];
```

## UI Components

1. **Network Graph View**: Main visualization area showing nodes and edges
2. **Query Nodes Field**: Comma-separated list of node IDs to query
3. **Evidence Field**: Comma-separated key=value pairs (e.g., "Symptom1=Yes, Symptom2=Yes")
4. **Results Text View**: Displays inference results with exact probabilities
5. **Status Label**: Shows operation status and error messages

## Notes

- The visualization maintains exact precision for all probability calculations
- Node positions are automatically calculated using a force-directed layout algorithm
- The app loads an example medical diagnosis network by default
- CPT (Conditional Probability Table) setup requires lower-level API access - see C++ implementation for full control

## Integration with Xcode

To use this in Xcode:

1. Create a new macOS Cocoa App project
2. Add all `.h`, `.m`, `.mm`, and `.hpp` files to the project
3. Set the project to use Objective-C++ for `.mm` files
4. Link against Cocoa and Foundation frameworks
5. Build and run

## Copyright

Copyright (C) 2025, Shyamal Chandra
