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

#ifndef PROG_CREATION_PROC_EXE_H
#define PROG_CREATION_PROC_EXE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "build_spec_graph.h"

void execLoop(TreeNode** order);
void execNode(TreeNode* node);
int timeCheck(TreeNode* node);

#endif
