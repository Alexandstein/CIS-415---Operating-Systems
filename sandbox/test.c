/*
TODO
	:Use waitpid in the main shell to selectively choose with groups or children to wait on
	:	negative nums for groups
	:For Background procs, make it so wait() doesn't happen


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
//#include <bits/sigaction.h>
#define _XOPEN_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#include "tokenizer.h"
#include "utils.h"
#include "LinkedList.h"
#include "redirect.h"
#include "modHash.h"
#include "process.h"
#include "errCheck.h"
#include "handlers.h"

//Declare the process table
ModHash* table;

//
int loop;

static void handleChild(int sig, siginfo_t* siginfo)
{
	Process* p;
	int status;
	printf("Child term. detected\n");
	wait(&status);
	pid_t pid = siginfo->si_pid;
	p = (Process*)ModHash_get(table, pid);
	printf("Completed Child %d: %s\n", p->pid, p->name);
	printf("%d's execution group is %d\n", pid, p->group);
	perror(0);
		
	//Iterate through table to see if any are running
/*	for(child = 0; child < table->size; child++){
		if((p = ((Process*)ModHash_get(table, child))) != 0){	//Check if process exists
			if(waitpid(-1, &status, WNOHANG) == -1){				//Check if running
																//If not, alert terminal
				printf("Completed Child %d: %s\n", p->pid, p->name); 	
			}else{
				printf("Child %d: %s still running\n", p->pid, p->name); 	
			}
		}
	}
	printf("Child Termination\n"); */
}

static void handleStop(int sig, siginfo_t* siginfo)
{
	printf("STOP >:( \n");
}

static void handleTerm(int sig, siginfo_t* siginfo)
{
	printf("SPLODE X_X\n");
	loop = 0;
}

int main(int argc, char *argv[])
{
	table = ModHash_init(64);
	
	char* command = "python";
	char* command1= "cat < testing";
	char* command2= "python --version | cat > boop";
	char** args = toExecArgs(command2);
	char** args2 = toExecArgs(command1);
	int stat = 1;	
	
	printf("Redirect?: %d\n", isProperRedirection(args2));
	executePipe(args, 1, 1);
//	LinkedList* tester = getRedirectionQueue(args);
	LinkedList* tester = getPipeQueue(args);
	char** print;
//	printf("\tThing: %s\n", tester->head->data);
//	return 0;
	while((print = LinkedList_next(tester)) != NULL){
		printf("Next queue entry\n");
		for(int i = 0; print[i] != NULL; i++){
			printf("\tThing: %s\n", print[i]);
		}
	}
		
	return 0;
	/*
	
	struct sigaction sa;
	sa.sa_handler = handleChild;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &sa, NULL);
	
	struct sigaction sa_Z;
	sa.sa_handler = handleStop;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGTSTP, &sa, NULL);
	
	struct sigaction sa_C;
	sa.sa_handler = handleTerm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &sa, NULL);
	
	pid_t child = 1;
	int pid = fork();
	Process* proc = Process_init("sleep1", pid, 1, 1);
	ModHash_put(table, proc->pid, proc);
	if(!pid){
		sleep(1);
//		printf("unsleep 1\n");
		exit(0);
	}else{
		int newpid = fork();
		Process* proc2 = Process_init("sleep3", newpid, 1, 1);
		ModHash_put(table, proc2->pid, proc2);
		if(!newpid){
			sleep(3);
//			printf("unsleep 3\n");
			exit(0);
		}
//		printf("Proc %d started\n", pid);
		Process* p;
		int status;
		loop = 1;
		while(loop){			//Loop infinitely to allow detection of child termination
//			printf("Waiting...\n");
		}
	}
	return 0;
	////////
	/*
	
	//ERROR EXAMPLE
	int testFile = open("fakefile.txt", O_RDWR, 0644);
	if(fileNotFoundErr()){
		printf("%d: Ya dun goof'd\n", errno);	
		perror(0);
	}
	
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
	char** print;
	
	LinkedList_queue(dorp, command);
	LinkedList_queue(dorp, command1);
	LinkedList_free(dorp);
	
//	print = (char*)LinkedList_dequeue(dorp);
		
	args = toExecArgs(command1);
	
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
	}
*/
}