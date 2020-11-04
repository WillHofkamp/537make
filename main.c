/////////////////////////////////////////////////
//
// File:	main.c
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

