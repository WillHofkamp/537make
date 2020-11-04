/////////////////////////////////////////////////
//
// File:        text_parsing.c
//
// Author:      Abhi Kosuri
//              Mike Tuchler
// All files:   build_spec_graph.c, .h
//              build_spec_repr.c, .h
//              proc_creation_prog_exe.c, .h
//              text_parsing.c, .h
//              main.c, README
//
/////////////////////////////////////////////////


#include "text_parsing.h"

int lineNum = 0;

//opens the file and error handles
FILE* openFile() {
	FILE* file  = fopen("makefile", "r");

	if(file == NULL) {
		file = fopen("Makefile", "r");
		if (file == NULL) {
                	fprintf(stderr, "Error opening file\n");
                	exit(1);
		}
	}

	return file;
}

//method to close the file once it has been openend
void closeFile(FILE* file) {
	lineNum = 0;
        fclose(file);
        return;
}

//function to parse the target of each line
//inputs buffer to an empty array and pointer to an open file
//returns line number 
int parseTargets(char* name, FILE* file){
	
	char* token;
	char* rest = NULL;
	char line[BUFF_SIZE];
	
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
		int result = readLine(line, file);
		if (result == -1) {
			return 0;
		}
		else if (result == -2) {
			fprintf(stderr, "%i: Line too long\n", lineNum);
			exit(0);
		}

		// ignore lines that start with \n, \t, or #
		//read til you encounter a colon character
		if (line[0] != '\0' && line[0] != '\t' && line[0] != '#' && line[0] != ' ') {	
			token = strtok_r(line, ":", &rest);
			if (token == NULL){
				fprintf(stderr, "%i: Incorrect target \"%s\"\n", lineNum, line);
				exit(1);
			}
			else{
				strcpy(name,token);
				// whitespace handling
				int j = 0;
				while(j < BUFF_SIZE) {
					if (name[j] != ' ' && name[j] == '\0') {
						lineNum++;
					} else if (name[j] == '\n') {
						break;
					} else if (name[j] == '\0') {
						fprintf(stderr, "%i: Null character in target line \"%s\"\n", lineNum, line);
						exit(1);
					}
					j++;
				}
				return lineNum;
			}
		}
		else {
			//line starts with a \n or \t or #: ignore it
		}
		lineNum++;
	}		
	return 0;
}

//function to handle the dependencies as they could be the
//name of another target or name of another file 
//takes reads each line in from the line buffer checking for
//dependencies.
char** parseDependencies(int lineNumba){
	
	FILE* file = openFile();

	//throw out lines until you get to lineNum
	for(int d = 1; d < lineNumba; d++){
		while(fgetc(file) != '\n') {
			if (feof(file)) {
				return NULL;
			}
		}
	}

	char* line = calloc(BUFF_SIZE, sizeof(char));
	//char* line = malloc(BUFF_SIZE);

	// read into line
	int result = readLine(line, file);

	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Line too long\n", lineNumba);
		exit(0);
	}
	
	// EOF check
	if (feof(file)) {
		return NULL;
	}

	char** dList = malloc(sizeof(char*)*MAX_NODES);	
	for (int n = 0; n < MAX_NODES; n++) {
		dList[n] = malloc(BUFF_SIZE);
        }

	// index of line read from Makefile
	int lineIndex = 0;
	// index of dependant number in dList (first array index)
	int listIndex = 0;
	// index of dependant string in dList (second array index)
	int deppIndex = 0;

	// read until the colon to get the first dependancy
	while (lineIndex < BUFF_SIZE && line[lineIndex] != ':') {
		lineIndex++;
	}
	lineIndex++;
	// throw out all subsequent spaces
	while (lineIndex < BUFF_SIZE && line[lineIndex] == ' ') {
		lineIndex++;
	}

	// read char by char until you get to a string terminator
	// the remainder of the line is a series of
	// 	- chars making up the dependants
	// 	- followed by some non-zero number of spaces
	// 	- all ending in a terminator
	while (lineIndex < BUFF_SIZE && line[lineIndex] != '\0' && listIndex < MAX_NODES) {
		// if you find a non-space, non-terminating char...
		if (lineIndex < BUFF_SIZE && line[lineIndex] != ' ') {
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
			while (lineIndex < BUFF_SIZE && line[lineIndex] == ' ') {
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
	
	closeFile(file);
	free(line);
	return dList;
}

//function parses the command line
//reads up to the line if starts with a tab character
//return an array of strings that are passed into execvp() 
//returns NULL on end of cmd lines or EOF
char** parseCommandLine(int* lineNumba){
	
	FILE* file = openFile();
	char line[BUFF_SIZE];

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
			return parseCommandLine(lineNumba);
		}
		else {
			return NULL;
		}
	}

	// initialize/malloc array and some variables
	char** array = malloc(sizeof(char*)*CMD_SIZE);
	for (int n = 0; n < CMD_SIZE; n++) {
		array[n] = malloc(CMD_SIZE);
	}

	// read in line
	int result = readLine(line, file);
	if (result != 0) {
		return NULL;
	}
	else if (result == -2) {
		fprintf(stderr, "%i: Line too long\n", *lineNumba);
		exit(0);
	}


	// index of the line from Makefile
	int lineIndex = 0;
	// index of the command line argument
	int listIndex = 0;
	// index of next character within command line argument
	int arggIndex = 0;

	// read into the array we return
	while (line[lineIndex] != '\0' && listIndex < MAX_NODES) {
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

	closeFile(file);
	return array;
}

int readLine(char* buff, FILE* file) {
	int i = 0;
	char c;
        int cont = 1;
        while (cont) {
                c = fgetc(file);
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
                else if (i == BUFF_SIZE) {
                        return -2;
                }
                // no stop condition, read the character!
                if (cont) {
                        buff[i] = c;
                }
                i++;
        }
	return 0;
}
