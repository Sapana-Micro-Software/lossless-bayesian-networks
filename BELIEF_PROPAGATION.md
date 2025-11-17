# Lossless Belief Propagation with Influence Tracing

## Overview

This implementation adds **lossless belief propagation** (sum-product algorithm) to the Bayesian network, enabling exact inference while tracing how probabilities flow through the network.

## Key Features

### 1. **Sum-Product Message Passing**
- Implements the standard belief propagation algorithm
- Maintains exact probabilities (lossless)
- Two-pass algorithm: upward and downward message passing

### 2. **Influence Tracing**
- Traces how evidence influences query variables
- Identifies causal pathways through the network
- Computes influence strength for each path
- Provides per-state influence analysis

### 3. **Exact Inference**
- No approximations - maintains full precision
- Computes posterior beliefs for all nodes
- Handles evidence correctly
- Normalizes distributions properly

## Algorithm Details

### Message Passing Structure

The algorithm uses a two-pass approach:

1. **Upward Pass (Leaves → Root)**
   - Messages flow from child nodes to parent nodes
   - Each message represents the probability distribution
   - Messages are computed using conditional probabilities and child messages

2. **Downward Pass (Root → Leaves)**
   - Messages flow from parent nodes to child nodes
   - Propagates information from root to all nodes
   - Updates beliefs based on parent information

### Belief Computation

For each node, beliefs are computed as:
```
Belief(node) = Prior(node) × ∏ Messages_from_parents × ∏ Messages_from_children
```

### Influence Tracing

The algorithm traces:
- **Source Node**: Where evidence is observed
- **Target Node**: Query variable of interest
- **Path**: Sequence of nodes through which influence flows
- **Influence Strength**: Quantitative measure of how much the source affects the target
- **Per-State Influence**: How each state of the target is affected

## Usage Example

```cpp
// Create network
BayesianNetwork network;
network.addNode("A", "Cause", {"False", "True"});
network.addNode("B", "Intermediate", {"Low", "High"});
network.addNode("C", "Effect", {"Negative", "Positive"});
network.addEdge("A", "B");
network.addEdge("B", "C");

// Set up CPTs...
// (see main.cpp for complete example)

// Perform belief propagation
std::map<std::string, std::string> evidence;
evidence["C"] = "Positive";

std::vector<std::string> queryNodes = {"A", "B"};

auto result = network.beliefPropagation(queryNodes, evidence, true);
auto& beliefs = result.first;        // Node -> State -> Probability
auto& influenceTraces = result.second;  // Vector of influence paths

// Access beliefs
for (const auto& nodeBelief : beliefs) {
    std::cout << "Node: " << nodeBelief.first << "\n";
    for (const auto& stateProb : nodeBelief.second) {
        std::cout << "  P(" << stateProb.first << ") = " 
                  << stateProb.second << "\n";
    }
}

// Access influence traces
for (const auto& trace : influenceTraces) {
    std::cout << "Source: " << trace.sourceNode 
              << " -> Target: " << trace.targetNode << "\n";
    std::cout << "Path: " << trace.path << "\n";
    std::cout << "Strength: " << trace.influenceStrength << "\n";
}
```

## API Reference

### `beliefPropagation()`

```cpp
std::pair<
    std::map<std::string, std::map<std::string, double>>,  // Beliefs
    std::vector<InfluenceTrace>                             // Traces
>
beliefPropagation(
    const std::vector<std::string>& queryNodes,  // Variables to query
    const std::map<std::string, std::string>& evidence,  // Observed evidence
    bool traceInfluence = true  // Whether to trace influence paths
) const;
```

**Returns:**
- **First element**: Map from node ID to state probabilities (beliefs)
- **Second element**: Vector of influence traces showing how evidence flows

### `InfluenceTrace` Structure

```cpp
struct InfluenceTrace {
    std::string sourceNode;           // Where evidence is observed
    std::string targetNode;            // Query variable
    std::string path;                 // Path taken (e.g., "A->B->C")
    double influenceStrength;        // Overall influence strength
    std::map<std::string, double> stateInfluences;  // Per-state influence
};
```

## Advantages

1. **Efficient**: Message passing is often more efficient than variable elimination for tree-structured networks
2. **Exact**: Maintains full precision - no approximations
3. **Informative**: Provides influence traces showing causal pathways
4. **Flexible**: Can query multiple nodes simultaneously
5. **Lossless**: All probability information preserved

## Applications

- **Causal Analysis**: Understand how causes affect effects
- **Diagnostic Reasoning**: Trace symptoms back to diseases
- **Sensitivity Analysis**: See how evidence affects beliefs
- **Path Analysis**: Identify important pathways in the network
- **Explainable AI**: Provide explanations for inference results

## Comparison with Variable Elimination

| Feature | Variable Elimination | Belief Propagation |
|---------|---------------------|-------------------|
| Exactness | ✓ Lossless | ✓ Lossless |
| Efficiency | Good for small networks | Better for tree structures |
| Influence Tracing | ✗ Not available | ✓ Built-in |
| Message Passing | ✗ No | ✓ Yes |
| Path Analysis | ✗ No | ✓ Yes |

## Implementation Notes

- The algorithm handles observed evidence correctly
- Messages are normalized to prevent numerical issues
- Influence paths are found using depth-first search
- The algorithm works for any DAG structure
- All computations maintain exact precision

## Future Enhancements

Potential improvements:
- Loopy belief propagation for networks with cycles
- Approximate variants for very large networks
- Parallel message passing
- Incremental updates when evidence changes
- Visualization of message flow

## References

- Pearl, J. (1988). *Probabilistic Reasoning in Intelligent Systems*
- Koller, D. & Friedman, N. (2009). *Probabilistic Graphical Models*
