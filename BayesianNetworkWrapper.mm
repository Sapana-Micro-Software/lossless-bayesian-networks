/*
 * BayesianNetworkWrapper.mm - Objective-C++ implementation
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This file implements the Objective-C wrapper for the C++ Bayesian network.
 */

#import "BayesianNetworkWrapper.h"
#import "bayesian_network.hpp"
#import <vector>
#import <string>
#import <map>

// Convert NSString to std::string
static std::string NSStringToStdString(NSString *nsString) {
    if (!nsString) return "";
    return [nsString UTF8String];
}

// Convert std::string to NSString
static NSString *StdStringToNSString(const std::string &stdString) {
    return [NSString stringWithUTF8String:stdString.c_str()];
}

// Convert NSArray<NSString *> to std::vector<std::string>
static std::vector<std::string> NSArrayToVector(NSArray<NSString *> *array) {
    std::vector<std::string> result;
    for (NSString *str in array) {
        result.push_back(NSStringToStdString(str));
    }
    return result;
}

// Convert std::vector<std::string> to NSArray<NSString *>
static NSArray<NSString *> *VectorToNSArray(const std::vector<std::string> &vec) {
    NSMutableArray<NSString *> *result = [NSMutableArray array];
    for (const auto &str : vec) {
        [result addObject:StdStringToNSString(str)];
    }
    return result;
}

// Convert NSDictionary to std::map
static std::map<std::string, std::string> NSDictionaryToMap(NSDictionary<NSString *, NSString *> *dict) {
    std::map<std::string, std::string> result;
    for (NSString *key in dict) {
        result[NSStringToStdString(key)] = NSStringToStdString(dict[key]);
    }
    return result;
}

// Convert std::map to NSDictionary
static NSDictionary<NSString *, NSString *> *MapToNSDictionary(const std::map<std::string, std::string> &map) {
    NSMutableDictionary<NSString *, NSString *> *result = [NSMutableDictionary dictionary];
    for (const auto &pair : map) {
        result[StdStringToNSString(pair.first)] = StdStringToNSString(pair.second);
    }
    return result;
}

@implementation BNNode

- (instancetype)initWithId:(NSString *)nodeId 
                       name:(NSString *)name 
                     states:(NSArray<NSString *> *)states {
    self = [super init];
    if (self) {
        _nodeId = nodeId;
        _name = name;
        _states = states;
        _parentIds = @[];
        _x = 0.0;
        _y = 0.0;
    }
    return self;
}

@end

@implementation BNEdge

- (instancetype)initWithFrom:(NSString *)fromNodeId to:(NSString *)toNodeId {
    self = [super init];
    if (self) {
        _fromNodeId = fromNodeId;
        _toNodeId = toNodeId;
    }
    return self;
}

@end

@implementation BNInferenceResult

@end

@interface BayesianNetworkWrapper ()

// Internal C++ network instance
@property (nonatomic, assign) void *cppNetwork;
// Last error message
@property (nonatomic, strong) NSString *errorMessage;

@end

@implementation BayesianNetworkWrapper

- (instancetype)init {
    self = [super init];
    if (self) {
        // Create C++ BayesianNetwork instance
        BayesianNetwork *network = new BayesianNetwork();
        _cppNetwork = (__bridge_retained void *)network;
        _nodes = @[];
        _edges = @[];
        _errorMessage = nil;
    }
    return self;
}

- (void)dealloc {
    // Clean up C++ instance
    if (_cppNetwork) {
        BayesianNetwork *network = (__bridge_transfer BayesianNetwork *)_cppNetwork;
        delete network;
        _cppNetwork = nil;
    }
}

- (BayesianNetwork *)getCppNetwork {
    return (__bridge BayesianNetwork *)_cppNetwork;
}

- (BOOL)addNodeWithId:(NSString *)nodeId 
                 name:(NSString *)name 
               states:(NSArray<NSString *> *)states {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        std::string nodeIdStr = NSStringToStdString(nodeId);
        std::string nameStr = NSStringToStdString(name);
        std::vector<std::string> statesVec = NSArrayToVector(states);
        
        network->addNode(nodeIdStr, nameStr, statesVec);
        
        // Update nodes array
        [self updateNodesAndEdges];
        
        self.errorMessage = nil;
        return YES;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return NO;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return NO;
    }
}

- (BOOL)addEdgeFromParent:(NSString *)parentId toChild:(NSString *)childId {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        std::string parentIdStr = NSStringToStdString(parentId);
        std::string childIdStr = NSStringToStdString(childId);
        
        network->addEdge(parentIdStr, childIdStr);
        
        // Update edges array
        [self updateNodesAndEdges];
        
        self.errorMessage = nil;
        return YES;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return NO;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return NO;
    }
}

