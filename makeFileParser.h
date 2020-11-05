////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: makeFileParser.h
// This File Description: This file parses each part of the makefile including the
//						  targets, dependencies, and command line
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

#define BUFFER 4096

static const int MAX_NUM_NODES = 64;
static const int CMD_SIZE = 64;

FILE* openMakeFile();

void closeMakeFile(FILE* f);

int parseMakeTargets(char* target, FILE* f);

char** parseTargetDependencies(int lineNum);

char** parseMakeCommandLine(int* lineNum);

int readFileLine(char* buff, FILE* f);

void skipWhitespace(char* str);

#endif
