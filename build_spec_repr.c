/////////////////////////////////////////////////
//
// File:	build_spec_repr.c
// 
// Purpose:	building nodes from text parsing
// 		functions, and returning an array
// 		of TreeNodes
//
// Author:      Abhi Kosuri
//              Mike Tuchler
// All files:   build_spec_graph.c, .h
//              build_spec_repr.c, .h
//              proc_creation_prog_exe.c, .h
//              text_parsing.c, .h
//              main.c, README
//
/////////////////////////////////////////////////

#include "build_spec_repr.h"

// function to initialize a new TreeNode
// inputs:	the name of the target file
// 		the line number
// return:	pointer to the new Node
TreeNode* nodeInit(char *name, int line) {
        TreeNode* node = malloc(sizeof(TreeNode));
        // input given name and line number
        node->name = malloc(BUFF_SIZE);
        strcpy(node->name, name);
        node->line = line;
        // for use with DFS
        node->checked = 0;
	node->recur = 0;
        // parent and children remain NULL
	// create space for children
	node->children = malloc(sizeof(TreeNode*)*MAX_NODES);
	node->numchild = 0;

        return node;
}

// function to free a node (may delete)
// inputs:	pointer to Node to be freed
// return:	0 if properly freed
int nodeFree(TreeNode* node) {
        free(node->name);
        node->name = NULL;
	free(node->children);
	node->children = NULL;
	free(node);
	node = NULL;
        return 0;
}

// reads the Makefile and parses targets, turning them to nodes
// passes them into an unsorted array of TreeNodes
// inputs:	idk
// return:	idk
TreeNode** getNodes() {
	// Returned array of TreeNodes
	TreeNode** graph = malloc(sizeof(TreeNode*)*MAX_NODES);
	// initialize graph to NULL
	for (int i = 0; i < MAX_NODES; i++) {
		graph[i] = NULL;
	}
	// ints and buffer
	int nodeIndex = 0;
	int lineNum = 0;
	char* targetBuff = calloc(BUFF_SIZE, sizeof(char));
	TreeNode* nodeCheck;

	// Pointer to the open file
	FILE *f = openFile();
	// first parseTargets call
	lineNum = parseTargets(targetBuff, f);
	// Loop thru the file
	while (nodeIndex < BUFF_SIZE && lineNum > 0) {
		// parseTargets finds the next line with a viable
		// target and copies it into the input buffer. Then
		// it returns the line number it found it on
		nodeCheck = nodeInit(targetBuff, lineNum);
		// checking for multiple targets with the same name
		if (find(nodeCheck->name, graph) != NULL) {
			fprintf(stderr, "Error: multiple targets with same name: %s\n", nodeCheck->name);
			exit(1);
		}
		graph[nodeIndex] = nodeCheck;
		nodeIndex++;
		if (nodeIndex < BUFF_SIZE && nodeIndex == MAX_NODES) {
			fprintf(stderr, "Error: Makefile too long: %i\n", nodeIndex);
			exit(1);
		}
		lineNum = parseTargets(targetBuff, f);
	}
	// Close the file
	closeFile(f);
	free(targetBuff);
        return graph;
}

// finds a node with a given name in the list
// inputs:      the nodes name and the graph to search
// return:      the TreeNode, or NULL if DNE
TreeNode* find(char* name, TreeNode** graph) {
        int index = 0;
        int cmp;
        // loop until you've hit the max or the end
        // nodes are filled into the graph L to R
        while (index < MAX_NODES && graph[index] != NULL) {
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

// function to add a child to the list
void parentChild(TreeNode* parent, TreeNode* child) {
	// set child's parent node
	child->parent = parent;
	// add child to parent's array
	parent->children[parent->numchild] = child;
	parent->numchild++;
	// no space for more children
	return;
}
//prints out the tree
void printTree(TreeNode** graph) {
	int i = 0;
	while (i < MAX_NODES && graph[i] != NULL) {
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


