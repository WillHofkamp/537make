////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecification.h
// This File Description:
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef SPECIFICATION
#define SPECIFICATION

int buildSpecification(char **commands, int numCommands, int bufferSize);

int commandLineSplit(char* currentCommand, char** commandsParsed, int bufferSize);

#endif
