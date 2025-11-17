/*
 * ViewController.m - Implementation of main view controller
 * Copyright (C) 2025, Shyamal Chandra
 */

#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Initialize network wrapper
    BayesianNetworkWrapper *network = [[BayesianNetworkWrapper alloc] init];
    self.graphView.network = network;
    
    // Load example network
    [self loadExampleNetwork:nil];
}

- (IBAction)performInference:(id)sender {
    if (!self.graphView.network) {
        self.statusLabel.stringValue = @"Error: No network loaded";
        return;
    }
    
    // Parse query nodes
    NSString *queryText = self.queryNodesField.stringValue;
    NSArray<NSString *> *queryNodes = [queryText componentsSeparatedByString:@","];
    NSMutableArray<NSString *> *trimmedQueryNodes = [NSMutableArray array];
    for (NSString *nodeId in queryNodes) {
        NSString *trimmed = [nodeId stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        if (trimmed.length > 0) {
            [trimmedQueryNodes addObject:trimmed];
        }
    }
    
    // Parse evidence
    NSString *evidenceText = self.evidenceField.stringValue;
    NSMutableDictionary<NSString *, NSString *> *evidence = [NSMutableDictionary dictionary];
    
    NSArray<NSString *> *evidencePairs = [evidenceText componentsSeparatedByString:@","];
    for (NSString *pair in evidencePairs) {
        NSArray<NSString *> *keyValue = [pair componentsSeparatedByString:@"="];
        if (keyValue.count == 2) {
            NSString *key = [keyValue[0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
            NSString *value = [keyValue[1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
            evidence[key] = value;
        }
    }
    
    // Perform inference
    BNInferenceResult *result = [self.graphView.network performInferenceWithQueryNodes:trimmedQueryNodes
                                                                                evidence:evidence];
    
    if (result) {
        // Update graph view
        self.graphView.queryNodes = trimmedQueryNodes;
        self.graphView.evidence = evidence;
        self.graphView.inferenceResult = result;
        [self.graphView updateNetwork];
        
        // Display results
        NSMutableString *resultsText = [NSMutableString string];
        [resultsText appendString:@"Inference Results:\n\n"];
        
        for (NSDictionary<NSString *, NSString *> *assignment in result.probabilities) {
            NSNumber *prob = result.probabilities[assignment];
            NSMutableString *assignmentStr = [NSMutableString string];
            for (NSString *key in assignment) {
                [assignmentStr appendFormat:@"%@=%@ ", key, assignment[key]];
            }
            [resultsText appendFormat:@"P(%@) = %.6f\n", assignmentStr, prob.doubleValue];
        }
        
        self.resultsTextView.string = resultsText;
        self.statusLabel.stringValue = @"Inference completed successfully";
    } else {
        NSString *error = [self.graphView.network lastErrorMessage];
        self.statusLabel.stringValue = [NSString stringWithFormat:@"Error: %@", error ?: @"Unknown error"];
        self.resultsTextView.string = @"";
    }
}

- (IBAction)loadExampleNetwork:(id)sender {
    BayesianNetworkWrapper *network = self.graphView.network;
    
    // Clear existing network
    network = [[BayesianNetworkWrapper alloc] init];
    self.graphView.network = network;
    
    // Create medical diagnosis example
    [network addNodeWithId:@"Disease" name:@"Disease" states:@[@"None", @"Cold", @"Flu"]];
    [network addNodeWithId:@"Symptom1" name:@"Fever" states:@[@"No", @"Yes"]];
    [network addNodeWithId:@"Symptom2" name:@"Cough" states:@[@"No", @"Yes"]];
    
    [network addEdgeFromParent:@"Disease" toChild:@"Symptom1"];
    [network addEdgeFromParent:@"Disease" toChild:@"Symptom2"];
    
    // Note: Setting CPTs requires lower-level API access
    // For now, we'll just show the structure
    // In a full implementation, you'd set up the CPTs here
    
    [self.graphView updateNetwork];
    self.statusLabel.stringValue = @"Example network loaded (Medical Diagnosis)";
    self.queryNodesField.stringValue = @"Disease";
    self.evidenceField.stringValue = @"Symptom1=Yes, Symptom2=Yes";
}

- (IBAction)clearNetwork:(id)sender {
    self.graphView.network = [[BayesianNetworkWrapper alloc] init];
    [self.graphView updateNetwork];
    self.queryNodesField.stringValue = @"";
    self.evidenceField.stringValue = @"";
    self.resultsTextView.string = @"";
    self.statusLabel.stringValue = @"Network cleared";
}

@end
