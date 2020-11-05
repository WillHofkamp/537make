////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecificationGraph.c
// This File Description: This method builds the specification graph
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "buildSpecificationGraph.h"

//This method parses the makefile for dependencies, then sets
//the links between targets and dependencies
int showDependencies(GraphNode** graph) {
	GraphNode* currNode;
	GraphNode* nodeCheck;
	int numNodes = 0;
	// List of parsed dependencies
	char** parsedStrings;

	// find the number of nodes
	while(graph[numNodes] != NULL && numNodes < MAX_NUM_NODES) {
		numNodes++;
	}

	// index for the next node to be added at
	int nextNodeIndex = numNodes;

	// loop through nodes to get dependencies
	for (int i = 0; i < numNodes; i++) {
		if (graph[i] == NULL) {
			return 0;
		}
		currNode = graph[i];
		parsedStrings = parseTargetDependencies(currNode->line);

		if (parsedStrings == NULL) {
			return 0;
		}

		// loop through dependencies to see if they are nodes
		int j = 0;
		while (parsedStrings[j] != NULL) {
			// search for a node with that name
			nodeCheck = findNode(parsedStrings[j],graph);
			// if a node is found
			if (nodeCheck != NULL) {
				// add dependencies
				addChildToParent(graph[i], nodeCheck);
			}
			//a dependency but not a target
			else {
				//create node
					graph[nextNodeIndex] = createNode(parsedStrings[j], -1);
					addChildToParent(graph[i], graph[nextNodeIndex]);
					nextNodeIndex++;
			}
			j++;
		}

		// free parsedStrings before loop
		int k = 0;
		while (k < MAX_NODES)
		{
			free(parsedStrings[k]);
			parsedStrings[k] = NULL;
			k++;
		}
		free(parsedStrings);
	}
	return 0;
}

//This method creates the order the makefile will be built
//this is also where the cycle checking in, and uses depth first search
GraphNode** createFlow(GraphNode* root, GraphNode** graph) {
	if (root == NULL) {
		// first target in makefile
		root = graph[0];
	}
	//  initialize new buildOrder array
	GraphNode** order = malloc(sizeof(GraphNode*)*MAX_NUM_NODES);
	for (int i = 0; i < MAX_NUM_NODES; i++) {
		order[i] = NULL;
	}
	// run DFS on each node
	int j = 0;
	while (graph[j] != NULL) {
		searchDepthFirst(graph[j], NULL);
		// reset for next searchDepthFirst
		int k = 0;
		while (graph[k] != NULL) {
			graph[k]->checked = 0;
			graph[k]->repeated = 0;
			k++;
		}
		j++;
	}

	//  call searchDepthFirst on root node
	searchDepthFirst(root, order);

	return order;
}

//Runs DFS to build the ordered graph
void searchDepthFirst(GraphNode* node, GraphNode** order) {  
	if (node->repeated == 1) {
		fprintf(stderr, "Loop in dependencies detected\n");
		exit(0);
	}
	if (node->checked == 1) {
		return;
	}
	node->checked = 1;
	node->repeated = 1;

        for (int i = 0; i < node->numchild; i++) {
		// if child is a target
		if (node->children[i]->line > 0) {
			searchDepthFirst(node->children[i], order);
		}
        }
	// No cycle found
	node->repeated = 0;

	// No write permission
	if (order == NULL) {
		return;
	}
        int j = 0;
        while (j < MAX_NUM_NODES) {
		if (order[j] == NULL) {
			break;
		}
                j++;
        }
        // settles on next index without a node
        order[j] = node;

        return;
}


// Determines the build mode and
//graph's root based on command line arguments.
GraphNode* findGraphRoot(int argc, const char* argv[], GraphNode** graph) {
	char cmdArg[BUFFER_SIZE];

	if (argc == 2)
	{
		strcpy(cmdArg, argv[1]);
		GraphNode *root = find(cmdArg, graph);
		if (root == NULL)
		{
			fprintf(stderr, "Invalid input\n");
			exit(1);
		}
		return root;
	}
	else if (argc > 2)
	{
		fprintf(stderr, "Too many arguments provided\n");
		exit(1);
	}
	return NULL;
}
