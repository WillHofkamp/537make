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
	int numNodes = 0;
	// List of strings parsed as dependencies
	char** dList;

	// find the number of nodes
	while(graph[numNodes] != NULL && numNodes < MAX_NUM_NODES) {
		numNodes++;
	}

	// allows us to add new nodes to the graph
	int nextNodeIndex = numNodes;

	// loop through nodes to get dependencies
	// if successful, graph[i] is parent of graph[k]
	for (int i = 0; i < numNodes; i++) {
		if (graph[i] == NULL) {
			//printf("%i = numNodes\n", numNodes);
			return 0;
		}
		currNode = graph[i];
		//printf("----TARGET: %s----\n", currNode->name);
		dList = parseTargetDependencies(currNode->line);

		// Null handling for dList
		if (dList == NULL) {
			return 0;
		}

		// loop through dependencies to see if they are nodes
		int j = 0;
		while (dList[j] != NULL) {
			// search for a node with that name
			nodeCheck = findNode(dList[j],graph);
			//printf("%s searched\n", dList[j]);
			// if a node is found...
			if (nodeCheck != NULL) {
				//printf("\tsearch success\n");
				// add dependencies
				addChildToParent(graph[i], nodeCheck);
			}
			// otherwise, it's a dependency but not a target
			// make a node for it, EVEN IF IT'S NOT A FILE
			else {
				//create node
					graph[nextNodeIndex] = createNode(dList[j], -1);
					addChildToParent(graph[i], graph[nextNodeIndex]);
					nextNodeIndex++;
			}
			j++;
		}

		// free dList before loop
		for (int f = 0; f < MAX_NUM_NODES; f++) {
			// pay respects
			free(dList[f]);
			dList[f] = NULL;
			//printf("D : %s\n", dList[f]);
		}
		free(dList);

	}

	return 0;
}

//This method creates the order the makefile will be built
//this is also where the cycle checking in, and uses depth first search
GraphNode** createGraphOrder(GraphNode* root, GraphNode** graph) {
	// handle NULL input - default case
	if (root == NULL) {
		// root is the first target listed in Makefile
		root = graph[0];
	}
	//  initialize new buildOrder array
	GraphNode** order = malloc(sizeof(GraphNode*)*MAX_NUM_NODES);
	for (int i = 0; i < MAX_NUM_NODES; i++) {
		order[i] = NULL;
	}
	// call searchDepthFirst on EVERY node, w/o write permission
	int j = 0;
	while (graph[j] != NULL) {
		searchDepthFirst(graph[j], NULL);
		// reset for next searchDepthFirst
		int k = 0;
		while (graph[k] != NULL) {
			graph[k]->checked = 0;
			graph[k]->recur = 0;
			k++;
		}
		j++;
	}

	//  call searchDepthFirst on root node
	searchDepthFirst(root, order);

	return order;
}

//This method is the depth first search function which builds the ordered graph
//and also detects any cycles
void searchDepthFirst(GraphNode* node, GraphNode** order) {
	// finds if the node is in a loop
	if (node->recur == 1) {
		fprintf(stderr, "%i: Error: loop in dependencies detected\n", node->line);
		exit(0);
	}
	if (node->checked == 1) {
		return;
	}
	node->checked = 1;
	node->recur = 1;

        for (int i = 0; i < node->numchild; i++) {
		// for each unchecked child
		// it it's a target
		// recursive call
		if (node->children[i]->line > 0) {
			searchDepthFirst(node->children[i], order);
		}
        }
	// once you're here, you havent found a cycle
	node->recur = 0;

        // once your done searchDepthFirst'ing through node's children
        // you're ready to add it to order
	
	// IF ORDER == NULL, THIS searchDepthFirst DOESNT HAVE WRITE PERMISSION
	// DO NOT CONTINUE
	if (order == NULL) {
		return;
	}

	// only get here with WRITE PERMISSION
        int j = 0;
        while (j < MAX_NUM_NODES) {
		if (order[j] == NULL) {
			break;
		}
                j++;
        }
        // settles on next index w/o a node
        order[j] = node;

        return;
}

// Taking the command line arguments, this method determines
// the build mode (either default or from a certain target)
//This method takes in the command line arguments, and chooses the
//graph's root based on that.
GraphNode* findGraphRoot(int argc, const char* argv[], GraphNode** graph) {
	// default, NULL case
	if (argc == 1) {
		return NULL;
	}
	// check for proper input
	else if (argc == 2) {
		char argument[BUFFER];
		strcpy(argument, argv[1]);
		GraphNode* root = findNode(argument, graph);
		if (root == NULL) {
			fprintf(stderr,"Error: input is not valid\n");
			exit(1);
		}
		// not null, found a fitting root
		return root;
	}
	else {
		fprintf(stderr, "Error: too many arguments used\n");
		exit(1);
	}
	return NULL;
}
