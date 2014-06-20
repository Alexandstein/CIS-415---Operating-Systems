#include "process.h"
#include "utils.h"
#include "modHash.h"

Process* Process_init(char* name, int pid, int group, int background){
	Process* output    = calloc(1, sizeof(*output));
	output->name	   = name;
	output->pid 	   = pid;
	output->group 	   = group;
	output->background = background;
	output->status	   = 0;

	return output;
}

/*
	Destructor
*/
void Process_free(Process* proc){
	free(proc);
}

/*
Description
	Checks to see whether there is an & in the command which tells the shell to execute
	process in the background.
Args
	char** args
		Array of args
*/
int isBackgroundCommand(char** args){
	if(compareStrings(args[len2((const char**)args) - 1],"&")){
		return 1;
	}
	return 0;
}

/*
Description
	Checks how many proccess are still on the table.
Args:
	ModHash* table
		Table storing the process data
Return:		
	Int containing number of processes
*/
int numProcesses(ModHash* table){
	int count = 0;
	for(int i = 0; i < table->size; i++){
		if(ModHash_get(table, i)){
			count++;
		}
	}
	return count;
}