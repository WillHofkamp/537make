////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecificationGraph.h
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "buildSpecification.h"
#include "makeFileParser.h"

int showDependencies(GraphNode** graph);

GraphNode** createOrderedGraph(GraphNode* root, GraphNode** graph);

void searchGraph(GraphNode* node, GraphNode** ordered);

#endif
