/*
 * cpt.hpp - Conditional Probability Table implementation
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This file implements the Conditional Probability Table (CPT) for storing
 * conditional probability distributions in a lossless manner.
 */

#ifndef CPT_HPP
#define CPT_HPP

// Vector container
#include <vector>
// String operations
#include <string>
// Map container
#include <map>
// Exception handling
#include <stdexcept>
// String stream operations
#include <sstream>
// Input/output manipulators
#include <iomanip>
// Mathematical operations
#include <cmath>

/**
 * ConditionalProbabilityTable class stores conditional probabilities
 * in a lossless, exact representation using multi-dimensional indexing.
 */
class ConditionalProbabilityTable {
private:
    // Flat storage of probabilities
    std::vector<double> probabilities;
    // Dimensions for each parent + self
    std::vector<size_t> dimensions;
    // Stride values for multi-dimensional indexing
    std::vector<size_t> strides;
    // Total number of probability entries
    size_t totalSize;

    /**
     * Calculate strides for multi-dimensional array indexing
     */
    void calculateStrides() {
        strides.resize(dimensions.size());
        if (dimensions.empty()) {
            return;
        }
        // Last dimension has stride 1
        strides[dimensions.size() - 1] = 1;
        // Calculate strides from right to left
        for (int i = static_cast<int>(dimensions.size()) - 2; i >= 0; --i) {
            strides[i] = strides[i + 1] * dimensions[i + 1];
        }
    }

    /**
     * Convert multi-dimensional indices to flat index
     * @param indices Vector of indices for each dimension
     * @return Flat index into probability array
     */
    size_t getFlatIndex(const std::vector<size_t>& indices) const {
        if (indices.size() != dimensions.size()) {
            throw std::runtime_error("Index dimension mismatch");
        }
        size_t index = 0;
        for (size_t i = 0; i < indices.size(); ++i) {
            if (indices[i] >= dimensions[i]) {
                throw std::runtime_error("Index out of bounds");
            }
            index += indices[i] * strides[i];
        }
        return index;
    }

public:
    /**
     * Default constructor
     */
    ConditionalProbabilityTable() : totalSize(0) {}

    /**
     * Constructor with dimensions
     * @param dims Vector of dimensions (last is node, others are parents)
     */
    ConditionalProbabilityTable(const std::vector<size_t>& dims) 
        : dimensions(dims), totalSize(1) {
        // Calculate total size
        for (size_t dim : dimensions) {
            totalSize *= dim;
        }
        // Initialize to zero
        probabilities.resize(totalSize, 0.0);
        calculateStrides();
    }

    /**
     * Set probability for given parent and node state indices
     * @param parentStates Vector of parent state indices
     * @param nodeState Index of node state
     * @param prob Probability value (must be in [0, 1])
     */
    void setProbability(const std::vector<size_t>& parentStates, 
                       size_t nodeState, 
                       double prob) {
        if (prob < 0.0 || prob > 1.0) {
            throw std::runtime_error("Probability must be in [0, 1]");
        }
        std::vector<size_t> indices = parentStates;
        indices.push_back(nodeState);
        size_t idx = getFlatIndex(indices);
        probabilities[idx] = prob;
    }

    /**
     * Get probability for given parent and node state indices
     * @param parentStates Vector of parent state indices
     * @param nodeState Index of node state
     * @return Probability value
     */
    double getProbability(const std::vector<size_t>& parentStates, 
                         size_t nodeState) const {
        std::vector<size_t> indices = parentStates;
        indices.push_back(nodeState);
        size_t idx = getFlatIndex(indices);
        return probabilities[idx];
    }

    /**
     * Normalize probabilities for each parent configuration
     * Ensures each conditional distribution sums to 1.0
     */
    void normalize() {
        size_t numParentConfigs = 1;
        for (size_t i = 0; i < dimensions.size() - 1; ++i) {
            numParentConfigs *= dimensions[i];
        }
        size_t numNodeStates = dimensions.back();

        // Normalize each parent configuration
        for (size_t parentConfig = 0; parentConfig < numParentConfigs; ++parentConfig) {
            double sum = 0.0;
            // Calculate sum for this parent configuration
            for (size_t nodeState = 0; nodeState < numNodeStates; ++nodeState) {
                std::vector<size_t> indices;
                // Reconstruct parent indices from parentConfig
                size_t temp = parentConfig;
                for (size_t i = 0; i < dimensions.size() - 1; ++i) {
                    indices.push_back(temp % dimensions[i]);
                    temp /= dimensions[i];
                }
                indices.push_back(nodeState);
                sum += probabilities[getFlatIndex(indices)];
            }

            // Normalize if sum > 0
            if (sum > 1e-10) {
                for (size_t nodeState = 0; nodeState < numNodeStates; ++nodeState) {
                    std::vector<size_t> indices;
                    size_t temp = parentConfig;
                    for (size_t i = 0; i < dimensions.size() - 1; ++i) {
                        indices.push_back(temp % dimensions[i]);
                        temp /= dimensions[i];
                    }
                    indices.push_back(nodeState);
                    size_t idx = getFlatIndex(indices);
                    probabilities[idx] /= sum;
                }
            }
        }
    }

    /**
     * Validate that all conditional distributions sum to 1.0
     * @param tolerance Tolerance for floating point comparison
     * @return True if valid, false otherwise
     */
    bool isValid(double tolerance = 1e-6) const {
        size_t numParentConfigs = 1;
        for (size_t i = 0; i < dimensions.size() - 1; ++i) {
            numParentConfigs *= dimensions[i];
        }
        size_t numNodeStates = dimensions.back();

        // Check each parent configuration
        for (size_t parentConfig = 0; parentConfig < numParentConfigs; ++parentConfig) {
            double sum = 0.0;
            for (size_t nodeState = 0; nodeState < numNodeStates; ++nodeState) {
                std::vector<size_t> indices;
                size_t temp = parentConfig;
                for (size_t i = 0; i < dimensions.size() - 1; ++i) {
                    indices.push_back(temp % dimensions[i]);
                    temp /= dimensions[i];
                }
                indices.push_back(nodeState);
                sum += probabilities[getFlatIndex(indices)];
            }
            if (std::abs(sum - 1.0) > tolerance) {
                return false;
            }
        }
        return true;
    }

    /**
     * Get dimensions
     * @return Vector of dimensions
     */
    const std::vector<size_t>& getDimensions() const {
        return dimensions;
    }

    /**
     * Get total number of entries
     * @return Total size
     */
    size_t getTotalSize() const {
        return totalSize;
    }
};

#endif // CPT_HPP
