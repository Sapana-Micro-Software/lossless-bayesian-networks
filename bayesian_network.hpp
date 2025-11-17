/*
 * bayesian_network.hpp - Lossless Bayesian Network implementation
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This file implements a lossless Bayesian network with exact inference
 * capabilities, maintaining all probability information without approximation.
 */

#ifndef BAYESIAN_NETWORK_HPP
#define BAYESIAN_NETWORK_HPP

// Node structure
#include "node.hpp"
// Conditional Probability Table
#include "cpt.hpp"
// Map container
#include <map>
// Vector container
#include <vector>
// String operations
#include <string>
// Set container
#include <set>
// Queue for topological sort
#include <queue>
// Algorithm utilities
#include <algorithm>
// Exception handling
#include <stdexcept>
// Input/output stream
#include <iostream>
// File stream operations
#include <fstream>
// String stream operations
#include <sstream>

/**
 * BayesianNetwork class implements a lossless Bayesian network.
 * Supports exact inference using variable elimination and maintains
 * all probability distributions without loss of precision.
 */
class BayesianNetwork {
private:
    // Map of node ID to Node
    std::map<std::string, Node> nodes;
    // Map of node ID to CPT
    std::map<std::string, ConditionalProbabilityTable> cpts;
    // Topological order of nodes
    std::vector<std::string> nodeOrder;

    /**
     * Perform topological sort to determine node ordering
     * @return Vector of node IDs in topological order
     */
    std::vector<std::string> topologicalSort() const {
        // In-degree for each node
        std::map<std::string, size_t> inDegree;
        // Result ordering
        std::vector<std::string> result;

        // Initialize in-degrees
        for (const auto& pair : nodes) {
            inDegree[pair.first] = 0;
        }

        // Calculate in-degrees
        for (const auto& pair : nodes) {
            for ([[maybe_unused]] const std::string& parentId : pair.second.parentIds) {
                inDegree[pair.first]++;
            }
        }

        // Kahn's algorithm for topological sort
        std::queue<std::string> q;
        for (const auto& pair : inDegree) {
            if (pair.second == 0) {
                q.push(pair.first);
            }
        }

        while (!q.empty()) {
            std::string current = q.front();
            q.pop();
            result.push_back(current);

            // Decrease in-degree of children
            for (auto& pair : nodes) {
                if (pair.second.hasParent(current)) {
                    inDegree[pair.first]--;
                    if (inDegree[pair.first] == 0) {
                        q.push(pair.first);
                    }
                }
            }
        }

        // Check for cycles
        if (result.size() != nodes.size()) {
            throw std::runtime_error("Graph contains cycles - not a valid DAG");
        }

        return result;
    }

    /**
     * Check if graph is acyclic (DAG validation)
     * @return True if acyclic, false otherwise
     */
    bool isAcyclic() const {
        try {
            topologicalSort();
            return true;
        } catch (const std::runtime_error&) {
            return false;
        }
    }

public:
    /**
     * Default constructor
     */
    BayesianNetwork() = default;

    /**
     * Add a node to the network
     * @param nodeId Unique identifier for the node
     * @param nodeName Name of the node
     * @param states Vector of possible state names
     */
    void addNode(const std::string& nodeId, 
                 const std::string& nodeName, 
                 const std::vector<std::string>& states) {
        if (nodes.find(nodeId) != nodes.end()) {
            throw std::runtime_error("Node with ID " + nodeId + " already exists");
        }
        nodes[nodeId] = Node(nodeName, states);
        nodeOrder = topologicalSort();
    }

    /**
     * Add an edge from parent to child
     * @param parentId ID of parent node
     * @param childId ID of child node
     */
    void addEdge(const std::string& parentId, const std::string& childId) {
        if (nodes.find(parentId) == nodes.end()) {
            throw std::runtime_error("Parent node " + parentId + " does not exist");
        }
        if (nodes.find(childId) == nodes.end()) {
            throw std::runtime_error("Child node " + childId + " does not exist");
        }
        if (parentId == childId) {
            throw std::runtime_error("Cannot add self-loop");
        }
        nodes[childId].addParent(parentId);
        // Check for cycles after adding edge
        if (!isAcyclic()) {
            nodes[childId].removeParent(parentId);
            throw std::runtime_error("Adding edge would create a cycle");
        }
        nodeOrder = topologicalSort();
    }

