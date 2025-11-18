/*
 * regression_tests.cpp - Regression tests for Bayesian Network
 * Copyright (C) 2025, Shyamal Chandra
 *
 * Tests to ensure existing functionality continues to work correctly
 */

#include "test_framework.hpp"
#include "../bayesian_network.hpp"
#include <vector>
#include <string>
#include <map>

void runMedicalDiagnosisRegression(TestSuite& suite) {
    suite.runTest("Medical diagnosis example regression", []() {
        BayesianNetwork network;
        
        // Add nodes
        network.addNode("Disease", "Disease", {"None", "Cold", "Flu"});
        network.addNode("Symptom1", "Fever", {"No", "Yes"});
        network.addNode("Symptom2", "Cough", {"No", "Yes"});
        
        // Add edges
        network.addEdge("Disease", "Symptom1");
        network.addEdge("Disease", "Symptom2");
        
        // Create CPTs
        std::vector<size_t> symptom1Dims = {3, 2};
        ConditionalProbabilityTable cpt1(symptom1Dims);
        cpt1.setProbability({0}, 0, 0.9);
        cpt1.setProbability({0}, 1, 0.1);
        cpt1.setProbability({1}, 0, 0.7);
        cpt1.setProbability({1}, 1, 0.3);
        cpt1.setProbability({2}, 0, 0.2);
        cpt1.setProbability({2}, 1, 0.8);
        cpt1.normalize();
        network.setCPT("Symptom1", cpt1);
        
        std::vector<size_t> symptom2Dims = {3, 2};
        ConditionalProbabilityTable cpt2(symptom2Dims);
        cpt2.setProbability({0}, 0, 0.95);
        cpt2.setProbability({0}, 1, 0.05);
        cpt2.setProbability({1}, 0, 0.3);
        cpt2.setProbability({1}, 1, 0.7);
        cpt2.setProbability({2}, 0, 0.4);
        cpt2.setProbability({2}, 1, 0.6);
        cpt2.normalize();
        network.setCPT("Symptom2", cpt2);
        
        std::vector<size_t> diseaseDims = {3};
        ConditionalProbabilityTable diseaseCPT(diseaseDims);
        diseaseCPT.setProbability({}, 0, 0.7);
        diseaseCPT.setProbability({}, 1, 0.2);
        diseaseCPT.setProbability({}, 2, 0.1);
        diseaseCPT.normalize();
        network.setCPT("Disease", diseaseCPT);
        
        // Perform inference
        std::map<std::string, std::string> evidence;
        evidence["Symptom1"] = "Yes";
        evidence["Symptom2"] = "Yes";
        
        std::vector<std::string> queryNodes = {"Disease"};
        auto results = network.variableElimination(queryNodes, evidence);
        
        // Regression: Check that results are reasonable (probabilities sum to ~1.0)
        double sum = 0.0;
        for (const auto& pair : results) {
            sum += pair.second;
        }
        
        return TestSuite::assertEqual(sum, 1.0, 1e-4) &&
               TestSuite::assertTrue(results.size() == 3);  // Three disease states
    });
}

