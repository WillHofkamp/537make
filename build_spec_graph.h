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
