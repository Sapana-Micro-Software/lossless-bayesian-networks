/*
 * unit_tests.cpp - Unit tests for Bayesian Network components
 * Copyright (C) 2025, Shyamal Chandra
 *
 * Tests individual components: Node, CPT, and BayesianNetwork methods
 */

#include "test_framework.hpp"
#include "../node.hpp"
#include "../cpt.hpp"
#include "../bayesian_network.hpp"
#include <vector>
#include <string>
#include <map>

void runNodeTests(TestSuite& suite) {
    suite.runTest("Node construction", []() {
        Node node("TestNode", {"State1", "State2", "State3"});
        return TestSuite::assertEqual(node.name, std::string("TestNode")) &&
               TestSuite::assertEqual(node.getNumStates(), size_t(3));
    });

    suite.runTest("Node state index lookup", []() {
        Node node("TestNode", {"State1", "State2", "State3"});
        return TestSuite::assertEqual(node.getStateIndex("State1"), 0) &&
               TestSuite::assertEqual(node.getStateIndex("State2"), 1) &&
               TestSuite::assertEqual(node.getStateIndex("State3"), 2) &&
               TestSuite::assertEqual(node.getStateIndex("Invalid"), -1);
    });

    suite.runTest("Node state existence check", []() {
        Node node("TestNode", {"State1", "State2"});
        return TestSuite::assertTrue(node.hasState("State1")) &&
               TestSuite::assertTrue(node.hasState("State2")) &&
               TestSuite::assertFalse(node.hasState("State3"));
    });

    suite.runTest("Node parent management", []() {
        Node node("TestNode", {"State1"});
        node.addParent("Parent1");
        node.addParent("Parent2");
        
        return TestSuite::assertTrue(node.hasParent("Parent1")) &&
               TestSuite::assertTrue(node.hasParent("Parent2")) &&
               TestSuite::assertEqual(node.getNumParents(), size_t(2));
    });

    suite.runTest("Node parent removal", []() {
        Node node("TestNode", {"State1"});
        node.addParent("Parent1");
        node.addParent("Parent2");
        node.removeParent("Parent1");
        
        return TestSuite::assertFalse(node.hasParent("Parent1")) &&
               TestSuite::assertTrue(node.hasParent("Parent2")) &&
               TestSuite::assertEqual(node.getNumParents(), size_t(1));
    });
}

void runCPTTests(TestSuite& suite) {
    suite.runTest("CPT construction", []() {
        std::vector<size_t> dims = {2, 3};
        ConditionalProbabilityTable cpt(dims);
        return TestSuite::assertEqual(cpt.getTotalSize(), size_t(6));
    });

    suite.runTest("CPT set and get probability", []() {
        std::vector<size_t> dims = {2, 2};
        ConditionalProbabilityTable cpt(dims);
        cpt.setProbability({0}, 0, 0.7);
        cpt.setProbability({0}, 1, 0.3);
        cpt.setProbability({1}, 0, 0.4);
        cpt.setProbability({1}, 1, 0.6);
        
        return TestSuite::assertEqual(cpt.getProbability({0}, 0), 0.7) &&
               TestSuite::assertEqual(cpt.getProbability({0}, 1), 0.3) &&
               TestSuite::assertEqual(cpt.getProbability({1}, 0), 0.4) &&
               TestSuite::assertEqual(cpt.getProbability({1}, 1), 0.6);
    });

    suite.runTest("CPT normalization", []() {
        std::vector<size_t> dims = {2, 2};
        ConditionalProbabilityTable cpt(dims);
        cpt.setProbability({0}, 0, 0.5);
        cpt.setProbability({0}, 1, 0.5);
        cpt.setProbability({1}, 0, 0.2);
        cpt.setProbability({1}, 1, 0.8);
        cpt.normalize();
        
        return TestSuite::assertTrue(cpt.isValid());
    });

    suite.runTest("CPT validation", []() {
        std::vector<size_t> dims = {2, 2};
        ConditionalProbabilityTable cpt(dims);
        cpt.setProbability({0}, 0, 0.5);
        cpt.setProbability({0}, 1, 0.5);
        cpt.setProbability({1}, 0, 0.3);
        cpt.setProbability({1}, 1, 0.7);
        
        return TestSuite::assertTrue(cpt.isValid());
    });

    suite.runTest("CPT invalid probability range", []() {
        std::vector<size_t> dims = {1, 2};
        ConditionalProbabilityTable cpt(dims);
        
        bool throws1 = TestSuite::assertThrows([&]() {
            cpt.setProbability({}, 0, 1.5);  // > 1.0
        });
        
        bool throws2 = TestSuite::assertThrows([&]() {
            cpt.setProbability({}, 0, -0.1);  // < 0.0
        });
        
        return throws1 && throws2;
    });
}

