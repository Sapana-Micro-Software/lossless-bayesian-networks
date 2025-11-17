# Reverse Belief Propagation with Lossless Tracing

## Overview

**Reverse Belief Propagation** complements forward belief propagation by propagating beliefs from **effects (children)** to **causes (parents)**, enabling diagnostic reasoning with lossless probability tracing.

## Key Features

### 1. **Reverse Message Passing**
- Propagates messages from observed effects to their causes
- Two-pass algorithm: reverse upward and reverse downward passes
- Maintains exact probabilities (lossless)

### 2. **Diagnostic Reasoning**
- Given observed effects, infer probabilities of causes
- Perfect for medical diagnosis, fault diagnosis, and root cause analysis
- Traces how effects influence their causes

### 3. **Lossless Reverse Tracing**
- Traces probability flow in reverse direction (effect → cause)
- Identifies reverse causal pathways
- Computes reverse influence strength
- Provides per-state reverse influence analysis

## Algorithm Details

### Reverse Message Passing Structure

The algorithm uses a two-pass approach in reverse:

1. **Reverse Upward Pass (Effects → Causes)**
   - Messages flow from child nodes (effects) to parent nodes (causes)
   - Starts from evidence nodes (observed effects)
   - Propagates diagnostic information upward

2. **Reverse Downward Pass (Causes → Other Effects)**
   - Messages flow from parent nodes to other child nodes
   - Distributes information to all related nodes
   - Updates beliefs based on reverse propagation

### Reverse Belief Computation

For each node, reverse beliefs are computed as:
```
Reverse_Belief(node) = Prior(node) × ∏ Reverse_Messages_from_children × ∏ Reverse_Messages_from_parents
```

### Reverse Influence Tracing

The algorithm traces:
- **Source Node**: Observed effect (evidence)
- **Target Node**: Query cause
- **Reverse Path**: Sequence of nodes following parent relationships (e.g., "Symptom<-Disease")
- **Reverse Influence Strength**: How much the effect influences the cause
- **Per-State Reverse Influence**: How each state of the cause is affected

## Usage Example

```cpp
// Create diagnostic network: Disease -> Symptom1, Symptom2
BayesianNetwork network;
network.addNode("Disease", "Disease", {"None", "Cold", "Flu"});
network.addNode("Symptom1", "Fever", {"No", "Yes"});
network.addNode("Symptom2", "Cough", {"No", "Yes"});

network.addEdge("Disease", "Symptom1");
network.addEdge("Disease", "Symptom2");

// Set up CPTs...
// (see main.cpp for complete example)

// Perform reverse belief propagation
// Given observed effects (symptoms), infer causes (disease)
std::map<std::string, std::string> evidence;
evidence["Symptom1"] = "Yes";  // Observed effect
evidence["Symptom2"] = "Yes";  // Observed effect

std::vector<std::string> queryNodes = {"Disease"};  // Query cause

auto result = network.reverseBeliefPropagation(queryNodes, evidence, true);
auto& beliefs = result.first;        // Node -> State -> Probability
auto& reverseInfluenceTraces = result.second;  // Vector of reverse influence paths

// Access reverse beliefs
for (const auto& nodeBelief : beliefs) {
    std::cout << "Node: " << nodeBelief.first << "\n";
    for (const auto& stateProb : nodeBelief.second) {
        std::cout << "  P(" << stateProb.first << ") = " 
                  << stateProb.second << "\n";
    }
}

// Access reverse influence traces
for (const auto& trace : reverseInfluenceTraces) {
    std::cout << "Source (Effect): " << trace.sourceNode 
              << " -> Target (Cause): " << trace.targetNode << "\n";
    std::cout << "Reverse Path: " << trace.path << "\n";
    std::cout << "Strength: " << trace.influenceStrength << "\n";
}
```

## API Reference

### `reverseBeliefPropagation()`

```cpp
std::pair<
    std::map<std::string, std::map<std::string, double>>,  // Reverse Beliefs
    std::vector<InfluenceTrace>                             // Reverse Traces
>
reverseBeliefPropagation(
    const std::vector<std::string>& queryNodes,  // Causes to query
    const std::map<std::string, std::string>& evidence,  // Observed effects
    bool traceInfluence = true  // Whether to trace reverse influence paths
) const;
```

**Returns:**
- **First element**: Map from node ID to state probabilities (reverse beliefs)
- **Second element**: Vector of reverse influence traces showing how effects flow to causes

### Reverse Path Notation

Reverse paths use `<-` to indicate reverse direction:
- Forward path: `"Disease->Symptom1"`
- Reverse path: `"Symptom1<-Disease"`

## Comparison: Forward vs Reverse

| Feature | Forward Belief Propagation | Reverse Belief Propagation |
|--------|---------------------------|---------------------------|
| Direction | Causes → Effects | Effects → Causes |
| Use Case | Predictive reasoning | Diagnostic reasoning |
| Evidence | Observed causes | Observed effects |
| Query | Effects | Causes |
| Path Notation | `->` | `<-` |
| Example | "Given disease, predict symptoms" | "Given symptoms, diagnose disease" |

## Applications

### 1. **Medical Diagnosis**
- **Input**: Observed symptoms (effects)
- **Output**: Disease probabilities (causes)
- **Example**: Given fever and cough, what's the probability of flu?

### 2. **Fault Diagnosis**
- **Input**: Observed system failures (effects)
- **Output**: Root cause probabilities (causes)
- **Example**: Given error messages, what component failed?

### 3. **Root Cause Analysis**
- **Input**: Observed symptoms/problems (effects)
- **Output**: Underlying cause probabilities
- **Example**: Given performance issues, what's the root cause?

### 4. **Quality Control**
- **Input**: Observed defects (effects)
- **Output**: Manufacturing process issue probabilities (causes)
- **Example**: Given product defects, which process step failed?

## Advantages

1. **Exact**: Maintains full precision - no approximations
2. **Diagnostic**: Perfect for diagnostic reasoning tasks
3. **Informative**: Provides reverse influence traces
4. **Lossless**: All probability information preserved
5. **Complementary**: Works alongside forward propagation

## Implementation Notes

- The algorithm handles observed evidence correctly
- Reverse messages are normalized to prevent numerical issues
- Reverse influence paths follow parent relationships
- The algorithm works for any DAG structure
- All computations maintain exact precision
- Reverse paths use `<-` notation to distinguish from forward paths

## Example Output

```
Evidence (Observed Effects): Symptom1=Yes, Symptom2=Yes
Query (Causes): What is P(Disease)?

Reverse Beliefs (Diagnostic Probabilities):
  Disease:
    P(Disease=Cold) = 0.4492
    P(Disease=Flu) = 0.5134
    P(Disease=None) = 0.0374

Reverse Influence Traces (Effect -> Cause):
  Source (Effect): Symptom1 -> Target (Cause): Disease
  Reverse Path: Symptom1<-Disease
  Influence Strength: 0.3333
  Per-State Influences:
    Cold: 0.4492
    Flu: 0.5134
    None: 0.0374
```

## Combining Forward and Reverse

You can use both algorithms together:

```cpp
// Forward: Given cause, predict effects
auto forward = network.beliefPropagation(effectNodes, causeEvidence, true);

// Reverse: Given effects, diagnose causes
auto reverse = network.reverseBeliefPropagation(causeNodes, effectEvidence, true);
```

This provides complete bidirectional reasoning capabilities.

## References

- Pearl, J. (1988). *Probabilistic Reasoning in Intelligent Systems*
- Koller, D. & Friedman, N. (2009). *Probabilistic Graphical Models*
- Diagnostic reasoning in Bayesian networks