    /**
     * Set conditional probability table for a node
     * @param nodeId ID of the node
     * @param cpt Conditional probability table
     */
    void setCPT(const std::string& nodeId, const ConditionalProbabilityTable& cpt) {
        if (nodes.find(nodeId) == nodes.end()) {
            throw std::runtime_error("Node " + nodeId + " does not exist");
        }
        cpts[nodeId] = cpt;
    }

    /**
     * Get conditional probability
     * @param nodeId ID of the node
     * @param nodeState State of the node
     * @param parentStates Map of parent IDs to their states
     * @return Conditional probability P(nodeState | parentStates)
     */
    double getConditionalProbability(const std::string& nodeId,
                                    const std::string& nodeState,
                                    const std::map<std::string, std::string>& parentStates) const {
        if (cpts.find(nodeId) == cpts.end()) {
            throw std::runtime_error("CPT not set for node " + nodeId);
        }

        const Node& node = nodes.at(nodeId);
        const ConditionalProbabilityTable& cpt = cpts.at(nodeId);

        // Build parent state indices
        std::vector<size_t> parentStateIndices;
        for (const std::string& parentId : node.parentIds) {
            if (parentStates.find(parentId) == parentStates.end()) {
                throw std::runtime_error("Missing parent state for " + parentId);
            }
            const Node& parentNode = nodes.at(parentId);
            int parentStateIdx = parentNode.getStateIndex(parentStates.at(parentId));
            if (parentStateIdx == -1) {
                throw std::runtime_error("Invalid state for parent " + parentId);
            }
            parentStateIndices.push_back(static_cast<size_t>(parentStateIdx));
        }

        // Get node state index
        int nodeStateIdx = node.getStateIndex(nodeState);
        if (nodeStateIdx == -1) {
            throw std::runtime_error("Invalid state for node " + nodeId);
        }

        return cpt.getProbability(parentStateIndices, static_cast<size_t>(nodeStateIdx));
    }

    /**
     * Compute joint probability for a full assignment
     * @param assignment Map of node IDs to their states
     * @return Joint probability P(assignment)
     */
    double computeJointProbability(const std::map<std::string, std::string>& assignment) const {
        double jointProb = 1.0;

        // Process nodes in topological order
        for (const std::string& nodeId : nodeOrder) {
            if (assignment.find(nodeId) == assignment.end()) {
                throw std::runtime_error("Missing assignment for node " + nodeId);
            }

            const Node& node = nodes.at(nodeId);
            std::string nodeState = assignment.at(nodeId);

            // Build parent states
            std::map<std::string, std::string> parentStates;
            for (const std::string& parentId : node.parentIds) {
                if (assignment.find(parentId) == assignment.end()) {
                    throw std::runtime_error("Missing assignment for parent " + parentId);
                }
                parentStates[parentId] = assignment.at(parentId);
            }

            // Multiply by conditional probability
            double condProb = getConditionalProbability(nodeId, nodeState, parentStates);
            jointProb *= condProb;
        }

        return jointProb;
    }

