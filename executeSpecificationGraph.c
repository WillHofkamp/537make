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

int executed = 0;

//This method loops through each node and checks if they can be executed
void checkNodes(GraphNode** graph) {
	int execute = 1;
	int i = 0;
	while (graph[i] != NULL) {
		// check if node is a target
		if (graph[i]->line < 0) {
			execute = 0;
		}
		// check if node is a file
		if (access(graph[i]->name, F_OK) == 0) {
			if (!modificationCheck(graph[i])) {
				execute = 0;
			}
		}
		if (execute) {
			executeNodeProcess(graph[i]);
		}
		i++;
		execute = 1;
	}
	if (!executed) {
		fprintf(stderr, "All files up-to-date\n");
	}

	return;
}

//This method creates a child proces from the parent process and runs it,
//then moves on down the graph
void executeNodeProcess(GraphNode* node) {
	pid_t pid;
	int status;

	char** cmdList;
	int* currLine = &(node->line);
	(*currLine)++; 
	cmdList = parseMakeCommandLine(currLine);
	while(cmdList != NULL){
		pid = fork();

		if(pid < 0){
			fprintf(stderr, "Process could not be forked\n");
			exit(0);
		}
        else if(pid == 0){
			execvp(cmdList[0], cmdList);
			exit(EXIT_FAILURE);
		}
		else{
			wait(&status);
			if (WEXITSTATUS(status)) {
				// didn't exit normally
				fprintf(stderr,"Invalid command \n");
				int j = 0;
				while (cmdList[j] != NULL) {
					fprintf(stderr, "%s ", cmdList[j]);
					j++;
				}
				fprintf(stderr, "\"\n");
				exit(0);
			}
		}	
		//line executed
		executed = 1;
		(*currLine)++;

		//free memory
		for (int k = 0; k < MAX_COMMAND_LIST_SIZE; k++) {
			free(cmdList[k]);
		}
		free(cmdList);

		cmdList = parseMakeCommandLine(currLine);
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

	// loop throught the children
	for (int i = 0; i < node->numchild; i++) {
		struct stat attr;
		if (stat(node->children[i]->name, &attr) == 0)
		{
			t_child = attr.st_mtime;
		} else {
			t_child = 0;
		}
		// recompile parent  if child was modified later than parent
		if( difftime(t_node, t_child) <= 0 && t_child != 0) {
			return 1;
		}
	}
	return 0;
}
