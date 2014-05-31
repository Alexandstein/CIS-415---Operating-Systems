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
   
===How dup2 works===
dup2(Source fd, fd you want to have go to the same place);
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
#include <errno.h>
#include "LinkedList.h"
#include "redirect.h"
#include "modHash.h"

int main(int argc, char *argv[])
{
	ModHash* hash = ModHash_init(1000);
	int* moo = calloc(1, sizeof(int));
	*moo = 90099;
	ModHash_put(hash, 1009, moo);
	ModHash_free(hash, 0);
	printf("%d\n", errno);	
	return 0;
	////////
	int moop = open("nope", O_RDWR | O_CREAT, 0644);
	int boop = open("Noop", O_RDWR | O_CREAT, 0644);;
	dup2(STDOUT_FILENO, moop);		//Makes moop go to same place as standard out
	dup2(boop, STDOUT_FILENO);		//Makes stdout go to the file boop points to
	dup2(moop, STDOUT_FILENO);		//Saved out in moop, so recover stdout
	write(moop, "Hey\n", 4);
	printf("Moop: %d Boop: %d\n", moop, boop);
	return;
	/////
	LinkedList* dorp = LinkedList_init();
	char* command = "python";
	char* command1= "cat < testing";
	char* command2= "python --version > testing";
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
	////
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

