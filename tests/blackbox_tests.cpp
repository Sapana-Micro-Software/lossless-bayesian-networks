/*
 * blackbox_tests.cpp - Blackbox Integration Tests
 * Copyright (C) 2025, Shyamal Chandra
 *
 * Tests the system as a whole without knowing internal implementation
 */

#include "test_framework.hpp"
#include "../bayesian_network.hpp"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <chrono>

void runEndToEndTests(TestSuite& suite) {
    suite.runTest("Complete workflow: build network and infer", []() {
        // Blackbox: Test complete workflow without knowing internals
        BayesianNetwork network;
        
        // Build network
        network.addNode("Cause", "Cause", {"False", "True"});
        network.addNode("Effect", "Effect", {"Negative", "Positive"});
        network.addEdge("Cause", "Effect");
        
        // Set probabilities
        std::vector<size_t> causeDims = {2};
        ConditionalProbabilityTable causeCPT(causeDims);
        causeCPT.setProbability({}, 0, 0.7);
        causeCPT.setProbability({}, 1, 0.3);
        network.setCPT("Cause", causeCPT);
        
        std::vector<size_t> effectDims = {2, 2};
        ConditionalProbabilityTable effectCPT(effectDims);
        effectCPT.setProbability({0}, 0, 0.9);
        effectCPT.setProbability({0}, 1, 0.1);
        effectCPT.setProbability({1}, 0, 0.2);
        effectCPT.setProbability({1}, 1, 0.8);
        effectCPT.normalize();
        network.setCPT("Effect", effectCPT);
        
        // Perform inference
        std::map<std::string, std::string> evidence;
        evidence["Effect"] = "Positive";
        std::vector<std::string> query = {"Cause"};
        
        auto results = network.variableElimination(query, evidence);
        
        // Blackbox: Check that results make sense
        double sum = 0.0;
        for (const auto& pair : results) {
            sum += pair.second;
            // Probabilities should be in [0, 1]
            if (pair.second < 0.0 || pair.second > 1.0) {
                return false;
            }
        }
        
        return TestSuite::assertEqual(sum, 1.0, 1e-4) &&
               TestSuite::assertTrue(results.size() == 2);
    });

    suite.runTest("Multiple inference methods produce valid results", []() {
        BayesianNetwork network;
        network.addNode("A", "A", {"X", "Y"});
        network.addNode("B", "B", {"P", "Q"});
        network.addEdge("A", "B");
        
        // Set up CPTs
        std::vector<size_t> aDims = {2};
        ConditionalProbabilityTable aCPT(aDims);
        aCPT.setProbability({}, 0, 0.6);
        aCPT.setProbability({}, 1, 0.4);
        network.setCPT("A", aCPT);
        
        std::vector<size_t> bDims = {2, 2};
        ConditionalProbabilityTable bCPT(bDims);
        bCPT.setProbability({0}, 0, 0.8);
        bCPT.setProbability({0}, 1, 0.2);
        bCPT.setProbability({1}, 0, 0.3);
        bCPT.setProbability({1}, 1, 0.7);
        bCPT.normalize();
        network.setCPT("B", bCPT);
        
        std::map<std::string, std::string> evidence;
        evidence["B"] = "Q";
        std::vector<std::string> query = {"A"};
        
        // Test all three inference methods
        auto veResults = network.variableElimination(query, evidence);
        auto bpResult = network.beliefPropagation(query, evidence, false);
        auto reverseResult = network.reverseBeliefPropagation(query, evidence, false);
        
        // Blackbox: All should produce valid probability distributions
        bool veValid = true, bpValid = true, reverseValid = true;
        
        double veSum = 0.0;
        for (const auto& pair : veResults) {
            veSum += pair.second;
            if (pair.second < 0.0 || pair.second > 1.0) veValid = false;
        }
        
        if (bpResult.first.find("A") != bpResult.first.end()) {
            double bpSum = 0.0;
            for (const auto& pair : bpResult.first.at("A")) {
                bpSum += pair.second;
                if (pair.second < 0.0 || pair.second > 1.0) bpValid = false;
            }
            if (std::abs(bpSum - 1.0) > 1e-4) bpValid = false;
        }
        
        if (reverseResult.first.find("A") != reverseResult.first.end()) {
            double reverseSum = 0.0;
            for (const auto& pair : reverseResult.first.at("A")) {
                reverseSum += pair.second;
                if (pair.second < 0.0 || pair.second > 1.0) reverseValid = false;
            }
            if (std::abs(reverseSum - 1.0) > 1e-4) reverseValid = false;
        }
        
        return TestSuite::assertTrue(veValid && bpValid && reverseValid) &&
               TestSuite::assertEqual(veSum, 1.0, 1e-4);
    });
}

