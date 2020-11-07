////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: makeFileParser.c
// This File Description: This file parses each part of the makefile including the
//						  targets, dependencies, and command line
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include "makeFileParser.h"

int lineNum = 0;
bool foundColon = false;
char* makeFileName;

//This method is a setter that sets the makefile name
void setFileName(const char* name) {
	makeFileName = name;
} 

//This method opens the makefile based off the cmd line arg
FILE* openMakeFile() {
	if(strcmp("makefile", makeFileName) == 0) {
		FILE* file  = fopen("makefile", "r");
		if(file == NULL) {
			file = fopen("Makefile", "r");
			if (file == NULL) {
            	fprintf(stderr, "Error: Could not open file\n");
            	exit(1);
			}
		}
		return file;
	} else {
		FILE* file  = fopen(makeFileName, "r");
		if (file == NULL) {
            fprintf(stderr, "Error: Could not open file\n");
            exit(1);
		}
		return file;
	}
}


//This method parses the line for a target, and then inputs it
//and saves it. The line number of the target is returned.
int parseMakeTargets(char* targetString, FILE* file){
	
	char* target;
	char* nullStr = NULL;
	char currLine[BUFFER];
  
	if(targetString == NULL){
		return 0;
	}	
	if (lineNum == 0) {
		lineNum = 1;
	}

	while(!feof(file)) {
		// read in line
		int result = readFileLine(currLine, file, lineNum);
		if (result == -1) {
			return 0;
		}
		else if (result == -2) {
			fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", lineNum, currLine);
			exit(0);
		}

		//read til you encounter a colon character
		if (currLine[0] != '\0' && currLine[0] != '\t' && currLine[0] != '#') {	
			target = strtok_r(currLine, ":", &nullStr);
			if (target == NULL){
				fprintf(stderr, "%i: Error: Target is incorrect \"%s\"\n", lineNum, currLine);
				exit(1);
			} else if (foundColon == false){
    			fprintf(stderr, "%i: Error: Colon is missing \"%s\"\n", lineNum, currLine);
    			exit(1);
    		} else{
				strcpy(targetString,target);
				// remove any extra whitespace
				for (int j = 0; j < BUFFER; j++) {
					if (targetString[j] == ' ') {
						targetString[j] = '\0';
						return lineNum++;
					}
				}
				return lineNum++;
			}
		}
		else {
			//skip line
		}
		lineNum++;
	}		
	return 0;
}

//This method parses the dependencies based on the line of the file
char** parseTargetDependencies(int targetLineNum){
	
	FILE* file = openMakeFile();

	//discard lines until you get to lineNum
	for(int i = 1; i < targetLineNum; i++){
		while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
	}

	char* currLine = calloc(BUFFER, sizeof(char));

	// read into line
	int result = readFileLine(currLine, file, targetLineNum);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", targetLineNum, currLine);
		exit(0);
	}
	
	if (feof(file)) {
		return NULL;
	}

	char** dependencyList = malloc(sizeof(char*)*MAX_NODE_LIST_SIZE);	
	for (int n = 0; n < MAX_NODE_LIST_SIZE; n++) {
		dependencyList[n] = malloc(BUFFER);
    }

	int makeFileLineIndex = 0;
	int dependencyListIndex = 0;
	int dependencyStringIndex = 0;

	//skip over until : is found
	while (makeFileLineIndex < BUFFER && currLine[makeFileLineIndex] != ':') {
		makeFileLineIndex++;
	}
	makeFileLineIndex++;
	//skip over excess whitespace
	while (makeFileLineIndex < BUFFER && currLine[makeFileLineIndex] == ' ') {
		makeFileLineIndex++;
	}

	//read until string terminator is reached
	while (makeFileLineIndex < BUFFER && currLine[makeFileLineIndex] != '\0' && dependencyListIndex < MAX_NODE_LIST_SIZE) {
		if (makeFileLineIndex < BUFFER && currLine[makeFileLineIndex] != ' ') {
			//add to dependency list if not space
			dependencyList[dependencyListIndex][dependencyStringIndex] = currLine[makeFileLineIndex];
			dependencyStringIndex++;
			makeFileLineIndex++;
		}
		else {
			//if there is a space, add dependency to list and start adding new dependency
			dependencyList[dependencyListIndex][dependencyStringIndex] = '\0';
			dependencyListIndex++;
			dependencyStringIndex = 0;
			while (makeFileLineIndex < BUFFER && currLine[makeFileLineIndex] == ' ') {
				makeFileLineIndex++;
			}
		}
	}
	//when the null terminator is found above
	if (dependencyStringIndex != 0) {
		dependencyList[dependencyListIndex][dependencyStringIndex] = '\0';
		dependencyListIndex++;
	}
	free(dependencyList[dependencyListIndex]);
	dependencyList[dependencyListIndex] = NULL;
	
	//free memory
	lineNum = 0;
    fclose(file);
	free(currLine);
	return dependencyList;
}

