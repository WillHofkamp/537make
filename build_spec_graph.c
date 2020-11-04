/////////////////////////////////////////////////
//
// File:	build_spec_graph.c
//
// Author:	Abhi Kosuri
// 		Mike Tuchler
// All files:	build_spec_graph.c, .h
// 		build_spec_repr.c, .h
// 		proc_creation_prog_exe.c, .h
// 		text_parsing.c, .h
// 		main.c, README
//
/////////////////////////////////////////////////

#include "build_spec_graph.h"

// parses Makefile for dependencies, and sets parents/children
int connectNodes(TreeNode** graph) {
	TreeNode* currNode;
	TreeNode* nodeCheck;
	int numNodes = 0;
	// List of strings parsed as dependencies
	char** dList;

	// find the number of nodes
	while(graph[numNodes] != NULL && numNodes < MAX_NODES) {
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
		dList = parseDependencies(currNode->line);

		// Null handling for dList
		if (dList == NULL) {
			return 0;
		}

		// loop through dependencies to see if they are nodes
		int j = 0;
		while (dList[j] != NULL) {
			// search for a node with that name
			nodeCheck = find(dList[j],graph);
			//printf("%s searched\n", dList[j]);
			// if a node is found...
			if (nodeCheck != NULL) {
				//printf("\tsearch success\n");
				// add dependencies
				parentChild(graph[i], nodeCheck);
			}
			// otherwise, it's a dependency but not a target
			// make a node for it, EVEN IF IT'S NOT A FILE
			else {
				//create node
				if (nextNodeIndex < MAX_NODES) {
					graph[nextNodeIndex] = nodeInit(dList[j], -1);
					parentChild(graph[i], graph[nextNodeIndex]);
					nextNodeIndex++;
				}
				else {
					fprintf(stderr,"Error: Too many nodes.\n");
				}
			}
			j++;
		}

		// free dList before loop
		for (int f = 0; f < MAX_NODES; f++) {
			// pay respects
			free(dList[f]);
			dList[f] = NULL;
			//printf("D : %s\n", dList[f]);
		}
		free(dList);

	}

	return 0;
}

// using a DFS, creates a build order for the Makefile
// also checks for cycles, which are bad
// input:	the graph, i.e. list of node*s
// return:	list of node*s in build/execution order
TreeNode** buildOrder(TreeNode* root, TreeNode** graph) {
	// handle NULL input - default case
	if (root == NULL) {
		// root is the first target listed in Makefile
		root = graph[0];
	}
	//  initialize new buildOrder array
	TreeNode** order = malloc(sizeof(TreeNode*)*MAX_NODES);
	for (int i = 0; i < MAX_NODES; i++) {
		order[i] = NULL;
	}
	// call DFS on EVERY node, w/o write permission
	int j = 0;
	while (graph[j] != NULL) {
		DFS(graph[j], NULL);
		// reset for next DFS
		int k = 0;
		while (graph[k] != NULL) {
			graph[k]->checked = 0;
			graph[k]->recur = 0;
			k++;
		}
		j++;
	}

	//  call DFS on root node
	DFS(root, order);

	return order;
}

// DFS function, that both builds order and detects cycles
// inputs:	the node to DFS on, and the array to add stuff to build order
// return:	nothing, but it does exit if it detects a cycle
void DFS(TreeNode* node, TreeNode** order) {
	// finds if the node is in a loop
	if (node->recur == 1) {
		fprintf(stderr, "%i: Error: dependency loop detected\n", node->line);
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
			DFS(node->children[i], order);
		}
        }
	// once you're here, you havent found a cycle
	node->recur = 0;

        // once your done DFS'ing through node's children
        // you're ready to add it to order
	
	// IF ORDER == NULL, THIS DFS DOESNT HAVE WRITE PERMISSION
	// DO NOT CONTINUE
	if (order == NULL) {
		return;
	}

	// only get here with WRITE PERMISSION
        int j = 0;
        while (j < MAX_NODES) {
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
TreeNode* getRoot(int argc, const char* argv[], TreeNode** graph) {
	// default, NULL case
	if (argc == 1) {
		return NULL;
	}
	// check for proper input
	else if (argc == 2) {
		char argument[BUFF_SIZE];
		strcpy(argument, argv[1]);
		TreeNode* root = find(argument, graph);
		if (root == NULL) {
			fprintf(stderr,"Error: input is not a valid argument\n");
			exit(1);
		}
		// not null, found a fitting root
		return root;
	}
	else {
		fprintf(stderr, "Error: improper input format - too many arguments\n");
		exit(1);
	}
	return NULL;
}