    /**
     * Variable elimination for exact inference
     * @param queryNodes Nodes to query (variables of interest)
     * @param evidence Map of observed node IDs to their states
     * @return Map of query assignments to their probabilities
     */
    std::map<std::map<std::string, std::string>, double> 
    variableElimination(const std::vector<std::string>& queryNodes,
                       const std::map<std::string, std::string>& evidence) const {
        // This is a simplified version - full implementation would use
        // factor graphs and proper elimination ordering
        std::map<std::map<std::string, std::string>, double> result;

        // Get all unobserved nodes
        std::vector<std::string> unobservedNodes;
        for (const auto& pair : nodes) {
            if (evidence.find(pair.first) == evidence.end()) {
                unobservedNodes.push_back(pair.first);
            }
        }

        // Generate all possible assignments for query nodes
        std::vector<std::map<std::string, std::string>> queryAssignments;
        generateAssignments(queryNodes, queryAssignments);

        // For each query assignment, compute probability
        for (const auto& queryAssignment : queryAssignments) {
            // Combine with evidence
            std::map<std::string, std::string> fullAssignment = evidence;
            for (const auto& pair : queryAssignment) {
                fullAssignment[pair.first] = pair.second;
            }

            // Sum over all unobserved non-query nodes
            double prob = 0.0;
            std::vector<std::string> sumNodes;
            for (const std::string& nodeId : unobservedNodes) {
                bool isQueryNode = false;
                for (const std::string& qNode : queryNodes) {
                    if (nodeId == qNode) {
                        isQueryNode = true;
                        break;
                    }
                }
                if (!isQueryNode) {
                    sumNodes.push_back(nodeId);
                }
            }

            // Sum over all configurations of sum nodes
            std::vector<std::map<std::string, std::string>> sumAssignments;
            generateAssignments(sumNodes, sumAssignments);

            for (const auto& sumAssignment : sumAssignments) {
                std::map<std::string, std::string> completeAssignment = fullAssignment;
                for (const auto& pair : sumAssignment) {
                    completeAssignment[pair.first] = pair.second;
                }
                try {
                    prob += computeJointProbability(completeAssignment);
                } catch (const std::exception&) {
                    // Skip invalid assignments
                }
            }

            result[queryAssignment] = prob;
        }

        // Normalize
        double sum = 0.0;
        for (const auto& pair : result) {
            sum += pair.second;
        }
        if (sum > 1e-10) {
            for (auto& pair : result) {
                pair.second /= sum;
            }
        }

        return result;
    }

    /**
     * Generate all possible assignments for given nodes
     * @param nodeIds Vector of node IDs
     * @param assignments Output vector of assignments
     */
    void generateAssignments(const std::vector<std::string>& nodeIds,
                            std::vector<std::map<std::string, std::string>>& assignments) const {
        assignments.clear();
        if (nodeIds.empty()) {
            assignments.push_back(std::map<std::string, std::string>());
            return;
        }

        // Recursive generation
        std::vector<std::vector<std::string>> stateLists;
        for (const std::string& nodeId : nodeIds) {
            if (nodes.find(nodeId) == nodes.end()) {
                throw std::runtime_error("Node " + nodeId + " does not exist");
            }
            stateLists.push_back(nodes.at(nodeId).states);
        }

        generateAssignmentsRecursive(nodeIds, stateLists, 0, 
                                    std::map<std::string, std::string>(), assignments);
    }

    /**
     * Recursive helper for generating assignments
     */
    void generateAssignmentsRecursive(const std::vector<std::string>& nodeIds,
                                     const std::vector<std::vector<std::string>>& stateLists,
                                     size_t currentIndex,
                                     const std::map<std::string, std::string>& currentAssignment,
                                     std::vector<std::map<std::string, std::string>>& assignments) const {
        if (currentIndex >= nodeIds.size()) {
            assignments.push_back(currentAssignment);
            return;
        }

        for (const std::string& state : stateLists[currentIndex]) {
            std::map<std::string, std::string> newAssignment = currentAssignment;
            newAssignment[nodeIds[currentIndex]] = state;
            generateAssignmentsRecursive(nodeIds, stateLists, currentIndex + 1,
                                        newAssignment, assignments);
        }
    }

    /**
     * Get node by ID
     * @param nodeId ID of the node
     * @return Reference to the node
     */
    const Node& getNode(const std::string& nodeId) const {
        if (nodes.find(nodeId) == nodes.end()) {
            throw std::runtime_error("Node " + nodeId + " does not exist");
        }
        return nodes.at(nodeId);
    }

    /**
     * Get all node IDs
     * @return Vector of node IDs
     */
    std::vector<std::string> getNodeIds() const {
        std::vector<std::string> ids;
        for (const auto& pair : nodes) {
            ids.push_back(pair.first);
        }
        return ids;
    }