//This method parses the command line of the makefile
//And returns an array of strings or null if the line is over
char** parseMakeCommandLine(int* cmdLineNum){
	
	FILE* file = openMakeFile();
	char currLine[BUFFER];

    //read each lineNum and throws out the newline
    for(int i = 1; i < *cmdLineNum; i++){
        while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
    }

	char c = fgetc(file);
	if (feof(file)) {
		return NULL;
	}
	if (c != '\t') {
		// Skip lines starting with a newline or #
		if (c == '\n' || c == '#') {
			(*cmdLineNum)++;
			return parseMakeCommandLine(cmdLineNum);
		}
		else {
			if(c == ' ') {
				fprintf(stderr, "%i: Error: should start with a tab character \n", *cmdLineNum);
				exit(1);
			} else {
				return NULL;
			}
			
		}
	}

	//allocate memory for array of commands
	char** cmdArray = malloc(sizeof(char*)*MAX_COMMAND_LIST_SIZE);
	for (int j = 0; j < MAX_COMMAND_LIST_SIZE; j++) {
		cmdArray[j] = malloc(MAX_COMMAND_LIST_SIZE);
	}

	//read line and check its valid
	int result = readFileLine(currLine, file, (int)cmdLineNum);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", *cmdLineNum, currLine);
		exit(0);
	}

	int makeFileLineIndex = 0;
	int commandListIndex = 0;
	int commandStringIndex = 0;

	// read into the array we return
	while (currLine[makeFileLineIndex] != '\0' && commandListIndex < MAX_NODE_LIST_SIZE) {
        if (!isspace(currLine[makeFileLineIndex])) {
            cmdArray[commandListIndex][commandStringIndex] = currLine[makeFileLineIndex];
            commandStringIndex++;
            makeFileLineIndex++;
        }
        else {
			if(makeFileLineIndex > 0 && !isspace(currLine[makeFileLineIndex-1])) {
				cmdArray[commandListIndex][commandStringIndex] = '\0';
            	commandListIndex++;
            	commandStringIndex = 0;
				makeFileLineIndex++;
			} else {
				makeFileLineIndex++;
			}
			
        }
    }
	// add a null pointer after last arg
	if (commandStringIndex != 0) {
		cmdArray[commandListIndex][commandStringIndex] = '\0';
		commandListIndex++;
	}
	free(cmdArray[commandListIndex]);
	cmdArray[commandListIndex] = NULL;

	lineNum = 0;
	fclose(file);
	return cmdArray;
}

//This method reads a line from the file and formats it correctly and checks if there are
//any null characters in the line, not counting the newline terminator
int readFileLine(char* cmdString, FILE* file, int lineNum) {
	int i = 0;
	char currChar;
  	int endNotReached = 1;
    while (endNotReached) {
        currChar = fgetc(file);
        if (currChar == ':'){
            foundColon = true;
        }
        if (currChar == '\n') {
            cmdString[i] = '\0';
            endNotReached = 0;
        }
        else if (feof(file)) {
            if (i == 0) {
                return -1;
            }
            cmdString[i] = '\0';
            endNotReached = 0;
        }
        else if (i == BUFFER) {
            return -2;
		}
        if (endNotReached) {
			//since end was still not reached, add cmd char
			if (currChar == '\0') {
				fprintf(stderr, "%i: Error: the target line has a null character between dependencies \n", lineNum);
				exit(1);
			} else {
				cmdString[i] = currChar;
			}
        }
       	i++;
    }
	return 0;
}
