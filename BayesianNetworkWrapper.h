/*
 * BayesianNetworkWrapper.h - Objective-C wrapper for Lossless Bayesian Network
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This file provides an Objective-C interface to the C++ Bayesian network implementation.
 */

#import <Foundation/Foundation.h>

// Forward declarations
@class BNNode;
@class BNEdge;
@class BNInferenceResult;

/**
 * BNNode represents a node in the Bayesian network
 */
@interface BNNode : NSObject

// Node identifier
@property (nonatomic, strong) NSString *nodeId;
// Node display name
@property (nonatomic, strong) NSString *name;
// Array of possible state names
@property (nonatomic, strong) NSArray<NSString *> *states;
// Array of parent node IDs
@property (nonatomic, strong) NSArray<NSString *> *parentIds;
// X coordinate for visualization
@property (nonatomic, assign) CGFloat x;
// Y coordinate for visualization
@property (nonatomic, assign) CGFloat y;

- (instancetype)initWithId:(NSString *)nodeId 
                       name:(NSString *)name 
                     states:(NSArray<NSString *> *)states;

@end

/**
 * BNEdge represents an edge in the Bayesian network
 */
@interface BNEdge : NSObject

// Source node ID
@property (nonatomic, strong) NSString *fromNodeId;
// Destination node ID
@property (nonatomic, strong) NSString *toNodeId;

- (instancetype)initWithFrom:(NSString *)fromNodeId to:(NSString *)toNodeId;

@end

/**
 * BNInferenceResult represents the result of an inference query
 */
@interface BNInferenceResult : NSObject

// Map of assignments to probabilities
@property (nonatomic, strong) NSDictionary<NSDictionary<NSString *, NSString *> *, NSNumber *> *probabilities;

@end

/**
 * BayesianNetworkWrapper provides Objective-C interface to C++ Bayesian network
 */
@interface BayesianNetworkWrapper : NSObject

// Array of all nodes in the network
@property (nonatomic, strong, readonly) NSArray<BNNode *> *nodes;
// Array of all edges in the network
@property (nonatomic, strong, readonly) NSArray<BNEdge *> *edges;

/**
 * Initialize with an empty network
 */
- (instancetype)init;

/**
 * Add a node to the network
 * @param nodeId Unique identifier for the node
 * @param name Display name of the node
 * @param states Array of possible state names
 * @return YES if successful, NO otherwise
 */
- (BOOL)addNodeWithId:(NSString *)nodeId 
                 name:(NSString *)name 
               states:(NSArray<NSString *> *)states;

/**
 * Add an edge from parent to child
 * @param parentId ID of parent node
 * @param childId ID of child node
 * @return YES if successful, NO otherwise
 */
- (BOOL)addEdgeFromParent:(NSString *)parentId toChild:(NSString *)childId;

/**
 * Set conditional probability for a node
 * @param nodeId ID of the node
 * @param parentStates Dictionary mapping parent IDs to their states
 * @param nodeState State of the node
 * @param probability Probability value (0.0 to 1.0)
 * @return YES if successful, NO otherwise
 */
- (BOOL)setProbabilityForNode:(NSString *)nodeId 
                 parentStates:(NSDictionary<NSString *, NSString *> *)parentStates
                    nodeState:(NSString *)nodeState
                  probability:(double)probability;

/**
 * Normalize probabilities for a node
 * @param nodeId ID of the node
 * @return YES if successful, NO otherwise
 */
- (BOOL)normalizeProbabilitiesForNode:(NSString *)nodeId;

/**
 * Perform inference query
 * @param queryNodeIds Array of node IDs to query
 * @param evidence Dictionary mapping observed node IDs to their states
 * @return BNInferenceResult with probabilities, or nil on error
 */
- (BNInferenceResult *)performInferenceWithQueryNodes:(NSArray<NSString *> *)queryNodeIds 
                                               evidence:(NSDictionary<NSString *, NSString *> *)evidence;

/**
 * Compute joint probability for a full assignment
 * @param assignment Dictionary mapping node IDs to their states
 * @return Joint probability value, or -1.0 on error
 */
- (double)computeJointProbability:(NSDictionary<NSString *, NSString *> *)assignment;

/**
 * Get conditional probability
 * @param nodeId ID of the node
 * @param nodeState State of the node
 * @param parentStates Dictionary mapping parent IDs to their states
 * @return Conditional probability value, or -1.0 on error
 */
- (double)getConditionalProbabilityForNode:(NSString *)nodeId 
                                  nodeState:(NSString *)nodeState
                              parentStates:(NSDictionary<NSString *, NSString *> *)parentStates;

/**
 * Load network from file
 * @param filePath Path to the network file
 * @return YES if successful, NO otherwise
 */
- (BOOL)loadFromFile:(NSString *)filePath;

/**
 * Save network to file
 * @param filePath Path to save the network file
 * @return YES if successful, NO otherwise
 */
- (BOOL)saveToFile:(NSString *)filePath;

/**
 * Get error message from last operation
 * @return Error message string, or nil if no error
 */
- (NSString *)lastErrorMessage;

@end
