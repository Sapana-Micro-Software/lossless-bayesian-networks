/*
 * NetworkGraphView.h - Custom view for visualizing Bayesian network
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This view displays the Bayesian network graph with nodes and edges.
 */

#import <Cocoa/Cocoa.h>
#import "BayesianNetworkWrapper.h"

/**
 * NetworkGraphView displays the Bayesian network as a graph
 */
@interface NetworkGraphView : NSView

// Bayesian network wrapper
@property (nonatomic, strong) BayesianNetworkWrapper *network;
// Selected node for interaction
@property (nonatomic, strong) NSString *selectedNodeId;
// Evidence nodes (highlighted)
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *evidence;
// Query nodes (highlighted)
@property (nonatomic, strong) NSArray<NSString *> *queryNodes;
// Inference results to display
@property (nonatomic, strong) BNInferenceResult *inferenceResult;

/**
 * Update the view with new network data
 */
- (void)updateNetwork;

/**
 * Perform layout of nodes (automatic positioning)
 */
- (void)layoutNodes;

@end
