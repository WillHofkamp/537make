////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecification.c
// This File Description:
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
//
/////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include "buildSpecification.h"

// This method take in the commands specific to a node
// and runs fork()/execvp() to build a program with 
// the proper specifications
//
// @param commands
// @return N/A
//
int buildSpecification(char **commands, int commandNum, int bufferSize) {

	pid_t processId;
	int pStatus;
	int execErrorFlag = 0;
	int numStringsInLine = 0;
	char** splitCommands = (char**) malloc(commandNum * bufferSize * sizeof(char*));
	if(splitCommands == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}

	for(int i = 0; i < commandNum; i++){
		if((processId = fork()) < 0){
			fprintf(stderr, "Error: forking new process failed\n");
			exit(-1);
		}else if(processId == 0){
			// overwrite the parent process with the new commands
			numStringsInLine = commandLineSplit(commands[i], splitCommands, bufferSize);
			if(numStringsInLine){
				execErrorFlag = execvp(splitCommands[0], splitCommands);
				// if child process fails, stop the build
				if(execErrorFlag){
					fprintf(stderr, "Error: Failure to execute because %s\n", strerror(errno));
					fprintf(stderr, "Error: Execvp failed.\n");
					exit(-1);
				}
			}
		}else{
			// parent process will wait until the child process has completed
			while(wait(&pStatus) != processId){}
		}
	}
	free(splitCommands);
	return 0;
}

int commandLineSplit(char* currentCommand, char** currentCommandsParsed, int bufferSize){
	int index = 0;
	char c = '\0';
	char *currString = NULL;
	int currStringIndex = 0;
	int currLineIndex = 0;
	int numOfStrings = 0;

	// for all of the currentCommands split them up and put them in a new 2d array for the execvp
	currLineIndex = 0;
	// cycle through the entire line
	while((c = currentCommand[currLineIndex]) != '\0'){
		if(currStringIndex == 0){
			currString = (char*) malloc(bufferSize * sizeof(char));
			if(currString == NULL){
				fprintf(stderr, "Error: Unable to malloc memory.\n");
				exit(-1);
			}
		}
		
		// if current char is a space than we reached the end of current string
		if(c == ' '){
			currString[currStringIndex] = '\0';
			*(currentCommandsParsed+index) = currString;
			numOfStrings++;
			index++;
			currStringIndex = 0;
			// store string in new split commands array
		}else{
			currString[currStringIndex] = c;
			currStringIndex++;
		}
		currLineIndex++;
	}

	// if the command isn't empty than add it to the array
	if(currStringIndex > 0){
		currString[currStringIndex] = '\0';
		*(currentCommandsParsed+index) = currString;
		numOfStrings++;
		index++;
	}
	
	*(currentCommandsParsed+index) = NULL;
	index++;

	return numOfStrings;
}

