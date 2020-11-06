////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecification.c
// This File Description: This file builds the nodes to put in the graph based
//						  on lines in the makefile
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "buildSpecification.h"

int memoryScale = 100;
int totalNodesNum = MAX_NODE_LIST_SIZE;


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
    // memory is allocated for children
	node->children = malloc(sizeof(GraphNode*)*MAX_NODE_LIST_SIZE);
	node->numchild = 0;

    return node;
}

//This method frees a node based on the pointer
//of a node passed in.
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
	//create graph
	GraphNode** graph = malloc(sizeof(GraphNode*)*MAX_NODE_LIST_SIZE);
	for (int i = 0; i < MAX_NODE_LIST_SIZE; i++) {
		graph[i] = NULL;
	}

	//allocate space for target string and temporary node
	char* targetString = calloc(BUFFER, sizeof(char));
	GraphNode* tempNode;

	FILE *f = openMakeFile();
	int lineNum = parseMakeTargets(targetString, f);
	int i = 0;
	while (i < BUFFER && lineNum > 0) {
		//create a node for this target
		tempNode = createNode(targetString, lineNum);
		//duplicate check
		if (findNode(tempNode->name, graph) != NULL) {
			fprintf(stderr, "Error: targets have multiples of name: %s\n", tempNode->name);
			exit(1);
		}
		//add node to graph
		graph[i] = tempNode;
		i++;

		//if max node size is reached, then realloc for more space
		if (i < BUFFER && i == MAX_NODE_LIST_SIZE ) {
      		graph = realloc(graph,sizeof(GraphNode*)*(totalNodesNum * memoryScale));
      		if(totalNodesNum == 0){
     			totalNodesNum = MAX_NODE_LIST_SIZE * memoryScale;
      		} else {
      			totalNodesNum = totalNodesNum * memoryScale;
     		}
		}
		lineNum = parseMakeTargets(targetString, f);
	}
	// Close the file
	lineNum = 0;
	fclose(f);
	free(targetString);
  
    return graph;
}

//This method finds a node that has a certain name in the graph
//and returns either a node or null if it doesn't exist
GraphNode* findNode(char* name, GraphNode** graph) {
    int i = 0;
    //iterate until the name is found
    while (graph[i] != NULL && i < MAX_NODE_LIST_SIZE) {
        if (strcmp(name, graph[i]->name) == 0) {
            return graph[i];
        }
        else {
            i++;
        }
    }
    return NULL;

}

//This method adds a child to a parent's list
void addChildToParent(GraphNode* parent, GraphNode* child) {
	//set parent of child
	child->parent = parent;

	//set child to parent
	parent->children[parent->numchild] = child;
	parent->numchild++;
	return;
}