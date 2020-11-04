/////////////////////////////////////////////////
//
// File:	build_spec_graph.h
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

#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "build_spec_repr.h"
#include "text_parsing.h"

int connectNodes(TreeNode** graph);
TreeNode** buildOrder(TreeNode* root, TreeNode** graph);
void DFS(TreeNode* node, TreeNode** order);
TreeNode* getRoot(int argc, const char* argv[], TreeNode** graph);

#endif
