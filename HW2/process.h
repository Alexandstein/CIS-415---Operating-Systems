#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "modHash.h"

/*
Process structure to store in hash table to keep track of processes
*/
typedef struct Process{
	int pid;
	int background;
	int group;
	int status;
	char* name;
} Process;

/*
Constructor
Args
	char* name
		The name of the process
	int pid
		The process id
	int group
		The job group
	int background
		If set, the process is running in the background
*/
Process* Process_init(char* name, int pid, int group, int background);

/*
	Destructor
*/
void Process_free(Process* proc);

/////Supporting functions peraining to processes

/*
Description
	Checks to see whether there is an & in the command which tells the shell to execute
	process in the background.
Args
	char** args
		Array of args
*/
int isBackgroundCommand(char** args);

/*
Description
	Checks how many proccess are still on the table.
Args:
	ModHash* table
		Table storing the process data
Return:		
	Int containing number of processes
*/
int numProcesses(ModHash* table);
#endif