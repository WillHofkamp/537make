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

void setFileName(const char* name) {
	makeFileName = name;
} 

//This method opens the makefile
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
	
	char* token;
	char* rest = NULL;
	char line[BUFFER];
  
	
	if(targetString == NULL){
		return 0;
	}	
	if (lineNum == 0) {
		lineNum = 1;
	}

	while(!feof(file)) {
		// read in line
		int result = readFileLine(line, file, lineNum);
		if (result == -1) {
			return 0;
		}
		else if (result == -2) {
			fprintf(stderr, "Line longer than buffer\n");
			exit(0);
		}

		// ignore lines that start with \n, \t, or #
		//read til you encounter a colon character
		if (line[0] != '\0' && line[0] != '\t' && line[0] != '#') {	
			token = strtok_r(line, ":", &rest);
			if (token == NULL){
				fprintf(stderr, "%i: Error: Target is incorrect \"%s\"\n", lineNum, line);
				exit(1);
			} else if (foundColon == false){
    			fprintf(stderr, "%i: Error: Colon is missing \"%s\"\n",lineNum, line);
    			exit(1);
    		} else{
				strcpy(targetString,token);
				// remove any extra whitespace
				// whitespace handling
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

	//throw out lines until you get to lineNum
	for(int d = 1; d < targetLineNum; d++){
		while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
	}

	char* line = calloc(BUFFER, sizeof(char));

	// read into line
	int result = readFileLine(line, file, targetLineNum);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", targetLineNum, line);
		exit(0);
	}
	
	// EOF check
	if (feof(file)) {
		return NULL;
	}

	char** dList = malloc(sizeof(char*)*MAX_NUM_NODES);	
	for (int n = 0; n < MAX_NUM_NODES; n++) {
		dList[n] = malloc(BUFFER);
    }

	// index of line read from Makefile
	int lineIndex = 0;
	// index of dependant number in parsedStrings
	int listIndex = 0;
	// index of dependant string in parsedStrings
	int depIndex = 0;

	// read until ":" is found
	while (lineIndex < BUFFER && line[lineIndex] != ':') {
		lineIndex++;
	}
	lineIndex++;
	while (lineIndex < BUFFER && line[lineIndex] == ' ') {
		lineIndex++;
	}

	// read char by char until you get to a string terminator
	// the remainder of the line is a series of
	// 	- chars making up the dependants
	// 	- followed by some non-zero number of spaces
	// 	- all ending in a terminator
	while (lineIndex < BUFFER && line[lineIndex] != '\0' && listIndex < MAX_NUM_NODES) {
		// if you find a non-space, non-terminating char...
		if (lineIndex < BUFFER && line[lineIndex] != ' ') {
			// ...set char in dList and increment
			dList[listIndex][deppIndex] = line[lineIndex];
			deppIndex++;
			// ...look at next char
			lineIndex++;
		}
		// if a space char is found
		else {
			// append a null terminator
			parsedStrings[listIndex][depIndex] = '\0';
			listIndex++;
			depIndex = 0;
			while (lineIndex < BUFFER && line[lineIndex] == ' ') {
				lineIndex++;
			}
		}
	}
	// if "\0:" found
	if (depIndex != 0) {
		parsedStrings[listIndex][depIndex] = '\0';
		listIndex++;
	}
	free(parsedStrings[listIndex]);
	parsedStrings[listIndex] = NULL;
	
	lineNum = 0;
    fclose(file);
	free(line);
	return parsedStrings;
}

//This method parses the command line of the makefile
//And returns an array of strings or null if the line is over
char** parseMakeCommandLine(int* cmdLineNum){
	
	FILE* file = openMakeFile();
	char line[BUFFER];

    //read each lineNum and throws out the newline
    for(int d = 1; d < *cmdLineNum; d++){
        while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
    }

	char c = fgetc(file);
	// quick EOF check
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
	char** cmdArray = malloc(sizeof(char*)*MAX_CMD_SIZE);
	for (int n = 0; n < MAX_CMD_SIZE; n++) {
		cmdArray[n] = malloc(MAX_CMD_SIZE);
	}

	// read in line
	int result = readFileLine(line, file, (int)cmdLineNum);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", *cmdLineNum, line);
		exit(0);
	}

	// index of the line from Makefile
	int lineIndex = 0;
	// index of the command line argument
	int listIndex = 0;
	// index of next character within command line argument
	int arggIndex = 0;

	// read into the array we return
	while (line[lineIndex] != '\0' && listIndex < MAX_NUM_NODES) {
                // if you find a non-space char...
                if (line[lineIndex] != ' ') {
                        // ...set char in array and increment
                        cmdArray[listIndex][arggIndex] = line[lineIndex];
                        arggIndex++;
                        lineIndex++;
                }
                else {
                        // append a null terminator
                        cmdArray[listIndex][arggIndex] = '\0';
                        // ...look at next char and next argument
                        listIndex++;
                        arggIndex = 0;
			lineIndex++;
                }
        }
	// add a null pointer after last arg
	if (arggIndex != 0) {
		cmdArray[listIndex][arggIndex] = '\0';
		listIndex++;
	}
	free(cmdArray[listIndex]);
	cmdArray[listIndex] = NULL;

	closeMakeFile(file);
	return cmdArray;
}

//This method reads a line from the file and formats it correctly and checks if there are
//any null characters in the line, not counting the newline terminator
int readFileLine(char* cmdString, FILE* file, int lineNum) {
	int i = 0;
	char c;
  	int cont = 1;
    while (cont) {
        c = fgetc(file);
        if (c == ':'){
            foundColon = true;
        }
        if (c == '\n') {
            cmdString[i] = '\0';
            cont = 0;
        }
        else if (feof(file)) {
            if (i == 0) {
                return -1;
            }
            cmdString[i] = '\0';
            cont = 0;
        }
        else if (i == BUFFER) {
            return -2;
		}
        // no stop condition, read the character!
        if (cont) {
			if (c == '\0') {
				fprintf(stderr, "%i: Error: the target line has a null character between dependencies \n", lineNum);
				exit(1);
			} else {
				cmdString[i] = c;
			}
        }
       	i++;
    }
	return 0;
}
