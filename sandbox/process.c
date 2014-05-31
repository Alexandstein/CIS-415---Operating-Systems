#include "process.h"
#include "utils.h"

Process* Process_init(char* name, int pid, int group, int background){
	Process* output    = calloc(1,1);
	output->name	   = name;
	output->pid 	   = pid;
	output->group 	   = group;
	output->background = background;
}

int isBackgroundCommand(char** args){
	if(compareStrings(args[len2(args) - 1],"&")){
		return 1;
	}
	return 0;
}