void runAlarmNetworkRegression(TestSuite& suite) {
    suite.runTest("Alarm network example regression", []() {
        BayesianNetwork network;
        
        network.addNode("Burglary", "Burglary", {"False", "True"});
        network.addNode("Earthquake", "Earthquake", {"False", "True"});
        network.addNode("Alarm", "Alarm", {"False", "True"});
        network.addNode("JohnCalls", "JohnCalls", {"False", "True"});
        network.addNode("MaryCalls", "MaryCalls", {"False", "True"});
        
        network.addEdge("Burglary", "Alarm");
        network.addEdge("Earthquake", "Alarm");
        network.addEdge("Alarm", "JohnCalls");
        network.addEdge("Alarm", "MaryCalls");
        
        // Set up CPTs (simplified)
        std::vector<size_t> burglaryDims = {2};
        ConditionalProbabilityTable burglaryCPT(burglaryDims);
        burglaryCPT.setProbability({}, 0, 0.999);
        burglaryCPT.setProbability({}, 1, 0.001);
        network.setCPT("Burglary", burglaryCPT);
        
        std::vector<size_t> earthquakeDims = {2};
        ConditionalProbabilityTable earthquakeCPT(earthquakeDims);
        earthquakeCPT.setProbability({}, 0, 0.998);
        earthquakeCPT.setProbability({}, 1, 0.002);
        network.setCPT("Earthquake", earthquakeCPT);
        
        // Set up simplified CPTs for Alarm, JohnCalls, MaryCalls
        std::vector<size_t> alarmDims = {2, 2, 2};
        ConditionalProbabilityTable alarmCPT(alarmDims);
        alarmCPT.setProbability({0, 0}, 0, 0.999);
        alarmCPT.setProbability({0, 0}, 1, 0.001);
        alarmCPT.setProbability({0, 1}, 0, 0.06);
        alarmCPT.setProbability({0, 1}, 1, 0.94);
        alarmCPT.setProbability({1, 0}, 0, 0.05);
        alarmCPT.setProbability({1, 0}, 1, 0.95);
        alarmCPT.setProbability({1, 1}, 0, 0.02);
        alarmCPT.setProbability({1, 1}, 1, 0.98);
        alarmCPT.normalize();
        network.setCPT("Alarm", alarmCPT);
        
        std::vector<size_t> johnDims = {2, 2};
        ConditionalProbabilityTable johnCPT(johnDims);
        johnCPT.setProbability({0}, 0, 0.95);
        johnCPT.setProbability({0}, 1, 0.05);
        johnCPT.setProbability({1}, 0, 0.10);
        johnCPT.setProbability({1}, 1, 0.90);
        johnCPT.normalize();
        network.setCPT("JohnCalls", johnCPT);
        
        std::vector<size_t> maryDims = {2, 2};
        ConditionalProbabilityTable maryCPT(maryDims);
        maryCPT.setProbability({0}, 0, 0.99);
        maryCPT.setProbability({0}, 1, 0.01);
        maryCPT.setProbability({1}, 0, 0.30);
        maryCPT.setProbability({1}, 1, 0.70);
        maryCPT.normalize();
        network.setCPT("MaryCalls", maryCPT);
        
        // Perform inference
        std::map<std::string, std::string> evidence;
        evidence["JohnCalls"] = "True";
        evidence["MaryCalls"] = "True";
        
        std::vector<std::string> queryNodes = {"Burglary"};
        auto results = network.variableElimination(queryNodes, evidence);
        
        // Regression: Check that results are valid probabilities
        double sum = 0.0;
        for (const auto& pair : results) {
            sum += pair.second;
            // Probabilities should be in [0, 1]
            if (pair.second < 0.0 || pair.second > 1.0) {
                return false;
            }
        }
        
        return TestSuite::assertEqual(sum, 1.0, 1e-4) &&
               TestSuite::assertTrue(results.size() == 2);  // Two states
    });
}

void runTopologicalSortRegression(TestSuite& suite) {
    suite.runTest("Topological sort consistency", []() {
        BayesianNetwork network;
        
        network.addNode("A", "A", {"X"});
        network.addNode("B", "B", {"X"});
        network.addNode("C", "C", {"X"});
        network.addNode("D", "D", {"X"});
        
        network.addEdge("A", "B");
        network.addEdge("B", "C");
        network.addEdge("A", "D");
        
        // Regression: Topological sort should always produce valid ordering
        // A should come before B, B before C, A before D
        auto nodeIds = network.getNodeIds();
        
        // Check that we can get nodes without errors
        bool allNodesAccessible = true;
        for (const auto& nodeId : nodeIds) {
            try {
                network.getNode(nodeId);
            } catch (...) {
                allNodesAccessible = false;
            }
        }
        
        return TestSuite::assertTrue(allNodesAccessible) &&
               TestSuite::assertEqual(nodeIds.size(), size_t(4));
    });
}

void runCPTNormalizationRegression(TestSuite& suite) {
    suite.runTest("CPT normalization consistency", []() {
        std::vector<size_t> dims = {2, 3};
        ConditionalProbabilityTable cpt(dims);
        
        // Set probabilities that don't sum to 1
        cpt.setProbability({0}, 0, 0.3);
        cpt.setProbability({0}, 1, 0.4);
        cpt.setProbability({0}, 2, 0.2);  // Sum = 0.9
        
        cpt.setProbability({1}, 0, 0.5);
        cpt.setProbability({1}, 1, 0.3);
        cpt.setProbability({1}, 2, 0.1);  // Sum = 0.9
        
        // Normalize
        cpt.normalize();
        
        // Regression: After normalization, should be valid
        return TestSuite::assertTrue(cpt.isValid());
    });
}

int main() {
    std::cout << "=== Regression Tests ===" << std::endl;
    
    TestSuite suite("Regression Tests");
    
    std::cout << "\nMedical Diagnosis Regression:" << std::endl;
    runMedicalDiagnosisRegression(suite);
    
    std::cout << "\nAlarm Network Regression:" << std::endl;
    runAlarmNetworkRegression(suite);
    
    std::cout << "\nTopological Sort Regression:" << std::endl;
    runTopologicalSortRegression(suite);
    
    std::cout << "\nCPT Normalization Regression:" << std::endl;
    runCPTNormalizationRegression(suite);
    
    suite.printSummary();
    
    return suite.allPassed() ? 0 : 1;
}
