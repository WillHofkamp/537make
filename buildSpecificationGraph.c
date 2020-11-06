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
//the connection between targets and dependencies
int connectNodes(GraphNode** graph) {
	GraphNode* currNode;
	GraphNode* nodeCheck;
	int graphSize = 0;
	char** dependencies;

	//find the next available spot to add node 
	while(graph[graphSize] != NULL && graphSize < MAX_NUM_NODES) {
		graphSize++;
	}
	// allows us to add new nodes to the graph
	int nextNodeIndex = graphSize;

	// loop through nodes to get dependencies
	// if successful, graph[i] is parent of graph[k]
	for (int i = 0; i < graphSize; i++) {
		if (graph[i] == NULL) {
			return 0;
		}
		currNode = graph[i];
		dependencies = parseTargetDependencies(currNode->line);

		// Null handling for dependencies
		if (dependencies == NULL) {
			return 0;
		}

		// loop through dependencies to see if they are nodes
		int j = 0;
		while (dependencies[j] != NULL) {
			// search for a node with that name
			nodeCheck = findNode(dependencies[j],graph);
			// if a node is found...
			if (nodeCheck != NULL) {
				// add dependencies
				addChildToParent(graph[i], nodeCheck);
			}
			// otherwise, it's a dependency but not a target
			// make a node for it, EVEN IF IT'S NOT A FILE
			else {
				//create node
					graph[nextNodeIndex] = createNode(dependencies[j], -1);
					addChildToParent(graph[i], graph[nextNodeIndex]);
					nextNodeIndex++;
			}
			j++;
		}

		// free dependencies before loop
		for (int f = 0; f < MAX_NUM_NODES; f++) {
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
	GraphNode** order = malloc(sizeof(GraphNode*)*MAX_NUM_NODES);
	for (int i = 0; i < MAX_NUM_NODES; i++) {
		order[i] = NULL;
	}

	//find the size of the graph
	int graphSize = 0;
	while(graph[graphSize] != NULL && graphSize < MAX_NUM_NODES) {
		graphSize++;
	}

	//do a depth first search on every node and check for initial cycles
	for(int j = 0; j < graphSize; j++) {
		if(graph[j] != NULL) {
			searchDepthFirst(graph[j], NULL);
			// reset for next searchDepthFirst
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
	searchDepthFirst(root, order);

	return order;
}

//This method is the depth first search function which builds the ordered graph
//and also detects any cycles
void searchDepthFirst(GraphNode* node, GraphNode** ordered) {
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
			searchDepthFirst(node->children[i], ordered);
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
	for(int j = 0; j < MAX_NUM_NODES; j++) {
		if(ordered[j] == NULL) {
			ordered[j] = node;
			break;
		}
	}
    return;
}