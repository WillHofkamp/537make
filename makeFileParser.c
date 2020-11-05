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
#include <stdbool.h>
#include <ctype.h>

int lineNum = 0;
bool foundColon = false;

//This method opens the makefile
FILE* openMakeFile() {
	FILE* file  = fopen("makefile", "r");

	if(file == NULL) {
		file = fopen("Makefile", "r");
		if (file == NULL) {
            fprintf(stderr, "Error: Could not open file\n");
            exit(1);
		}
	}

	return file;
}

//This method closes the make file
void closeMakeFile(FILE* file) {
	lineNum = 0;
        fclose(file);
        return;
}

//This method parses the line for a target, and then inputs it
//and saves it. The line number of the target is returned.
int parseMakeTargets(char* name, FILE* file){
	
	char* token;
	char* rest = NULL;
	char line[BUFFER];
  
	
	if(name == NULL){
		printf("Not a valid name");
		return 0;
	}	
	// only initialize lineNum if you're first to do it
	if (lineNum == 0) {
		lineNum = 1;
	}

	while(!feof(file)) {
		// read in line
		int result = readFileLine(line, file);
		if (result == -1) {
			return 0;
		}
		else if (result == -2) {
			fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", lineNum, line);
			exit(0);
		}

		// ignore lines that start with \n, \t, or #
		//read til you encounter a colon character
		if (line[0] != '\0' && line[0] != '\t' && line[0] != '#' && line[0] != ' ') {	
			token = strtok_r(line, ":", &rest);
			if (token == NULL){
				fprintf(stderr, "%i: Target is incorrect \"%s\"\n", lineNum, line);
				exit(1);
			} else if (foundColon == false){
    			fprintf(stderr, "%i:Error: Colon is missing \"%s\"\n",lineNum,line);
    			exit(1);
    		} else{
				memset(name, 0, strlen(name));
				strncpy(name,token, strlen(token));
				// remove any extra whitespace
				skipWhitespace(name);
				return lineNum++;
			}
		}
		else {
			//line starts with a \n or \t or #: ignore it
		}
		lineNum++;
	}		
	return 0;
}

//This method is used to trim any excess whitespace from the target
void skipWhitespace(char* str) {
  int i, x;
  for(i=x=0; str[i]; ++i) {
	  if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1]))) {
		str[x++] = str[i];
	}
  }
}

//This method parses the dependencies based on the line of the file
char** parseTargetDependencies(int lineNumba){
	
	FILE* file = openMakeFile();

	//throw out lines until you get to lineNum
	for(int d = 1; d < lineNumba; d++){
		while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
	}

	char* line = calloc(BUFFER, sizeof(char));

	// read into line
	int result = readFileLine(line, file);

	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", lineNumba, line);
		exit(0);
	}
	
	// EOF check
	if (feof(file)) {
		return NULL;
	}

	char** dList = malloc(sizeof(char*)*MAX_NUM_NODES
);	
	for (int n = 0; n < MAX_NUM_NODES
; n++) {
		dList[n] = malloc(BUFFER);
        }

	// index of line read from Makefile
	int lineIndex = 0;
	// index of dependant number in dList (first array index)
	int listIndex = 0;
	// index of dependant string in dList (second array index)
	int deppIndex = 0;

	// read until the colon to get the first dependancy
	while (lineIndex < BUFFER && line[lineIndex] != ':') {
		lineIndex++;
	}
	lineIndex++;
	// throw out all subsequent spaces
	while (lineIndex < BUFFER && line[lineIndex] == ' ') {
		lineIndex++;
	}

	// read char by char until you get to a string terminator
	// the remainder of the line is a series of
	// 	- chars making up the dependants
	// 	- followed by some non-zero number of spaces
	// 	- all ending in a terminator
	while (lineIndex < BUFFER && lineIndex < (int)strlen(line) && listIndex < MAX_NUM_NODES) {
		// if you find a non-space, non-terminating char...
		if (line[lineIndex] != ' ') {
			// ...set char in dList and increment
			dList[listIndex][deppIndex] = line[lineIndex];
			deppIndex++;
			// ...look at next char
			lineIndex++;
		}
		// if you find a space char...
		else {
			// append a null terminator
			dList[listIndex][deppIndex] = '\0';
			// ...look at next char and next dependant
			listIndex++;
			deppIndex = 0;
			// while loop ignores multiple consecutive spaces
			while (lineIndex < BUFFER && line[lineIndex] == ' ') {
				lineIndex++;
			}
		}
	}
	// stuff once you've found the \0:
	if (deppIndex != 0) {
		dList[listIndex][deppIndex] = '\0';
		listIndex++;
	}
	free(dList[listIndex]);
	dList[listIndex] = NULL;
	
	closeMakeFile(file);
	free(line);
	return dList;
}

//This method parses the command line of the makefile
//And returns an array of strings or null if the line is over
char** parseMakeCommandLine(int* lineNumba){
	
	FILE* file = openMakeFile();
	char line[BUFFER];

        //read each lineNum and throws out the newline
        for(int d = 1; d < *lineNumba; d++){
                while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
        }

	// check if viable command line
	char c = fgetc(file);
	// quick EOF check
	if (feof(file)) {
		return NULL;
	}
	if (c != '\t') {
		// Ignore a line that starts with a newline or #
		if (c == '\n' || c == '#') {
			(*lineNumba)++;
			return parseMakeCommandLine(lineNumba);
		}
		else {
			if(c == ' ') {
				fprintf(stderr, "%i: Error: should start with a tab character \n", *lineNumba);
				exit(1);
			} else {
				return NULL;
			}
			
		}
	}

	// initialize/malloc array and some variables
	char** array = malloc(sizeof(char*)*CMD_SIZE);
	for (int n = 0; n < CMD_SIZE; n++) {
		array[n] = malloc(CMD_SIZE);
	}

	// read in line
	int result = readFileLine(line, file);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Error: Line longer than buffer \"%s\"\n", *lineNumba, line);
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
                        array[listIndex][arggIndex] = line[lineIndex];
                        arggIndex++;
                        // ...look at next char
                        lineIndex++;
                }
                // if you find a space char...
                else {
                        // append a null terminator
                        array[listIndex][arggIndex] = '\0';
                        // ...look at next char and next argument
                        listIndex++;
                        arggIndex = 0;
			lineIndex++;
                }
        }
	// append a null pointer after last arg
	if (arggIndex != 0) {
		array[listIndex][arggIndex] = '\0';
		listIndex++;
	}
	free(array[listIndex]);
	array[listIndex] = NULL;

	closeMakeFile(file);
	return array;
}

//This method reads a line from the file and formats it correctly and checks if there are
//any null characters in the line, not counting the newline terminator
int readFileLine(char* buff, FILE* file) {
	int i = 0;
	char c;
  	int cont = 1;
        while (cont) {
                c = fgetc(file);
                
                if (c == ':'){
                  foundColon = true;
                }
                if (c == '\n') {
                        buff[i] = '\0';
                        cont = 0;
                }
                else if (feof(file)) {
                        if (i == 0) {
                                return -1;
                        }
                        buff[i] = '\0';
                        cont = 0;
                }
                else if (i == BUFFER) {
                        return -2;
                }
                // no stop condition, read the character!
        		if (cont) {
					if(i > 1 && buff[i-1] == ' ' && c == ' ') {
					} 
					else if (c == '\0') {
						fprintf(stderr, "%i: Error: the target line has a null character between dependencies. \n", i);
						exit(1);
					} else {
						buff[i] = c;
					}
        		}
       		i++;
        }
	return 0;
}
