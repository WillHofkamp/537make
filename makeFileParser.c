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
// void closeMakeFile(FILE* file) {
// 	lineNum = 0;
//     fclose(file);
//     return;
// }

//This method parses the line for a target, and then inputs it
//and saves it. The line number of the target is returned.
int parseMakeTargets(char* name, FILE* f){
	
	char* token;
	char* rest = NULL;
	char line[BUFFER];
  
	
	if(name == NULL){
		printf("Not a valid name");
		return 0;
	}	
	if (lineNum == 0) {
		lineNum = 1;
	}

	while(!feof(f)) {
		// read line
		int result = readFileLine(line, f);
		if (result == -1) {
			return 0;
		}
		else if (result == -2) {
			fprintf(stderr, "Line longer than buffer\n");
			exit(0);
		}

		// search for ":" and ignore lines starting with \n, \t, or #
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
			//skip line
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
char** parseTargetDependencies(int lineNumber){
	
	FILE* file = openMakeFile();

	// get to lineNum
	for(int d = 1; d < lineNumber; d++){
		while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
	}

	char* line = calloc(BUFFER, sizeof(char));

	// read line
	int result = readFileLine(line, file);

	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "Line longer than buffer\n");
		exit(0);
	}
	
	// EOF check
	if (feof(file)) {
		return NULL;
	}

	char** parsedStrings = malloc(sizeof(char*)*MAX_NUM_NODES
);	
	for (int n = 0; n < MAX_NUM_NODES
; n++) {
		parsedStrings[n] = malloc(BUFFER);
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

	// read line until string terminator is encountered
	while (lineIndex < BUFFER && lineIndex < (int)strlen(line) && listIndex < MAX_NUM_NODES) {
		if (line[lineIndex] != ' ') {
			parsedStrings[listIndex][depIndex] = line[lineIndex];
			depIndex++;
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
char** parseMakeCommandLine(int* lineNumber){
	
	FILE* file = openMakeFile();
	char line[BUFFER];

        //discards the newline char from each line
        for(int d = 1; d < *lineNumber; d++){
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
			(*lineNumber)++;
			return parseMakeCommandLine(lineNumber);
		}
		else {
			if(c == ' ') {
				fprintf(stderr, "Should start with a tab character \n");
				exit(1);
			} else {
				return NULL;
			}
			
		}
	}

	char** array = malloc(sizeof(char*)*CMD_SIZE);
	for (int n = 0; n < CMD_SIZE; n++) {
		array[n] = malloc(CMD_SIZE);
	}

	// read line
	int result = readFileLine(line, file);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "Line longer than buffer\n");
		exit(0);
	}


	// index of the Makefile line
	int lineIndex = 0;
	// index of the command line argument
	int listIndex = 0;
	// index of next character within command line argument
	int arggIndex = 0;

	// read into the array we return
	while (line[lineIndex] != '\0' && listIndex < MAX_NUM_NODES) {

                //if a non-space character found
				if (line[lineIndex] != ' ') {
                        array[listIndex][arggIndex] = line[lineIndex];
                        arggIndex++;
                        lineIndex++;
                }
                else {
                        // add a null terminator
                        array[listIndex][arggIndex] = '\0';
                        listIndex++;
                        arggIndex = 0;
			lineIndex++;
                }
        }
	// add a null pointer after last arg
	if (arggIndex != 0) {
		array[listIndex][arggIndex] = '\0';
		listIndex++;
	}
	free(array[listIndex]);
	array[listIndex] = NULL;

	lineNum = 0;
    fclose(file);
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
                //read the character
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
