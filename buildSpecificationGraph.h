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
#include "buildSpecification.h"
#include "makeFileParser.h"

int connectNodes(GraphNode** graph);

GraphNode** createGraphOrder(GraphNode* root, GraphNode** graph);

void searchDepthFirst(GraphNode* node, GraphNode** order);

GraphNode* findGraphRoot(int argc, const char* argv[], GraphNode** graph);


#endif
