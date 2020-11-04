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

#include <stdio.h>
#include <unistd.h>
#include "build_spec_graph.h"
#include "proc_creation_prog_exe.h"

void testParseCommandLine();
void testGetNodes();
void freeGraph(TreeNode **graph);

int main(int argc, const char* argv[]) {
	TreeNode** graph = getNodes();
	TreeNode* root = getRoot(argc, argv, graph);
	connectNodes(graph);
	TreeNode** order = buildOrder(root, graph);
	execLoop(order);
	freeGraph(graph);
	free(order);
}

void freeGraph(TreeNode **graph) {
	int index = 0;
	while (graph[index] != NULL) {
		TreeNode *node = graph[index];
		nodeFree(node);
		index++;
	}
	free(graph);
}