void runRobustnessTests(TestSuite& suite) {
    suite.runTest("Handles edge cases gracefully", []() {
        // Blackbox: System should handle edge cases
        BayesianNetwork network;
        
        // Single node network
        network.addNode("A", "A", {"X"});
        std::vector<size_t> dims = {1};
        ConditionalProbabilityTable cpt(dims);
        cpt.setProbability({}, 0, 1.0);
        network.setCPT("A", cpt);
        
        // Should be able to query it
        std::map<std::string, std::string> evidence;
        std::vector<std::string> query = {"A"};
        auto results = network.variableElimination(query, evidence);
        
        return TestSuite::assertTrue(results.size() == 1);
    });

    suite.runTest("Handles complex networks", []() {
        // Blackbox: Should handle more complex structures
        BayesianNetwork network;
        
        // Create a chain: A -> B -> C -> D
        network.addNode("A", "A", {"X"});
        network.addNode("B", "B", {"X"});
        network.addNode("C", "C", {"X"});
        network.addNode("D", "D", {"X"});
        
        network.addEdge("A", "B");
        network.addEdge("B", "C");
        network.addEdge("C", "D");
        
        // Set up simple CPTs
        std::vector<size_t> dims1 = {1};
        ConditionalProbabilityTable cpt1(dims1);
        cpt1.setProbability({}, 0, 1.0);
        network.setCPT("A", cpt1);
        
        std::vector<size_t> dims2 = {1, 1};
        ConditionalProbabilityTable cpt2(dims2);
        cpt2.setProbability({0}, 0, 1.0);
        network.setCPT("B", cpt2);
        network.setCPT("C", cpt2);
        network.setCPT("D", cpt2);
        
        // Should be able to perform inference
        std::map<std::string, std::string> evidence;
        evidence["D"] = "X";
        std::vector<std::string> query = {"A"};
        
        auto results = network.variableElimination(query, evidence);
        
        return TestSuite::assertTrue(results.size() == 1);
    });
}

void runPerformanceTests(TestSuite& suite) {
    suite.runTest("Reasonable performance for small networks", []() {
        // Blackbox: Should complete in reasonable time
        BayesianNetwork network;
        
        // Create a small network
        network.addNode("A", "A", {"False", "True"});
        network.addNode("B", "B", {"Low", "High"});
        network.addNode("C", "C", {"Negative", "Positive"});
        network.addEdge("A", "B");
        network.addEdge("B", "C");
        
        // Set up CPTs
        std::vector<size_t> aDims = {2};
        ConditionalProbabilityTable aCPT(aDims);
        aCPT.setProbability({}, 0, 0.7);
        aCPT.setProbability({}, 1, 0.3);
        network.setCPT("A", aCPT);
        
        std::vector<size_t> bDims = {2, 2};
        ConditionalProbabilityTable bCPT(bDims);
        bCPT.setProbability({0}, 0, 0.8);
        bCPT.setProbability({0}, 1, 0.2);
        bCPT.setProbability({1}, 0, 0.3);
        bCPT.setProbability({1}, 1, 0.7);
        bCPT.normalize();
        network.setCPT("B", bCPT);
        
        std::vector<size_t> cDims = {2, 2};
        ConditionalProbabilityTable cCPT(cDims);
        cCPT.setProbability({0}, 0, 0.9);
        cCPT.setProbability({0}, 1, 0.1);
        cCPT.setProbability({1}, 0, 0.2);
        cCPT.setProbability({1}, 1, 0.8);
        cCPT.normalize();
        network.setCPT("C", cCPT);
        
        // Perform inference multiple times
        std::map<std::string, std::string> evidence;
        evidence["C"] = "Positive";
        std::vector<std::string> query = {"A"};
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10; ++i) {
            network.variableElimination(query, evidence);
        }
        auto end = std::chrono::high_resolution_clock::now();
        
        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
        
        // Should complete 10 inferences in less than 1 second
        return TestSuite::assertTrue(elapsed < 1000.0, "Should be reasonably fast");
    });
}

int main() {
    std::cout << "=== Blackbox Integration Tests ===" << std::endl;
    
    TestSuite suite("Blackbox Tests");
    
    std::cout << "\nEnd-to-End Tests:" << std::endl;
    runEndToEndTests(suite);
    
    std::cout << "\nRobustness Tests:" << std::endl;
    runRobustnessTests(suite);
    
    std::cout << "\nPerformance Tests:" << std::endl;
    runPerformanceTests(suite);
    
    suite.printSummary();
    
    return suite.allPassed() ? 0 : 1;
}
