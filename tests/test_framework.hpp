/*
 * test_framework.hpp - Simple test framework for Bayesian Network tests
 * Copyright (C) 2025, Shyamal Chandra
 *
 * Lightweight testing framework for unit, regression, A-B, UX, and blackbox tests
 */

#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <iomanip>
#include <chrono>

// Test result structure
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
    double executionTime;  // in milliseconds
};

// Test suite class
class TestSuite {
private:
    std::vector<TestResult> results;
    std::string suiteName;
    int totalTests;
    int passedTests;
    int failedTests;
    double totalTime;

public:
    TestSuite(const std::string& name) 
        : suiteName(name), totalTests(0), passedTests(0), failedTests(0), totalTime(0.0) {}

    // Run a test
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        totalTests++;
        auto start = std::chrono::high_resolution_clock::now();
        
        bool passed = false;
        std::string message;
        
        try {
            passed = testFunc();
            if (!passed) {
                message = "Test assertion failed";
            }
        } catch (const std::exception& e) {
            passed = false;
            message = std::string("Exception: ") + e.what();
        } catch (...) {
            passed = false;
            message = "Unknown exception";
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        double execTime = std::chrono::duration<double, std::milli>(end - start).count();
        totalTime += execTime;
        
        TestResult result;
        result.name = testName;
        result.passed = passed;
        result.message = message;
        result.executionTime = execTime;
        results.push_back(result);
        
        if (passed) {
            passedTests++;
            std::cout << "  ✓ " << testName << " (" << std::fixed << std::setprecision(2) 
                      << execTime << "ms)" << std::endl;
        } else {
            failedTests++;
            std::cout << "  ✗ " << testName << " - " << message << std::endl;
        }
    }

    // Assertion helpers
    static bool assertTrue(bool condition, const std::string& msg = "") {
        if (!condition && !msg.empty()) {
            std::cerr << "    Assertion failed: " << msg << std::endl;
        }
        return condition;
    }

    static bool assertFalse(bool condition, const std::string& msg = "") {
        return assertTrue(!condition, msg);
    }

    static bool assertEqual(double a, double b, double tolerance = 1e-6, const std::string& msg = "") {
        bool result = std::abs(a - b) < tolerance;
        if (!result) {
            std::cerr << "    Assertion failed: " << a << " != " << b 
                      << " (tolerance: " << tolerance << ")";
            if (!msg.empty()) std::cerr << " - " << msg;
            std::cerr << std::endl;
        }
        return result;
    }

    static bool assertEqual(const std::string& a, const std::string& b, const std::string& msg = "") {
        bool result = (a == b);
        if (!result) {
            std::cerr << "    Assertion failed: \"" << a << "\" != \"" << b << "\"";
            if (!msg.empty()) std::cerr << " - " << msg;
            std::cerr << std::endl;
        }
        return result;
    }

    static bool assertEqual(size_t a, size_t b, const std::string& msg = "") {
        bool result = (a == b);
        if (!result) {
            std::cerr << "    Assertion failed: " << a << " != " << b;
            if (!msg.empty()) std::cerr << " - " << msg;
            std::cerr << std::endl;
        }
        return result;
    }

    static bool assertEqual(int a, int b, const std::string& msg = "") {
        bool result = (a == b);
        if (!result) {
            std::cerr << "    Assertion failed: " << a << " != " << b;
            if (!msg.empty()) std::cerr << " - " << msg;
            std::cerr << std::endl;
        }
        return result;
    }

    static bool assertNotNull(void* ptr, const std::string& msg = "") {
        bool result = (ptr != nullptr);
        if (!result && !msg.empty()) {
            std::cerr << "    Assertion failed: " << msg << std::endl;
        }
        return result;
    }

    static bool assertThrows(std::function<void()> func, const std::string& msg = "") {
        try {
            func();
            if (!msg.empty()) {
                std::cerr << "    Assertion failed: Expected exception but none thrown - " << msg << std::endl;
            }
            return false;
        } catch (...) {
            return true;
        }
    }

    // Print summary
    void printSummary() {
        std::cout << "\n" << suiteName << " Summary:" << std::endl;
        std::cout << "  Total tests: " << totalTests << std::endl;
        std::cout << "  Passed: " << passedTests << std::endl;
        std::cout << "  Failed: " << failedTests << std::endl;
        std::cout << "  Total time: " << std::fixed << std::setprecision(2) << totalTime << "ms" << std::endl;
        
        if (failedTests > 0) {
            std::cout << "\nFailed tests:" << std::endl;
            for (const auto& result : results) {
                if (!result.passed) {
                    std::cout << "  - " << result.name << ": " << result.message << std::endl;
                }
            }
        }
    }

    // Get results
    bool allPassed() const { return failedTests == 0; }
    int getTotalTests() const { return totalTests; }
    int getPassedTests() const { return passedTests; }
    int getFailedTests() const { return failedTests; }
};

#endif // TEST_FRAMEWORK_HPP
