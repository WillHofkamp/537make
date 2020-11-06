////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecification.h
// This File Description: This is the interface for the class that builds 
//                        specifications for the graph
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#ifndef BUILD_SPECIFICATION
#define BUILD_SPECIFICATION

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "makeFileParser.h"

typedef struct GraphNode GraphNode;
struct GraphNode {
        char *name;
        int line, numchild, visited, recurred;
        GraphNode *parent;
        GraphNode **children;
};

GraphNode* createNode(char* name, int line);

int freeNode(GraphNode* node);

GraphNode** getNodes();

void getModificationTime(GraphNode* node);

GraphNode* findNode(char* name, GraphNode** graph);

void addChildToParent(GraphNode* parent, GraphNode* child);

#endif
