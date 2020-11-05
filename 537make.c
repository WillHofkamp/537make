////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: 537make.c
// This File Description: The main file of the program which calls everything to run
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include "buildSpecificationGraph.h"
#include "executeSpecificationGraph.h"

void testParseCommandLine();
void testGetNodes();
void freeGraphMemory(GraphNode **graph);

int main(int argc, const char* argv[]) {
	GraphNode** graph = getNodes();
	GraphNode* root = findGraphRoot(argc, argv, graph);
	connectNodes(graph);
	GraphNode** order = createGraphOrder(root, graph);
	checkNodes(order);
	freeGraphMemory(graph);
	free(order);
}

//Frees the memory that holds the graph
//for proper memory usage
void freeGraphMemory(GraphNode **graph) {
	int index = 0;
	while (graph[index] != NULL) {
		GraphNode *node = graph[index];
		freeNode(node);
		index++;
	}
	free(graph);
}

