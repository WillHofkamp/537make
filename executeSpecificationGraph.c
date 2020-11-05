////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: executeSpecificationGraph.c
// This File Description: This method executes the graph in the order it was built
//
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////


#include "executeSpecificationGraph.h"
int wasexec = 0;

//This method loops through each node and checks if they can be executed
void checkNodes(GraphNode** order) {
	int execute = 1;
	int i = 0;
	// printGraph(order);
	while (order[i] != NULL) {
		// check if it's a target
		// if its line number = -1, don't exec
		if (order[i]->line < 0) {
			execute = 0;
		}
		// check if it's a file
		if (access(order[i]->name, F_OK) == 0) {
			// if so, check if it's up to date
			if (!modificationCheck(order[i])) {
				execute = 0;
			}
		}
		// great! you're good to go!
		if (execute) {
			executeNodeProcess(order[i]);
		}
		i++;
		execute = 1;
	}
	if (!wasexec) {
		fprintf(stderr, "All files up-to-date\n");
	}

	return;
}

//This method creates a child proces from the parent process and runs it,
//then moves on down the graph
void executeNodeProcess(GraphNode* node) {
	
	int* line = &(node->line);
	(*line)++;
	char** cmdList;
	pid_t pid;
	int status;
	 
	cmdList = parseMakeCommandLine(line);
	while(cmdList != NULL){
		pid = fork();

		if(pid < 0){
			fprintf(stderr, "Process could not be forked\n");
			exit(0);
		}
               	else if(pid == 0){
			// EXECUTE THE LINE
			execvp(cmdList[0], cmdList);
			exit(EXIT_FAILURE);
		}

		else{
			wait(&status);
			if (WEXITSTATUS(status)) {
				// didn't exit normally
				fprintf(stderr,"%i: Invalid command \"", *line);
				int y = 0;
				while (cmdList[y] != NULL) {
					fprintf(stderr, "%s ", cmdList[y]);
					y++;
				}
				fprintf(stderr, "\"\n");
				exit(0);
			}
			else {
				// manually print command
				int x = 0;
				while (cmdList[x] != NULL) {
					fprintf(stderr, "%s ", cmdList[x]);
				x++;
				}
				fprintf(stderr, "\n");
			}
		}	
		//done executing one line
		wasexec = 1;
		(*line)++;

		// free previous command list
		for (int f = 0; f < CMD_SIZE; f++) {
			// pay respects
			free(cmdList[f]);
		}
		free(cmdList);

		cmdList = parseMakeCommandLine(line);
	}

	return;
}

//This method finds the modified time of a file
static time_t getFileModifiedTime(const char *path)
{
	struct stat attr;
	if (stat(path, &attr) == 0)
	{
		return attr.st_mtime;
	}
	return 0;
}

//This method is used to check if the file has been modified
//and if it should be build
int modificationCheck(GraphNode* node) {
	time_t t_node = getFileModifiedTime(node->name);
	time_t t_child;

	// loop thru children
	for (int i = 0; i < node->numchild; i++) {
		//do thing
		t_child = getFileModifiedTime(node->children[i]->name);
		// if child was modified MORE SECONDS after the parent,
		// then parent needs to be recompiled.
		if( difftime(t_node, t_child) <= 0 && t_child != 0) {
			return 1;
		}
	}
	// no need to compile
	return 0;
}
