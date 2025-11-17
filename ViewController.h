/*
 * ViewController.h - Main view controller for Bayesian network visualization
 * Copyright (C) 2025, Shyamal Chandra
 */

#import <Cocoa/Cocoa.h>
#import "NetworkGraphView.h"
#import "BayesianNetworkWrapper.h"

@interface ViewController : NSViewController

// Network graph view
@property (nonatomic, weak) IBOutlet NetworkGraphView *graphView;
// Query nodes text field
@property (nonatomic, weak) IBOutlet NSTextField *queryNodesField;
// Evidence text field
@property (nonatomic, weak) IBOutlet NSTextField *evidenceField;
// Results text view
@property (nonatomic, weak) IBOutlet NSTextView *resultsTextView;
// Status label
@property (nonatomic, weak) IBOutlet NSTextField *statusLabel;

// Perform inference button action
- (IBAction)performInference:(id)sender;
// Load example network button action
- (IBAction)loadExampleNetwork:(id)sender;
// Clear network button action
- (IBAction)clearNetwork:(id)sender;

@end