void runBayesianNetworkTests(TestSuite& suite) {
    suite.runTest("Network node addition", []() {
        BayesianNetwork network;
        network.addNode("A", "NodeA", {"State1", "State2"});
        network.addNode("B", "NodeB", {"X", "Y"});
        
        auto nodeIds = network.getNodeIds();
        return TestSuite::assertEqual(nodeIds.size(), size_t(2));
    });

    suite.runTest("Network edge addition", []() {
        BayesianNetwork network;
        network.addNode("A", "NodeA", {"State1"});
        network.addNode("B", "NodeB", {"State1"});
        network.addEdge("A", "B");
        
        const Node& nodeB = network.getNode("B");
        return TestSuite::assertTrue(nodeB.hasParent("A"));
    });

    suite.runTest("Network cycle detection", []() {
        BayesianNetwork network;
        network.addNode("A", "NodeA", {"State1"});
        network.addNode("B", "NodeB", {"State1"});
        network.addNode("C", "NodeC", {"State1"});
        
        network.addEdge("A", "B");
        network.addEdge("B", "C");
        
        bool throws = TestSuite::assertThrows([&]() {
            network.addEdge("C", "A");  // Creates cycle
        });
        
        return throws;
    });

    suite.runTest("Network self-loop prevention", []() {
        BayesianNetwork network;
        network.addNode("A", "NodeA", {"State1"});
        
        bool throws = TestSuite::assertThrows([&]() {
            network.addEdge("A", "A");  // Self-loop
        });
        
        return throws;
    });

    suite.runTest("Network CPT setting", []() {
        BayesianNetwork network;
        network.addNode("A", "NodeA", {"State1", "State2"});
        
        std::vector<size_t> dims = {2};
        ConditionalProbabilityTable cpt(dims);
        cpt.setProbability({}, 0, 0.6);
        cpt.setProbability({}, 1, 0.4);
        network.setCPT("A", cpt);
        
        std::map<std::string, std::string> parentStates;
        double prob = network.getConditionalProbability("A", "State1", parentStates);
        
        return TestSuite::assertEqual(prob, 0.6);
    });

    suite.runTest("Network joint probability computation", []() {
        BayesianNetwork network;
        network.addNode("A", "NodeA", {"True", "False"});
        network.addNode("B", "NodeB", {"True", "False"});
        network.addEdge("A", "B");
        
        // Prior for A
        std::vector<size_t> aDims = {2};
        ConditionalProbabilityTable aCPT(aDims);
        aCPT.setProbability({}, 0, 0.7);  // P(A=True) = 0.7
        aCPT.setProbability({}, 1, 0.3);   // P(A=False) = 0.3
        network.setCPT("A", aCPT);
        
        // CPT for B given A
        std::vector<size_t> bDims = {2, 2};
        ConditionalProbabilityTable bCPT(bDims);
        bCPT.setProbability({0}, 0, 0.8);  // P(B=True | A=True) = 0.8
        bCPT.setProbability({0}, 1, 0.2);  // P(B=False | A=True) = 0.2
        bCPT.setProbability({1}, 0, 0.1);  // P(B=True | A=False) = 0.1
        bCPT.setProbability({1}, 1, 0.9);  // P(B=False | A=False) = 0.9
        network.setCPT("B", bCPT);
        
        std::map<std::string, std::string> assignment;
        assignment["A"] = "True";
        assignment["B"] = "True";
        
        double jointProb = network.computeJointProbability(assignment);
        double expected = 0.7 * 0.8;  // P(A=True) * P(B=True | A=True)
        
        return TestSuite::assertEqual(jointProb, expected, 1e-6);
    });
}

int main() {
    std::cout << "=== Unit Tests ===" << std::endl;
    
    TestSuite suite("Unit Tests");
    
    std::cout << "\nNode Tests:" << std::endl;
    runNodeTests(suite);
    
    std::cout << "\nCPT Tests:" << std::endl;
    runCPTTests(suite);
    
    std::cout << "\nBayesianNetwork Tests:" << std::endl;
    runBayesianNetworkTests(suite);
    
    suite.printSummary();
    
    return suite.allPassed() ? 0 : 1;
}
