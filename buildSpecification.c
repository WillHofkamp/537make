////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecification.c
// This File Description: 
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "buildSpecification.h"

int multiplier = 100;
int maxNodesTracker = 64;


//This method creates a new node in the graph
//based on the name of the file and the line number
//returns the new node
GraphNode* createNode(char *name, int line) {
	//allocates memory for node
    GraphNode* node = malloc(sizeof(GraphNode));
    node->name = malloc(BUFFER);
    strcpy(node->name, name);

	//set data
    node->line = line;
    node->visited = 0;
	node->recurred = 0;
    // parent and children are null, but memory is allocated for children
	node->children = malloc(sizeof(GraphNode*)*MAX_NUM_NODES);
	node->numchild = 0;

    return node;
}

//This method frees a node based on the pointer
//of a node passed in. 0 is returned if it is freed
//properly
int freeNode(GraphNode* node) {
	//free any data
    free(node->name);
    node->name = NULL;
	free(node->children);
	node->children = NULL;

	//finally free node
	free(node);
	node = NULL;
    return 0;
}

//This input reads the makefile and turns the targets into nodes
GraphNode** getNodes() {
	// Returned array of TreeNodes
	GraphNode** graph = malloc(sizeof(GraphNode*)*MAX_NUM_NODES);
	// initialize graph to NULL
	for (int i = 0; i < MAX_NUM_NODES; i++) {
		graph[i] = NULL;
	}
	// ints and buffer
	int nodeIndex = 0;
	int lineNum = 0;
	char* targetBuff = calloc(BUFFER, sizeof(char));
	GraphNode* nodeCheck;

	// Pointer to the open file
	FILE *f = openMakeFile();
	// first parseMakeTargets call
	lineNum = parseMakeTargets(targetBuff, f);
	while (nodeIndex < BUFFER && lineNum > 0) {
		// parseMakeTargets finds the next line with a viable
		// target and copies it into the input buffer. Then
		// it returns the line number it found it on
		nodeCheck = createNode(targetBuff, lineNum);
		// checking for multiple targets with the same name
		if (findNode(nodeCheck->name, graph) != NULL) {
			fprintf(stderr, "Error: targets have multiples of name: %s\n", nodeCheck->name);
			exit(1);
		}
		graph[nodeIndex] = nodeCheck;
		nodeIndex++;
		if (nodeIndex < BUFFER && nodeIndex == MAX_NUM_NODES ) {
      		graph = realloc(graph,sizeof(GraphNode*)*(maxNodesTracker * multiplier));
      		if(maxNodesTracker == 0){
     			maxNodesTracker = MAX_NUM_NODES * multiplier;
      		} else {
      			maxNodesTracker = maxNodesTracker * multiplier;
     		}
		}
		lineNum = parseMakeTargets(targetBuff, f);
	}
	// Close the file
	closeMakeFile(f);
	free(targetBuff);
  
    return graph;
}

//This method finds a node that has a certain name in the graph
//and returns either a node or null if it doesn't exist
GraphNode* findNode(char* name, GraphNode** graph) {
        int index = 0;
        int cmp;
        // loop until you've hit the max or the end
        // nodes are filled into the graph L to R
        while (index < MAX_NUM_NODES && graph[index] != NULL) {
                cmp = strcmp(name, graph[index]->name);
                if (cmp == 0) {
                    return graph[index];
                }
                else {
                    index++;
                }
        }
        // if it leaves the loop without finding a node
        return NULL;

}

//This method adds a child to a parent's list
void addChildToParent(GraphNode* parent, GraphNode* child) {
	// set child's parent node
	child->parent = parent;
	// add child to parent's array
	parent->children[parent->numchild] = child;
	parent->numchild++;
	// no space for more children
	return;
}

//This method prints out the graph
void printGraph(GraphNode** graph) {
	int i = 0;
	while (i < MAX_NUM_NODES && graph[i] != NULL) {
		fprintf(stderr, "@%i:\t-%s-\n", graph[i]->line, graph[i]->name);
		if (graph[i]->numchild > 0) {
			fprintf(stderr, "\t. ");
		}
		for (int j = 0; j < graph[i]->numchild; j++) {
			fprintf(stderr, "%s . ", graph[i]->children[j]->name);
		}
		if (graph[i]->numchild > 0) {
			fprintf(stderr, "\n");
		}
		i++;
	}
	return;
}


