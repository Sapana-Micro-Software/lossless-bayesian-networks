/*
 * node.hpp - Node structure for Bayesian Network
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This file defines the Node class representing a variable in the Bayesian network.
 */

#ifndef NODE_HPP
#define NODE_HPP

// String operations
#include <string>
// Vector container
#include <vector>
// Set container for parent IDs
#include <set>
// Map container for state indices
#include <map>

/**
 * Node class represents a variable in the Bayesian network.
 * Each node has a name, possible states, and maintains parent relationships.
 */
class Node {
public:
    // Variable name identifier
    std::string name;
    // Possible states this variable can take
    std::vector<std::string> states;
    // Set of parent node IDs (for DAG structure)
    std::set<std::string> parentIds;
    // Map state names to indices for fast lookup
    std::map<std::string, int> stateIndexMap;

    /**
     * Default constructor
     */
    Node() = default;

    /**
     * Constructor with name and states
     * @param nodeName Name of the node
     * @param nodeStates Vector of possible state names
     */
    // Build state index map for O(1) lookup
    Node(const std::string& nodeName, const std::vector<std::string>& nodeStates)
        : name(nodeName), states(nodeStates) {
        for (size_t i = 0; i < states.size(); ++i) {
            stateIndexMap[states[i]] = static_cast<int>(i);
        }
    }

    /**
     * Get the index of a state by name
     * @param stateName Name of the state
     * @return Index of the state, or -1 if not found
     */
    int getStateIndex(const std::string& stateName) const {
        auto it = stateIndexMap.find(stateName);
        return (it != stateIndexMap.end()) ? it->second : -1;
    }

    /**
     * Check if a state exists
     * @param stateName Name of the state
     * @return True if state exists, false otherwise
     */
    bool hasState(const std::string& stateName) const {
        return stateIndexMap.find(stateName) != stateIndexMap.end();
    }

    /**
     * Get number of states
     * @return Number of possible states
     */
    size_t getNumStates() const {
        return states.size();
    }

    /**
     * Add a parent node
     * @param parentId ID of the parent node
     */
    void addParent(const std::string& parentId) {
        parentIds.insert(parentId);
    }

    /**
     * Remove a parent node
     * @param parentId ID of the parent node
     */
    void removeParent(const std::string& parentId) {
        parentIds.erase(parentId);
    }

    /**
     * Check if node has a specific parent
     * @param parentId ID of the parent node
     * @return True if parent exists, false otherwise
     */
    bool hasParent(const std::string& parentId) const {
        return parentIds.find(parentId) != parentIds.end();
    }

    /**
     * Get number of parents
     * @return Number of parent nodes
     */
    size_t getNumParents() const {
        return parentIds.size();
    }
};

#endif // NODE_HPP
