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
