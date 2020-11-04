////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: queue.c
// This File Description: This is the queue implementation which can create, as
//						  well as enqueue and dequeue to, a queue whose stats 
//						  can be printed out
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////
#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "text_parsing.h"

// Declaring and Defining the TreeNode struct
typedef struct TreeNode TreeNode;
struct TreeNode {
        char *name;
        int line;
        TreeNode *parent;
        TreeNode **children;
	int numchild;
        int checked;
	int recur;
};

// Declaring functions
TreeNode* nodeInit(char* name, int line);
int nodeFree(TreeNode* node);
TreeNode** getNodes();
void getModTime(TreeNode* node);
TreeNode* find(char* name, TreeNode** graph);
void parentChild(TreeNode* parent, TreeNode* child);
void printTree(TreeNode** graph);

#endif
