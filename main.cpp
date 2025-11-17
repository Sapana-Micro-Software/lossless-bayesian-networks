/*
 * main.cpp - Example usage of Lossless Bayesian Network
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This file demonstrates how to use the Bayesian network implementation.
 */

// Main Bayesian network class
#include "bayesian_network.hpp"
// Input/output stream
#include <iostream>
// Input/output manipulators
#include <iomanip>

/**
 * Example: Simple medical diagnosis network
 * Nodes: Disease, Symptom1, Symptom2
 */
void exampleMedicalDiagnosis() {
    std::cout << "=== Medical Diagnosis Example ===\n\n";

    // Create network instance
    BayesianNetwork network;

    // Add nodes
    // Disease node with 3 states
    network.addNode("Disease", "Disease", {"None", "Cold", "Flu"});
    // Symptom 1: Fever
    network.addNode("Symptom1", "Fever", {"No", "Yes"});
    // Symptom 2: Cough
    network.addNode("Symptom2", "Cough", {"No", "Yes"});

    // Add edges (disease causes symptoms)
    // Disease -> Fever
    network.addEdge("Disease", "Symptom1");
    // Disease -> Cough
    network.addEdge("Disease", "Symptom2");

    // Create CPT for Symptom1 (Fever)
    // 3 disease states, 2 symptom states
    std::vector<size_t> symptom1Dims = {3, 2};
    ConditionalProbabilityTable cpt1(symptom1Dims);
    // P(Fever=No | Disease=None) = 0.9, P(Fever=Yes | Disease=None) = 0.1
    // Disease=None -> Fever=No
    cpt1.setProbability({0}, 0, 0.9);
    // Disease=None -> Fever=Yes
    cpt1.setProbability({0}, 1, 0.1);
    // P(Fever=No | Disease=Cold) = 0.7, P(Fever=Yes | Disease=Cold) = 0.3
    // Disease=Cold -> Fever=No
    cpt1.setProbability({1}, 0, 0.7);
    // Disease=Cold -> Fever=Yes
    cpt1.setProbability({1}, 1, 0.3);
    // P(Fever=No | Disease=Flu) = 0.2, P(Fever=Yes | Disease=Flu) = 0.8
    // Disease=Flu -> Fever=No
    cpt1.setProbability({2}, 0, 0.2);
    // Disease=Flu -> Fever=Yes
    cpt1.setProbability({2}, 1, 0.8);
    // Ensure normalization
    cpt1.normalize();
    // Set CPT for Symptom1
    network.setCPT("Symptom1", cpt1);

    // Create CPT for Symptom2 (Cough)
    // 3 disease states, 2 symptom states
    std::vector<size_t> symptom2Dims = {3, 2};
    ConditionalProbabilityTable cpt2(symptom2Dims);
    // P(Cough=No | Disease=None) = 0.95, P(Cough=Yes | Disease=None) = 0.05
    // Disease=None -> Cough=No
    cpt2.setProbability({0}, 0, 0.95);
    // Disease=None -> Cough=Yes
    cpt2.setProbability({0}, 1, 0.05);
    // P(Cough=No | Disease=Cold) = 0.3, P(Cough=Yes | Disease=Cold) = 0.7
    // Disease=Cold -> Cough=No
    cpt2.setProbability({1}, 0, 0.3);
    // Disease=Cold -> Cough=Yes
    cpt2.setProbability({1}, 1, 0.7);
    // P(Cough=No | Disease=Flu) = 0.4, P(Cough=Yes | Disease=Flu) = 0.6
    // Disease=Flu -> Cough=No
    cpt2.setProbability({2}, 0, 0.4);
    // Disease=Flu -> Cough=Yes
    cpt2.setProbability({2}, 1, 0.6);
    // Ensure normalization
    cpt2.normalize();
    // Set CPT for Symptom2
    network.setCPT("Symptom2", cpt2);

    // Create CPT for Disease (prior probabilities)
    // 3 disease states, no parents
    std::vector<size_t> diseaseDims = {3};
    ConditionalProbabilityTable diseaseCPT(diseaseDims);
    // P(Disease=None) = 0.7
    diseaseCPT.setProbability({}, 0, 0.7);
    // P(Disease=Cold) = 0.2
    diseaseCPT.setProbability({}, 1, 0.2);
    // P(Disease=Flu) = 0.1
    diseaseCPT.setProbability({}, 2, 0.1);
    // Ensure normalization
    diseaseCPT.normalize();
    // Set CPT for Disease
    network.setCPT("Disease", diseaseCPT);

    // Perform inference: Given Fever=Yes and Cough=Yes, what's P(Disease)?
    std::cout << "Inference: Given Fever=Yes, Cough=Yes\n";
    std::cout << "What is P(Disease)?\n\n";

    // Observed evidence
    std::map<std::string, std::string> evidence;
    // Fever = Yes
    evidence["Symptom1"] = "Yes";
    // Cough = Yes
    evidence["Symptom2"] = "Yes";

    // Query variable
    std::vector<std::string> queryNodes = {"Disease"};
    // Perform inference
    auto results = network.variableElimination(queryNodes, evidence);

    // Display results
    std::cout << "Results:\n";
    // Set precision
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& pair : results) {
        const auto& assignment = pair.first;
        double prob = pair.second;
        std::cout << "  P(Disease=" << assignment.at("Disease") << ") = " << prob << "\n";
    }
    std::cout << "\n";
}

/**
 * Example: Simple alarm network
 */
