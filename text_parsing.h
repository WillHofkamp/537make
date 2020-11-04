/////////////////////////////////////////////////
//
// File:	text_parsing.h
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

#ifndef TEXT_PARSING_H
#define TEXT_PARSING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFF_SIZE 1024

static const int MAX_NODES = 64;
static const int CMD_SIZE = 64;

FILE* openFile();
void closeFile(FILE* f);
int parseTargets(char* target, FILE* file);
char** parseDependencies(int lineNum);
char** parseCommandLine(int* lineNum);
int readLine(char* buff, FILE* file);

#endif
