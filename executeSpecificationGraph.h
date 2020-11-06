////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: executeSpecificationGraph.h
// This File Description: This is the interface for the class that executes the
//                        the processes in the specification graph
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#ifndef FILL_GRAPH
#define FILL_GRAPH

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include "buildSpecificationGraph.h"

void checkNodes(GraphNode** flow);

void executeNodeProcess(GraphNode* node);

int modificationCheck(GraphNode* node);

#endif