void exampleAlarmNetwork() {
    std::cout << "=== Alarm Network Example ===\n\n";

    // Create network instance
    BayesianNetwork network;

    // Add nodes
    // Burglary node
    network.addNode("Burglary", "Burglary", {"False", "True"});
    // Earthquake node
    network.addNode("Earthquake", "Earthquake", {"False", "True"});
    // Alarm node
    network.addNode("Alarm", "Alarm", {"False", "True"});
    // John calls node
    network.addNode("JohnCalls", "JohnCalls", {"False", "True"});
    // Mary calls node
    network.addNode("MaryCalls", "MaryCalls", {"False", "True"});

    // Add edges
    // Burglary -> Alarm
    network.addEdge("Burglary", "Alarm");
    // Earthquake -> Alarm
    network.addEdge("Earthquake", "Alarm");
    // Alarm -> JohnCalls
    network.addEdge("Alarm", "JohnCalls");
    // Alarm -> MaryCalls
    network.addEdge("Alarm", "MaryCalls");

    // Prior for Burglary
    std::vector<size_t> burglaryDims = {2};
    ConditionalProbabilityTable burglaryCPT(burglaryDims);
    // P(Burglary=False) = 0.999
    burglaryCPT.setProbability({}, 0, 0.999);
    // P(Burglary=True) = 0.001
    burglaryCPT.setProbability({}, 1, 0.001);
    network.setCPT("Burglary", burglaryCPT);

    // Prior for Earthquake
    std::vector<size_t> earthquakeDims = {2};
    ConditionalProbabilityTable earthquakeCPT(earthquakeDims);
    // P(Earthquake=False) = 0.998
    earthquakeCPT.setProbability({}, 0, 0.998);
    // P(Earthquake=True) = 0.002
    earthquakeCPT.setProbability({}, 1, 0.002);
    network.setCPT("Earthquake", earthquakeCPT);

    // CPT for Alarm (depends on Burglary and Earthquake)
    // Burglary, Earthquake, Alarm
    std::vector<size_t> alarmDims = {2, 2, 2};
    ConditionalProbabilityTable alarmCPT(alarmDims);
    // P(Alarm=True | Burglary=False, Earthquake=False) = 0.001
    alarmCPT.setProbability({0, 0}, 1, 0.001);
    // P(Alarm=True | Burglary=False, Earthquake=True) = 0.94
    alarmCPT.setProbability({0, 1}, 1, 0.94);
    // P(Alarm=True | Burglary=True, Earthquake=False) = 0.95
    alarmCPT.setProbability({1, 0}, 1, 0.95);
    // P(Alarm=True | Burglary=True, Earthquake=True) = 0.98
    alarmCPT.setProbability({1, 1}, 1, 0.98);
    // Set False probabilities (will normalize)
    alarmCPT.setProbability({0, 0}, 0, 0.999);
    alarmCPT.setProbability({0, 1}, 0, 0.06);
    alarmCPT.setProbability({1, 0}, 0, 0.05);
    alarmCPT.setProbability({1, 1}, 0, 0.02);
    alarmCPT.normalize();
    network.setCPT("Alarm", alarmCPT);

    // CPT for JohnCalls (depends on Alarm)
    // Alarm, JohnCalls
    std::vector<size_t> johnDims = {2, 2};
    ConditionalProbabilityTable johnCPT(johnDims);
    // P(JohnCalls=True | Alarm=False) = 0.05
    johnCPT.setProbability({0}, 1, 0.05);
    // P(JohnCalls=True | Alarm=True) = 0.90
    johnCPT.setProbability({1}, 1, 0.90);
    // P(JohnCalls=False | Alarm=False) = 0.95
    johnCPT.setProbability({0}, 0, 0.95);
    // P(JohnCalls=False | Alarm=True) = 0.10
    johnCPT.setProbability({1}, 0, 0.10);
    johnCPT.normalize();
    network.setCPT("JohnCalls", johnCPT);

    // CPT for MaryCalls (depends on Alarm)
    // Alarm, MaryCalls
    std::vector<size_t> maryDims = {2, 2};
    ConditionalProbabilityTable maryCPT(maryDims);
    // P(MaryCalls=True | Alarm=False) = 0.01
    maryCPT.setProbability({0}, 1, 0.01);
    // P(MaryCalls=True | Alarm=True) = 0.70
    maryCPT.setProbability({1}, 1, 0.70);
    // P(MaryCalls=False | Alarm=False) = 0.99
    maryCPT.setProbability({0}, 0, 0.99);
    // P(MaryCalls=False | Alarm=True) = 0.30
    maryCPT.setProbability({1}, 0, 0.30);
    maryCPT.normalize();
    network.setCPT("MaryCalls", maryCPT);

    // Inference: Given JohnCalls=True and MaryCalls=True, what's P(Burglary)?
    std::cout << "Inference: Given JohnCalls=True, MaryCalls=True\n";
    std::cout << "What is P(Burglary)?\n\n";

    std::map<std::string, std::string> evidence;
    evidence["JohnCalls"] = "True";
    evidence["MaryCalls"] = "True";

    std::vector<std::string> queryNodes = {"Burglary"};
    auto results = network.variableElimination(queryNodes, evidence);

    std::cout << "Results:\n";
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& pair : results) {
        const auto& assignment = pair.first;
        double prob = pair.second;
        std::cout << "  P(Burglary=" << assignment.at("Burglary") << ") = " << prob << "\n";
    }
    std::cout << "\n";
}

/**
 * Main function
 */
int main() {
    std::cout << "Lossless Bayesian Network Implementation\n";
    std::cout << "Copyright (C) 2025, Shyamal Chandra\n\n";

    try {
        // Run medical diagnosis example
        exampleMedicalDiagnosis();
        // Run alarm network example
        exampleAlarmNetwork();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
