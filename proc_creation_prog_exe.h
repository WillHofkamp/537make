/////////////////////////////////////////////////
//
// File:	prog_creation_proc_exe.h
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
