////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: makeFileParser.h
// This File Description:
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef PARSER
#define PARSER

#include "buildSpecificationGraph.h"

typedef struct{
	char *newFilename;
	char **processToBuild;
	int numProcessesToBuild;
}CommandLineObject;

CommandLineObject* parseCommandLine(int argc, char* argv[], int BUFFER_SIZE);

SpecificationGraph* parseMakefile(int BUFFER_SIZE, char *newFilename);

char** splitUpCommands(char** commands, int numOfCommands, int BUFFER_SIZE);

#endif
