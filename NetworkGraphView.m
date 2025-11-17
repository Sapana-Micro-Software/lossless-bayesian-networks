/*
 * NetworkGraphView.m - Implementation of network graph visualization
 * Copyright (C) 2025, Shyamal Chandra
 *
 * This view renders the Bayesian network with exact precision.
 */

#import "NetworkGraphView.h"

// Node radius for drawing
#define NODE_RADIUS 40.0
// Edge line width
#define EDGE_WIDTH 2.0
// Padding around view
#define PADDING 50.0
// Minimum distance between nodes
#define MIN_NODE_DISTANCE 120.0

@implementation NetworkGraphView

- (instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    if (self) {
        _network = nil;
        _selectedNodeId = nil;
        _evidence = @{};
        _queryNodes = @[];
        _inferenceResult = nil;
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Clear background
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);
    
    if (!self.network || self.network.nodes.count == 0) {
        // Draw placeholder text
        NSString *text = @"No network loaded";
        NSDictionary *attributes = @{
            NSFontAttributeName: [NSFont systemFontOfSize:24],
            NSForegroundColorAttributeName: [NSColor grayColor]
        };
        NSSize textSize = [text sizeWithAttributes:attributes];
        NSPoint textPoint = NSMakePoint(
            (self.bounds.size.width - textSize.width) / 2,
            (self.bounds.size.height - textSize.height) / 2
        );
        [text drawAtPoint:textPoint withAttributes:attributes];
        return;
    }
    
    // Draw edges first (so they appear behind nodes)
    [self drawEdges];
    
    // Draw nodes
    [self drawNodes];
    
    // Draw labels and probabilities
    [self drawLabels];
}

- (void)drawEdges {
    NSBezierPath *path = [NSBezierPath bezierPath];
    [path setLineWidth:EDGE_WIDTH];
    [[NSColor darkGrayColor] setStroke];
    
    for (BNEdge *edge in self.network.edges) {
        BNNode *fromNode = [self findNodeWithId:edge.fromNodeId];
        BNNode *toNode = [self findNodeWithId:edge.toNodeId];
        
        if (fromNode && toNode) {
            NSPoint fromPoint = NSMakePoint(fromNode.x, fromNode.y);
            NSPoint toPoint = NSMakePoint(toNode.x, toNode.y);
            
            // Calculate points on node boundaries
            CGFloat dx = toPoint.x - fromPoint.x;
            CGFloat dy = toPoint.y - fromPoint.y;
            CGFloat distance = sqrt(dx * dx + dy * dy);
            
            if (distance > 0) {
                CGFloat fromRadius = NODE_RADIUS;
                CGFloat toRadius = NODE_RADIUS;
                
                NSPoint fromBoundary = NSMakePoint(
                    fromPoint.x + (dx / distance) * fromRadius,
                    fromPoint.y + (dy / distance) * fromRadius
                );
                NSPoint toBoundary = NSMakePoint(
                    toPoint.x - (dx / distance) * toRadius,
                    toPoint.y - (dy / distance) * toRadius
                );
                
                [path moveToPoint:fromBoundary];
                [path lineToPoint:toBoundary];
            }
        }
    }
    
    [path stroke];
}

- (void)drawNodes {
    for (BNNode *node in self.network.nodes) {
        NSPoint center = NSMakePoint(node.x, node.y);
        
        // Determine node color based on state
        NSColor *nodeColor;
        if ([self.queryNodes containsObject:node.nodeId]) {
            // Query node - blue
            nodeColor = [NSColor colorWithRed:0.2 green:0.4 blue:1.0 alpha:1.0];
        } else if (self.evidence[node.nodeId]) {
            // Evidence node - green
            nodeColor = [NSColor colorWithRed:0.2 green:0.8 blue:0.2 alpha:1.0];
        } else if ([self.selectedNodeId isEqualToString:node.nodeId]) {
            // Selected node - orange
            nodeColor = [NSColor colorWithRed:1.0 green:0.6 blue:0.2 alpha:1.0];
        } else {
            // Default node - light gray
            nodeColor = [NSColor colorWithRed:0.9 green:0.9 blue:0.9 alpha:1.0];
        }
        
        // Draw node circle
        NSBezierPath *circle = [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(
            center.x - NODE_RADIUS,
            center.y - NODE_RADIUS,
            NODE_RADIUS * 2,
            NODE_RADIUS * 2
        )];
        
        [nodeColor setFill];
        [[NSColor blackColor] setStroke];
        [circle setLineWidth:2.0];
        [circle fill];
        [circle stroke];
        
        // Draw node name
        NSDictionary *nameAttributes = @{
            NSFontAttributeName: [NSFont boldSystemFontOfSize:12],
            NSForegroundColorAttributeName: [NSColor blackColor]
        };
        NSSize nameSize = [node.name sizeWithAttributes:nameAttributes];
        NSPoint namePoint = NSMakePoint(
            center.x - nameSize.width / 2,
            center.y - nameSize.height / 2
        );
        [node.name drawAtPoint:namePoint withAttributes:nameAttributes];
    }
}

