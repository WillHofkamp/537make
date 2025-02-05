////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: makeFileParser.h
// This File Description: This is the interface of the file parses each part of 
//                        the makefile including the targets, dependencies, and 
//                        command line
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#ifndef MAKE_FILE_PARSER
#define MAKE_FILE_PARSER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER 4096

static const int MAX_NODE_LIST_SIZE = 64;
static const int MAX_COMMAND_LIST_SIZE = 64;

void setFileName(const char* name);

FILE* openMakeFile();

int parseMakeTargets(char* targetString, FILE* file);

char** parseTargetDependencies(int lineNum);

char** parseMakeCommandLine(int* lineNum);

int readFileLine(char* cmdString, FILE* file, int lineNum);

#endif
