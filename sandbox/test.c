/*
===GDB NOTES===
Compile with gcc -g <file.c>
Run file with gdb <program file>

Commands:
	break: 	Set breakpoints by using `break <linenumber>`
	print: 	Print variables with `print <variable name>`
	run: 	runs file
	n:    	next line in main. Runs and skips over functions
	step: 	Run line by line, steps through functions as well.
	
===Valgrind notes===
To look for memory leaks...
   valgrind --tool=memcheck --leak-check=yes ./a.out
*/
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "tokenizer.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "LinkedList.h"

int main(int argc, char *argv[])
{
	LinkedList* dorp = LinkedList_init();
	char* command = "python";
	char* command1= "cat < dorp";
	char* command2= "python --version > dorp";
	char** print;
	
	LinkedList_queue(dorp, command);
	LinkedList_queue(dorp, command1);
	LinkedList_free(dorp);
	
//	print = (char*)LinkedList_dequeue(dorp);
		
	char** args = toExecArgs(command1);
	
	printf("Redirect?: %d\n", isProperRedirection(args));
	LinkedList* tester = getRedirectionQueue(args);
	
	while((print = LinkedList_next(tester)) != NULL){
		printf("Next queue entry\n");
		for(int i = 0; print[i] != NULL; i++){
			printf("\tThing: %s\n", print[i]);
		}
	}
	
	executeRedirection(args);
	return;
	/////
	int status = 0;
	int newOut = open("./newOut.txt", O_RDWR | O_CREAT, 0644);
	int pid = fork();
	
	if(!pid){
		int out = dup(STDOUT_FILENO);
		dup2(newOut, STDOUT_FILENO);
		printf("print \"mkmkmk\"\n");
		fflush(NULL);
		dup2(out, STDOUT_FILENO);
		exit(0);
	}else{
		wait(&status);
		dup2(newOut, STDIN_FILENO);	
		execvp(args[0], args);
	}/*
*/
}