- (void)drawLabels {
    // Draw probability information for query nodes
    if (self.inferenceResult && self.queryNodes.count > 0) {
        CGFloat yOffset = 20.0;
        NSFont *font = [NSFont systemFontOfSize:10];
        NSColor *textColor = [NSColor blackColor];
        
        for (NSString *queryNodeId in self.queryNodes) {
            BNNode *node = [self findNodeWithId:queryNodeId];
            if (!node) continue;
            
            NSPoint labelPoint = NSMakePoint(node.x, node.y + NODE_RADIUS + yOffset);
            
            // Find probabilities for this query node
            NSMutableString *probText = [NSMutableString string];
            for (NSDictionary<NSString *, NSString *> *assignment in self.inferenceResult.probabilities) {
                NSString *nodeState = assignment[queryNodeId];
                if (nodeState) {
                    NSNumber *prob = self.inferenceResult.probabilities[assignment];
                    [probText appendFormat:@"P(%@=%@)=%.4f\n", node.name, nodeState, prob.doubleValue];
                }
            }
            
            if (probText.length > 0) {
                NSDictionary *attributes = @{
                    NSFontAttributeName: font,
                    NSForegroundColorAttributeName: textColor,
                    NSBackgroundColorAttributeName: [NSColor colorWithWhite:1.0 alpha:0.9]
                };
                [probText drawAtPoint:labelPoint withAttributes:attributes];
                yOffset += 60.0;
            }
        }
    }
}

- (BNNode *)findNodeWithId:(NSString *)nodeId {
    for (BNNode *node in self.network.nodes) {
        if ([node.nodeId isEqualToString:nodeId]) {
            return node;
        }
    }
    return nil;
}

- (void)updateNetwork {
    [self layoutNodes];
    [self setNeedsDisplay:YES];
}

- (void)layoutNodes {
    if (!self.network || self.network.nodes.count == 0) {
        return;
    }
    
    // Simple force-directed layout algorithm
    NSArray<BNNode *> *nodes = self.network.nodes;
    NSSize viewSize = self.bounds.size;
    
    // Initialize positions in a circle
    CGFloat centerX = viewSize.width / 2;
    CGFloat centerY = viewSize.height / 2;
    CGFloat radius = MIN(viewSize.width, viewSize.height) / 3;
    
    for (NSUInteger i = 0; i < nodes.count; i++) {
        BNNode *node = nodes[i];
        CGFloat angle = (2.0 * M_PI * i) / nodes.count;
        node.x = centerX + radius * cos(angle);
        node.y = centerY + radius * sin(angle);
    }
    
    // Apply force-directed layout iterations
    for (NSInteger iteration = 0; iteration < 100; iteration++) {
        // Repulsion between all nodes
        for (BNNode *node1 in nodes) {
            CGFloat fx = 0.0, fy = 0.0;
            
            for (BNNode *node2 in nodes) {
                if (node1 == node2) continue;
                
                CGFloat dx = node1.x - node2.x;
                CGFloat dy = node1.y - node2.y;
                CGFloat distance = sqrt(dx * dx + dy * dy);
                
                if (distance > 0) {
                    // Repulsion force
                    CGFloat force = 1000.0 / (distance * distance);
                    fx += (dx / distance) * force;
                    fy += (dy / distance) * force;
                }
            }
            
            // Attraction along edges
            for (BNEdge *edge in self.network.edges) {
                if ([edge.toNodeId isEqualToString:node1.nodeId]) {
                    BNNode *parent = [self findNodeWithId:edge.fromNodeId];
                    if (parent) {
                        CGFloat dx = node1.x - parent.x;
                        CGFloat dy = node1.y - parent.y;
                        CGFloat distance = sqrt(dx * dx + dy * dy);
                        
                        if (distance > 0) {
                            // Attraction force
                            CGFloat force = distance * 0.01;
                            fx -= (dx / distance) * force;
                            fy -= (dy / distance) * force;
                        }
                    }
                }
            }
            
            // Update position
            node1.x += fx * 0.1;
            node1.y += fy * 0.1;
            
            // Keep within bounds
            node1.x = MAX(PADDING + NODE_RADIUS, MIN(viewSize.width - PADDING - NODE_RADIUS, node1.x));
            node1.y = MAX(PADDING + NODE_RADIUS, MIN(viewSize.height - PADDING - NODE_RADIUS, node1.y));
        }
    }
}

- (void)mouseDown:(NSEvent *)event {
    NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
    
    // Find clicked node
    for (BNNode *node in self.network.nodes) {
        CGFloat dx = location.x - node.x;
        CGFloat dy = location.y - node.y;
        CGFloat distance = sqrt(dx * dx + dy * dy);
        
        if (distance <= NODE_RADIUS) {
            self.selectedNodeId = node.nodeId;
            [self setNeedsDisplay:YES];
            break;
        }
    }
}

@end
