////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecificationGraph.h
// This File Description:
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef SPECIFICATION_GRAPH
#define SPECIFICATION_GRAPH

typedef struct{
	char *name;
	char **commands;
	char **dependencies;
	char **parents;
	int numOfDependencies;
	int numOfCommands;
	int numOfParents;
	int checked;
}Node;

typedef struct{
	int numInOrderedArray;
	int size;
	Node **nodes;
}SpecificationGraph;

Node* createGraphNode();

SpecificationGraph* createSpecificationGraph();

void insertNode(SpecificationGraph *graph, Node *newNode);

char** getGraphOrdering(SpecificationGraph *graph);

void getLowestNode(SpecificationGraph *graph, Node* node, char** orderedArray, int* arrayIndex);

Node* findNode(SpecificationGraph *graph, char* name);

int needToAddNode(Node *newNode);

int checkGraphForCycle(SpecificationGraph *graph);

void lookAtAllNodes(SpecificationGraph *graph, Node* node);

int isRelative(SpecificationGraph *graph, Node *currNode, char* element);

int addAlready(Node *currNode, char* parentName);

int isNotDependencyOfClean(Node *node);

#endif
