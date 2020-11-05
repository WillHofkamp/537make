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
void checkNodes(GraphNode** flow) {
	int execute = 1;
	int i = 0;
	while (flow[i] != NULL) {
		// check if node is a target
		if (flow[i]->line < 0) {
			execute = 0;
		}
		// check if node is a file
		if (access(flow[i]->name, F_OK) == 0) {
			if (!modificationCheck(flow[i])) {
				execute = 0;
			}
		}
		// execute node
		if (execute) {
			executeNodeProcess(flow[i]);
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
			execvp(cmdList[0], cmdList);
			exit(EXIT_FAILURE);
		}

		else{
			wait(&status);
			if (WEXITSTATUS(status)) {
				// didn't exit normally
				fprintf(stderr,"Invalid command \n");
				int i = 0;
				while (cmdList[i] != NULL) {
					fprintf(stderr, "%s ", cmdList[i]);
					i++;
				}
				fprintf(stderr, "\"\n");
				exit(0);
			}
			else {
				//print cmd
				int j = 0;
				while (cmdList[j] != NULL) {
					fprintf(stderr, "%s ", cmdList[j]);
				x++;
				}
				fprintf(stderr, "\n");
			}
		}	
		//line executed
		executed = 1;
		(*line)++;

		// free command list
		int k = 0;
		while(i<CMD_SIZE){
			free(cmdList[k]);
			i++;
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

	// loop throught the children
	for (int i = 0; i < node->numchild; i++) {
		t_child = getFileModifiedTime(node->children[i]->name);
		// recompile parent  if child was modified later than parent
		if( difftime(t_node, t_child) <= 0 && t_child != 0) {
			return 1;
		}
	}
	return 0;
}
