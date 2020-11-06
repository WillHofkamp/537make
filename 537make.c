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

int main(int argc, const char* argv[]) {
	//decide makefile name to use based on args
	if(argc == 1) {
		setFileName("makefile");
	} else if((argc == 4 || argc == 3) && (strcmp(argv[1], "-f") == 0)) {
		setFileName(argv[2]);
	} else {
		fprintf(stderr, "Error: incorrect number of arguments used\n");
		exit(1);
	}
	GraphNode** graph = getNodes();
	GraphNode* root;
	//decide the root target based on args
	if (argc == 1 || argc == 3) {
		root = NULL;
	}
	else if (argc == 4) {
		char argument[BUFFER];
		strcpy(argument, argv[3]);
		root = findNode(argument, graph);
		if (root == NULL) {
			fprintf(stderr,"Error: input is not valid\n");
			exit(1);
		}
	}
	showDependencies(graph);
	GraphNode** order = createOrderedGraph(root, graph);
	checkNodes(order);
	freeGraphMemory(graph);
	free(order);
}

