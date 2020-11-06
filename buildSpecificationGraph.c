////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecificationGraph.c
// This File Description: This class builds the specification graph
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
	int graphSize = 0;
	char** dependencies;

	//find the next available spot to add node 
	while(graph[graphSize] != NULL && graphSize < MAX_NODE_LIST_SIZE) {
		graphSize++;
	}
	
	int nextNodeIndex = graphSize;

	// looping through nodes to get dependencies
	for (int i = 0; i < graphSize; i++) {
		if (graph[i] == NULL) {
			return 0;
		}
		currNode = graph[i];
		dependencies = parseTargetDependencies(currNode->line);

		
		if (dependencies == NULL) {
			return 0;
		}

		// loop through dependencies to see if they are nodes
		int j = 0;
		while (dependencies[j] != NULL) {
			nodeCheck = findNode(dependencies[j],graph);
			if (nodeCheck != NULL) {
				//form dependencies
				addChildToParent(graph[i], nodeCheck);
			}
			else {
				//create node
				graph[nextNodeIndex] = createNode(dependencies[j], -1);
				addChildToParent(graph[i], graph[nextNodeIndex]);
				nextNodeIndex++;
			}
			j++;
		}
		
		for (int f = 0; f < MAX_NODE_LIST_SIZE; f++) {
			free(dependencies[f]);
			dependencies[f] = NULL;
		}
		free(dependencies);

	}
	return 0;
}

//This method creates the order the makefile will be built
//this is also where the cycle checking in, and uses depth first search
GraphNode** createOrderedGraph(GraphNode* root, GraphNode** graph) {
	if (root == NULL) {
		//if root is null then -f wasn't used, so make it the first graph node
		root = graph[0];
	}

	//create space for an ordered graph
	GraphNode** order = malloc(sizeof(GraphNode*)*MAX_NODE_LIST_SIZE);
	for (int i = 0; i < MAX_NODE_LIST_SIZE; i++) {
		order[i] = NULL;
	}

	//find the size of the graph
	int graphSize = 0;
	while(graph[graphSize] != NULL && graphSize < MAX_NODE_LIST_SIZE) {
		graphSize++;
	}

	//do a depth first search on every node and check for initial cycles
	for(int j = 0; j < graphSize; j++) {
		if(graph[j] != NULL) {
			searchGraph(graph[j], NULL);
			// reset for next searchGraph
			for(int k = 0; k < graphSize; k++) {
				if(graph[k] != NULL) {
					graph[k]->visited = 0;
					graph[k]->recurred = 0;
				} else {
					break;
				}
			}
		} else {
			break;
		}
	}

	//order the graph
	searchGraph(root, order);

	return order;
}

//This method is the depth first search function which builds the ordered graph
//and also detects any cycles
void searchGraph(GraphNode* node, GraphNode** ordered) {
	//checks that node hasn't been recurred over yet
	if (node->recurred == 1) {
		fprintf(stderr, "%i: Error: loop in dependencies detected at %s\n", node->line, node->name);
		exit(0);
	}
	if (node->visited == 1) {
		return;
	}

	//otherwise use node and set flags
	node->visited = 1;
	node->recurred = 1;
	//recursively search children
    for (int i = 0; i < node->numchild; i++) {
		if (node->children[i]->line > 0) {
			searchGraph(node->children[i], ordered);
		}
    }
	//cycle was not detected in graph, so reset recurred bit
	node->recurred = 0;

	//if order is null, it is just reading and checking nodes, so exit
	if (ordered == NULL) {
		return;
	}
	//otherwise set node in order
	//find next available spot in ordered graph
	for(int j = 0; j < MAX_NODE_LIST_SIZE; j++) {
		if(ordered[j] == NULL) {
			ordered[j] = node;
			break;
		}
	}
    return;
}