- (BOOL)setProbabilityForNode:(NSString *)nodeId 
                 parentStates:(NSDictionary<NSString *, NSString *> *)parentStates
                    nodeState:(NSString *)nodeState
                  probability:(double)probability {
    // This is a simplified interface - full implementation would need
    // to work with CPT dimensions properly
    // For now, we'll need to set up the CPT structure first
    self.errorMessage = @"CPT setting requires proper dimension setup - use lower-level API";
    return NO;
}

- (BOOL)normalizeProbabilitiesForNode:(NSString *)nodeId {
    // Implementation would normalize the CPT for the given node
    self.errorMessage = @"Normalization requires CPT access - use lower-level API";
    return NO;
}

- (BNInferenceResult *)performInferenceWithQueryNodes:(NSArray<NSString *> *)queryNodeIds 
                                               evidence:(NSDictionary<NSString *, NSString *> *)evidence {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        
        std::vector<std::string> queryVec = NSArrayToVector(queryNodeIds);
        std::map<std::string, std::string> evidenceMap = NSDictionaryToMap(evidence);
        
        auto cppResults = network->variableElimination(queryVec, evidenceMap);
        
        BNInferenceResult *result = [[BNInferenceResult alloc] init];
        NSMutableDictionary<NSDictionary<NSString *, NSString *> *, NSNumber *> *probDict = [NSMutableDictionary dictionary];
        
        for (const auto &pair : cppResults) {
            NSDictionary<NSString *, NSString *> *assignment = MapToNSDictionary(pair.first);
            NSNumber *prob = [NSNumber numberWithDouble:pair.second];
            probDict[assignment] = prob;
        }
        
        result.probabilities = probDict;
        self.errorMessage = nil;
        return result;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return nil;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return nil;
    }
}

- (double)computeJointProbability:(NSDictionary<NSString *, NSString *> *)assignment {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        std::map<std::string, std::string> assignmentMap = NSDictionaryToMap(assignment);
        double result = network->computeJointProbability(assignmentMap);
        self.errorMessage = nil;
        return result;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return -1.0;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return -1.0;
    }
}

- (double)getConditionalProbabilityForNode:(NSString *)nodeId 
                                  nodeState:(NSString *)nodeState
                              parentStates:(NSDictionary<NSString *, NSString *> *)parentStates {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        std::string nodeIdStr = NSStringToStdString(nodeId);
        std::string nodeStateStr = NSStringToStdString(nodeState);
        std::map<std::string, std::string> parentStatesMap = NSDictionaryToMap(parentStates);
        
        double result = network->getConditionalProbability(nodeIdStr, nodeStateStr, parentStatesMap);
        self.errorMessage = nil;
        return result;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return -1.0;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return -1.0;
    }
}

- (BOOL)loadFromFile:(NSString *)filePath {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        std::string filePathStr = NSStringToStdString(filePath);
        network->loadFromFile(filePathStr);
        [self updateNodesAndEdges];
        self.errorMessage = nil;
        return YES;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return NO;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return NO;
    }
}

- (BOOL)saveToFile:(NSString *)filePath {
    try {
        BayesianNetwork *network = [self getCppNetwork];
        std::string filePathStr = NSStringToStdString(filePath);
        network->saveToFile(filePathStr);
        self.errorMessage = nil;
        return YES;
    } catch (const std::exception& e) {
        self.errorMessage = [NSString stringWithUTF8String:e.what()];
        return NO;
    } catch (...) {
        self.errorMessage = @"Unknown C++ exception";
        return NO;
    }
}

- (NSString *)lastErrorMessage {
    return self.errorMessage;
}

- (void)updateNodesAndEdges {
    BayesianNetwork *network = [self getCppNetwork];
    NSMutableArray<BNNode *> *nodesArray = [NSMutableArray array];
    NSMutableArray<BNEdge *> *edgesArray = [NSMutableArray array];
    
    std::vector<std::string> nodeIds = network->getNodeIds();
    
    for (const std::string &nodeIdStr : nodeIds) {
        const Node &cppNode = network->getNode(nodeIdStr);
        
        BNNode *node = [[BNNode alloc] init];
        node.nodeId = StdStringToNSString(nodeIdStr);
        node.name = StdStringToNSString(cppNode.name);
        node.states = VectorToNSArray(cppNode.states);
        
        std::vector<std::string> parentIds;
        for (const std::string &parentId : cppNode.parentIds) {
            parentIds.push_back(parentId);
        }
        node.parentIds = VectorToNSArray(parentIds);
        
        [nodesArray addObject:node];
        
        // Add edges
        for (const std::string &parentId : cppNode.parentIds) {
            BNEdge *edge = [[BNEdge alloc] initWithFrom:StdStringToNSString(parentId)
                                                      to:StdStringToNSString(nodeIdStr)];
            [edgesArray addObject:edge];
        }
    }
    
    _nodes = nodesArray;
    _edges = edgesArray;
}

@end