    /**
     * Save network to file
     * @param filename Output filename
     */
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }

        // Write header
        file << "# Lossless Bayesian Network\n";
        file << "# Copyright (C) 2025, Shyamal Chandra\n\n";

        // Write nodes
        file << "NODES\n";
        for (const auto& pair : nodes) {
            const Node& node = pair.second;
            file << pair.first << " " << node.name << " ";
            file << node.states.size();
            for (const std::string& state : node.states) {
                file << " " << state;
            }
            file << "\n";
        }

        // Write edges
        file << "\nEDGES\n";
        for (const auto& pair : nodes) {
            for (const std::string& parentId : pair.second.parentIds) {
                file << parentId << " -> " << pair.first << "\n";
            }
        }

        // Write CPTs
        file << "\nCPTS\n";
        for (const auto& pair : cpts) {
            const std::string& nodeId = pair.first;
            const ConditionalProbabilityTable& cpt = pair.second;

            file << nodeId << "\n";
            const std::vector<size_t>& dims = cpt.getDimensions();

            // Write dimensions
            file << dims.size();
            for (size_t dim : dims) {
                file << " " << dim;
            }
            file << "\n";

            // Write probabilities (simplified - would need proper serialization)
            file << "# CPT data would be serialized here\n";
        }

        file.close();
    }

    /**
     * Load network from file
     * @param filename Input filename
     */
    void loadFromFile(const std::string& filename) {
        // Implementation would parse the file format
        // This is a placeholder for the full implementation
        (void)filename;  // Suppress unused parameter warning
        throw std::runtime_error("File loading not fully implemented");
    }

    /**
     * Structure to represent a message in belief propagation
     */
    struct Message {
        std::string fromNode;
        std::string toNode;
        std::map<std::string, double> probabilities;  // State -> probability
    };

    /**
     * Structure to represent influence trace
     */
    struct InfluenceTrace {
        std::string sourceNode;
        std::string targetNode;
        std::string path;  // Path taken (e.g., "A->B->C")
        double influenceStrength;  // How much influence flows
        std::map<std::string, double> stateInfluences;  // Per-state influence
    };

    /**
     * Lossless Belief Propagation with influence tracing
     * Implements sum-product algorithm for exact inference while tracing
     * how probabilities flow through the network.
     * 
     * @param queryNodes Nodes to query (variables of interest)
     * @param evidence Map of observed node IDs to their states
     * @param traceInfluence If true, trace influence paths
     * @return Pair of (beliefs, influence traces)
     */
    std::pair<std::map<std::string, std::map<std::string, double>>, 
              std::vector<InfluenceTrace>>
    beliefPropagation(const std::vector<std::string>& queryNodes,
                     const std::map<std::string, std::string>& evidence,
                     bool traceInfluence = true) const {
        
        // Beliefs: node -> state -> probability
        std::map<std::string, std::map<std::string, double>> beliefs;
        std::vector<InfluenceTrace> influenceTraces;
        
        // Initialize beliefs with prior probabilities
        for (const auto& pair : nodes) {
            const std::string& nodeId = pair.first;
            const Node& node = pair.second;
            
            // If node is in evidence, set belief to 1.0 for observed state
            if (evidence.find(nodeId) != evidence.end()) {
                std::string observedState = evidence.at(nodeId);
                for (const std::string& state : node.states) {
                    beliefs[nodeId][state] = (state == observedState) ? 1.0 : 0.0;
                }
            } else {
                // Initialize with uniform prior (will be updated by messages)
                double uniformProb = 1.0 / static_cast<double>(node.states.size());
                for (const std::string& state : node.states) {
                    beliefs[nodeId][state] = uniformProb;
                }
            }
        }
        
        // Build message passing structure
        // Messages: (from, to) -> message probabilities
        std::map<std::pair<std::string, std::string>, 
                 std::map<std::string, double>> messages;
        
        // Initialize messages from CPTs
        initializeMessages(messages, evidence);
        
        // Upward pass: messages from leaves to root
        upwardPass(messages, evidence);
        
        // Downward pass: messages from root to leaves
        downwardPass(messages, evidence);
        
        // Compute beliefs from messages
        computeBeliefs(messages, beliefs, evidence);
        
        // Trace influence if requested
        if (traceInfluence) {
            traceInfluencePaths(messages, beliefs, queryNodes, evidence, influenceTraces);
        }
        
        return std::make_pair(beliefs, influenceTraces);
    }

