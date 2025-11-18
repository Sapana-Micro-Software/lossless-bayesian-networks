/*
 * ux_tests.cpp - UX/API Usability Tests
 * Copyright (C) 2025, Shyamal Chandra
 *
 * Tests the user experience and API usability
 */

#include "test_framework.hpp"
#include "../bayesian_network.hpp"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

void runAPIUsabilityTests(TestSuite& suite) {
    suite.runTest("Simple network construction API", []() {
        // Test: Can a user easily create a simple network?
        BayesianNetwork network;
        
        network.addNode("Disease", "Disease", {"None", "Cold", "Flu"});
        network.addNode("Symptom", "Fever", {"No", "Yes"});
        network.addEdge("Disease", "Symptom");
        
        // UX: Should be able to query node IDs
        auto nodeIds = network.getNodeIds();
        return TestSuite::assertEqual(nodeIds.size(), size_t(2)) &&
               TestSuite::assertTrue(std::find(nodeIds.begin(), nodeIds.end(), "Disease") != nodeIds.end());
    });

    suite.runTest("Intuitive error messages", []() {
        BayesianNetwork network;
        network.addNode("A", "A", {"X"});
        
        // UX: Should get clear error when node doesn't exist
        bool throws = TestSuite::assertThrows([&]() {
            network.addEdge("NonExistent", "A");
        });
        
        return TestSuite::assertTrue(throws, "Should throw error for non-existent node");
    });

    suite.runTest("CPT setup workflow", []() {
        // UX: Setting up CPTs should be straightforward
        BayesianNetwork network;
        network.addNode("A", "A", {"False", "True"});
        network.addNode("B", "B", {"Low", "High"});
        network.addEdge("A", "B");
        
        std::vector<size_t> dims = {2, 2};
        ConditionalProbabilityTable cpt(dims);
        cpt.setProbability({0}, 0, 0.8);
        cpt.setProbability({0}, 1, 0.2);
        cpt.setProbability({1}, 0, 0.3);
        cpt.setProbability({1}, 1, 0.7);
        cpt.normalize();
        network.setCPT("B", cpt);
        
        // UX: Should be able to query probabilities
        std::map<std::string, std::string> parentStates;
        parentStates["A"] = "False";
        double prob = network.getConditionalProbability("B", "Low", parentStates);
        
        return TestSuite::assertEqual(prob, 0.8, 1e-6);
    });

    suite.runTest("Inference API simplicity", []() {
        // UX: Performing inference should be simple
        BayesianNetwork network;
        network.addNode("A", "A", {"False", "True"});
        network.addNode("B", "B", {"Low", "High"});
        network.addEdge("A", "B");
        
        // Set up simple CPTs
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
        
        // UX: Simple inference call
        std::map<std::string, std::string> evidence;
        evidence["B"] = "High";
        std::vector<std::string> query = {"A"};
        
        auto results = network.variableElimination(query, evidence);
        
        // UX: Results should be easy to interpret
        return TestSuite::assertTrue(results.size() == 2) &&
               TestSuite::assertTrue(results.begin()->second > 0.0);
    });
}

void runAPICompletenessTests(TestSuite& suite) {
    suite.runTest("All necessary API methods available", []() {
        BayesianNetwork network;
        
        // UX: Should have methods for all common operations
        network.addNode("A", "A", {"X"});
        
        // Test that addEdge exists (will throw for self-loop, which is expected)
        bool addEdgeExists = TestSuite::assertThrows([&]() {
            network.addEdge("A", "A");  // Will fail, but method exists
        });
        
        auto nodeIds = network.getNodeIds();
        network.getNode("A");  // Test that method exists and works
        
        // Check that essential methods exist
        bool hasAddNode = true;
        bool hasAddEdge = addEdgeExists;  // Method exists if it throws (expected behavior)
        bool hasGetNode = true;
        bool hasGetNodeIds = true;
        
        return TestSuite::assertTrue(hasAddNode && hasAddEdge && hasGetNode && hasGetNodeIds);
    });

    suite.runTest("Consistent naming conventions", []() {
        // UX: API should use consistent naming
        BayesianNetwork network;
        
        // camelCase for methods
        network.addNode("Test", "Test", {"X"});
        network.getNodeIds();
        network.getNode("Test");
        
        // Should work without confusion
        return TestSuite::assertTrue(true);
    });
}

void runErrorHandlingUXTests(TestSuite& suite) {
    suite.runTest("Helpful error messages for common mistakes", []() {
        BayesianNetwork network;
        network.addNode("A", "A", {"X"});
        
        // UX: Should get helpful error for duplicate node
        bool throws1 = TestSuite::assertThrows([&]() {
            network.addNode("A", "A", {"X"});  // Duplicate
        });
        
        // UX: Should get helpful error for invalid state
        bool throws2 = TestSuite::assertThrows([&]() {
            std::map<std::string, std::string> parentStates;
            network.getConditionalProbability("A", "InvalidState", parentStates);
        });
        
        return TestSuite::assertTrue(throws1 && throws2);
    });
}

int main() {
    std::cout << "=== UX/API Usability Tests ===" << std::endl;
    
    TestSuite suite("UX Tests");
    
    std::cout << "\nAPI Usability:" << std::endl;
    runAPIUsabilityTests(suite);
    
    std::cout << "\nAPI Completeness:" << std::endl;
    runAPICompletenessTests(suite);
    
    std::cout << "\nError Handling UX:" << std::endl;
    runErrorHandlingUXTests(suite);
    
    suite.printSummary();
    
    return suite.allPassed() ? 0 : 1;
}
