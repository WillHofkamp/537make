/////////////////////////////////////////////////
//
// File:	build_spec_repr.h
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