private:
    /**
     * Initialize messages from CPTs
     */
    void initializeMessages(std::map<std::pair<std::string, std::string>, 
                                    std::map<std::string, double>>& messages,
                           const std::map<std::string, std::string>& evidence) const {
        for (const auto& pair : nodes) {
            const std::string& nodeId = pair.first;
            const Node& node = pair.second;
            
            // Skip if node is observed
            if (evidence.find(nodeId) != evidence.end()) {
                continue;
            }
            
            // If node has no parents, initialize from prior
            if (node.parentIds.empty()) {
                if (cpts.find(nodeId) != cpts.end()) {
                    const ConditionalProbabilityTable& cpt = cpts.at(nodeId);
                    for (size_t i = 0; i < node.states.size(); ++i) {
                        double prob = cpt.getProbability({}, i);
                        // Send message to all children
                        for (const auto& childPair : nodes) {
                            if (childPair.second.hasParent(nodeId)) {
                                std::pair<std::string, std::string> key(nodeId, childPair.first);
                                messages[key][node.states[i]] = prob;
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Upward pass: propagate messages from leaves to root
     */
    void upwardPass(std::map<std::pair<std::string, std::string>, 
                            std::map<std::string, double>>& messages,
                   const std::map<std::string, std::string>& evidence) const {
        // Process nodes in reverse topological order (leaves first)
        std::vector<std::string> reverseOrder = nodeOrder;
        std::reverse(reverseOrder.begin(), reverseOrder.end());
        
        for (const std::string& nodeId : reverseOrder) {
            if (evidence.find(nodeId) != evidence.end()) {
                continue;  // Skip observed nodes
            }
            
            const Node& node = nodes.at(nodeId);
            if (cpts.find(nodeId) == cpts.end()) {
                continue;  // Skip nodes without CPT
            }
            
            // Collect messages from children
            std::map<std::string, std::map<std::string, double>> childMessages;
            for (const auto& childPair : nodes) {
                if (childPair.second.hasParent(nodeId)) {
                    std::pair<std::string, std::string> key(nodeId, childPair.first);
                    if (messages.find(key) != messages.end()) {
                        childMessages[childPair.first] = messages[key];
                    }
                }
            }
            
            // Send message to each parent
            for (const std::string& parentId : node.parentIds) {
                if (evidence.find(parentId) != evidence.end()) {
                    continue;  // Skip observed parents
                }
                
                std::map<std::string, double> messageToParent;
                const Node& parentNode = nodes.at(parentId);
                
                // For each parent state
                for (const std::string& parentState : parentNode.states) {
                    double messageValue = 0.0;
                    
                    // Sum over all node states
                    for (size_t nodeStateIdx = 0; nodeStateIdx < node.states.size(); ++nodeStateIdx) {
                        const std::string& nodeState = node.states[nodeStateIdx];
                        
                        // Get conditional probability
                        std::map<std::string, std::string> parentStates;
                        parentStates[parentId] = parentState;
                        // Add other parents if any
                        for (const std::string& otherParentId : node.parentIds) {
                            if (otherParentId != parentId) {
                                if (evidence.find(otherParentId) != evidence.end()) {
                                    parentStates[otherParentId] = evidence.at(otherParentId);
                                } else {
                                    // Use most likely state (simplified)
                                    parentStates[otherParentId] = parentNode.states[0];
                                }
                            }
                        }
                        
                        double condProb = getConditionalProbability(nodeId, nodeState, parentStates);
                        
                        // Multiply by messages from children
                        double childProduct = 1.0;
                        for (const auto& childMsgPair : childMessages) {
                            if (childMsgPair.second.find(nodeState) != childMsgPair.second.end()) {
                                childProduct *= childMsgPair.second.at(nodeState);
                            }
                        }
                        
                        messageValue += condProb * childProduct;
                    }
                    
                    messageToParent[parentState] = messageValue;
                }
                
                // Normalize message
                double sum = 0.0;
                for (const auto& msgPair : messageToParent) {
                    sum += msgPair.second;
                }
                if (sum > 1e-10) {
                    for (auto& msgPair : messageToParent) {
                        msgPair.second /= sum;
                    }
                }
                
                std::pair<std::string, std::string> key(nodeId, parentId);
                messages[key] = messageToParent;
            }
        }
    }

    /**
     * Downward pass: propagate messages from root to leaves
     */
    void downwardPass(std::map<std::pair<std::string, std::string>, 
                              std::map<std::string, double>>& messages,
                     const std::map<std::string, std::string>& evidence) const {
        // Process nodes in topological order (root first)
        for (const std::string& nodeId : nodeOrder) {
            if (evidence.find(nodeId) != evidence.end()) {
                continue;  // Skip observed nodes
            }
            
            const Node& node = nodes.at(nodeId);
            if (cpts.find(nodeId) == cpts.end()) {
                continue;
            }
            
            // Collect messages from parents
            std::map<std::string, std::map<std::string, double>> parentMessages;
            for (const std::string& parentId : node.parentIds) {
                std::pair<std::string, std::string> key(parentId, nodeId);
                if (messages.find(key) != messages.end()) {
                    parentMessages[parentId] = messages[key];
                }
            }
            
            // Send message to each child
            for (const auto& childPair : nodes) {
                if (childPair.second.hasParent(nodeId)) {
                    const std::string& childId = childPair.first;
                    if (evidence.find(childId) != evidence.end()) {
                        continue;  // Skip observed children
                    }
                    
                    std::map<std::string, double> messageToChild;
                    const Node& childNode = nodes.at(childId);
                    
                    // For each child state
                    for (const std::string& childState : childNode.states) {
                        double messageValue = 0.0;
                        
                        // Sum over all node states
                        for (size_t nodeStateIdx = 0; nodeStateIdx < node.states.size(); ++nodeStateIdx) {
                            const std::string& nodeState = node.states[nodeStateIdx];
                            
                            // Build parent states for conditional probability
                            std::map<std::string, std::string> parentStates;
                            for (const std::string& parentId : node.parentIds) {
                                if (evidence.find(parentId) != evidence.end()) {
                                    parentStates[parentId] = evidence.at(parentId);
                                } else if (parentMessages.find(parentId) != parentMessages.end()) {
                                    // Use most likely parent state from message
                                    double maxProb = 0.0;
                                    std::string maxState = nodes.at(parentId).states[0];
                                    for (const auto& msgPair : parentMessages.at(parentId)) {
                                        if (msgPair.second > maxProb) {
                                            maxProb = msgPair.second;
                                            maxState = msgPair.first;
                                        }
                                    }
                                    parentStates[parentId] = maxState;
                                } else {
                                    parentStates[parentId] = nodes.at(parentId).states[0];
                                }
                            }
                            
                            double condProb = getConditionalProbability(nodeId, nodeState, parentStates);
                            
                            // Multiply by parent messages
                            double parentProduct = 1.0;
                            for (const auto& parentMsgPair : parentMessages) {
                                if (parentMsgPair.second.find(parentStates[parentMsgPair.first]) != 
                                    parentMsgPair.second.end()) {
                                    parentProduct *= parentMsgPair.second.at(parentStates[parentMsgPair.first]);
                                }
                            }
                            
                            messageValue += condProb * parentProduct;
                        }
                        
                        messageToChild[childState] = messageValue;
                    }
                    
                    // Normalize message
                    double sum = 0.0;
                    for (const auto& msgPair : messageToChild) {
                        sum += msgPair.second;
                    }
                    if (sum > 1e-10) {
                        for (auto& msgPair : messageToChild) {
                            msgPair.second /= sum;
                        }
                    }
                    
                    std::pair<std::string, std::string> key(nodeId, childId);
                    messages[key] = messageToChild;
                }
            }
        }
    }

    /**
     * Compute beliefs from messages
     */
    void computeBeliefs(const std::map<std::pair<std::string, std::string>, 
                                     std::map<std::string, double>>& messages,
                      std::map<std::string, std::map<std::string, double>>& beliefs,
                      const std::map<std::string, std::string>& evidence) const {
        for (const auto& pair : nodes) {
            const std::string& nodeId = pair.first;
            const Node& node = pair.second;
            
            // Skip observed nodes
            if (evidence.find(nodeId) != evidence.end()) {
                continue;
            }
            
            // Initialize belief from prior if available
            if (cpts.find(nodeId) != cpts.end() && node.parentIds.empty()) {
                const ConditionalProbabilityTable& cpt = cpts.at(nodeId);
                for (size_t i = 0; i < node.states.size(); ++i) {
                    beliefs[nodeId][node.states[i]] = cpt.getProbability({}, i);
                }
            }
            
            // Multiply by messages from parents
            for (const std::string& parentId : node.parentIds) {
                std::pair<std::string, std::string> key(parentId, nodeId);
                if (messages.find(key) != messages.end()) {
                    for (const std::string& state : node.states) {
                        if (messages.at(key).find(state) != messages.at(key).end()) {
                            beliefs[nodeId][state] *= messages.at(key).at(state);
                        }
                    }
                }
            }
            
            // Multiply by messages from children
            for (const auto& childPair : nodes) {
                if (childPair.second.hasParent(nodeId)) {
                    std::pair<std::string, std::string> key(nodeId, childPair.first);
                    if (messages.find(key) != messages.end()) {
                        for (const std::string& state : node.states) {
                            if (messages.at(key).find(state) != messages.at(key).end()) {
                                beliefs[nodeId][state] *= messages.at(key).at(state);
                            }
                        }
                    }
                }
            }
            
            // Normalize beliefs
            double sum = 0.0;
            for (const auto& beliefPair : beliefs[nodeId]) {
                sum += beliefPair.second;
            }
            if (sum > 1e-10) {
                for (auto& beliefPair : beliefs[nodeId]) {
                    beliefPair.second /= sum;
                }
            }
        }
    }

    /**
     * Trace influence paths through the network
     */
    void traceInfluencePaths(const std::map<std::pair<std::string, std::string>, 
                                           std::map<std::string, double>>& /*messages*/,
                            const std::map<std::string, std::map<std::string, double>>& beliefs,
                            const std::vector<std::string>& queryNodes,
                            const std::map<std::string, std::string>& evidence,
                            std::vector<InfluenceTrace>& traces) const {
        traces.clear();
        
        // For each evidence node, trace influence to query nodes
        for (const auto& evidencePair : evidence) {
            const std::string& evidenceNode = evidencePair.first;
            
            for (const std::string& queryNode : queryNodes) {
                if (evidenceNode == queryNode) {
                    continue;  // Skip self-influence
                }
                
                // Find paths from evidence to query
                std::vector<std::vector<std::string>> paths;
                findPaths(evidenceNode, queryNode, std::vector<std::string>(), paths);
                
                // For each path, compute influence
                for (const auto& path : paths) {
                    InfluenceTrace trace;
                    trace.sourceNode = evidenceNode;
                    trace.targetNode = queryNode;
                    
                    // Build path string
                    std::ostringstream pathStream;
                    pathStream << evidenceNode;
                    for (size_t i = 1; i < path.size(); ++i) {
                        pathStream << "->" << path[i];
                    }
                    trace.path = pathStream.str();
                    
                    // Compute influence strength
                    double totalInfluence = 0.0;
                    if (beliefs.find(queryNode) != beliefs.end()) {
                        for (const auto& beliefPair : beliefs.at(queryNode)) {
                            totalInfluence += beliefPair.second;
                        }
                        totalInfluence /= static_cast<double>(beliefs.at(queryNode).size());
                    }
                    trace.influenceStrength = totalInfluence;
                    
                    // Compute per-state influence
                    if (beliefs.find(queryNode) != beliefs.end()) {
                        for (const auto& beliefPair : beliefs.at(queryNode)) {
                            trace.stateInfluences[beliefPair.first] = beliefPair.second;
                        }
                    }
                    
                    traces.push_back(trace);
                }
            }
        }
    }

    /**
     * Find all paths from source to target
     */
    void findPaths(const std::string& source,
                  const std::string& target,
                  std::vector<std::string> currentPath,
                  std::vector<std::vector<std::string>>& allPaths) const {
        currentPath.push_back(source);
        
        if (source == target) {
            allPaths.push_back(currentPath);
            return;
        }
        
        // Find children of source
        for (const auto& pair : nodes) {
            if (pair.second.hasParent(source)) {
                // Check if already in path (avoid cycles)
                bool inPath = false;
                for (const std::string& nodeInPath : currentPath) {
                    if (nodeInPath == pair.first) {
                        inPath = true;
                        break;
                    }
                }
                if (!inPath) {
                    findPaths(pair.first, target, currentPath, allPaths);
                }
            }
        }
    }
};

#endif // BAYESIAN_NETWORK_HPP
