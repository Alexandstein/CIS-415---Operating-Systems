#define _XOPEN_SOURCE
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#include "LinkedList.h"
#include "redirect.h"
#include "tokenizer.h"
#include "utils.h"
#include "modHash.h"
#include "process.h"
#include "errCheck.h"

//Some constants
static const void* PROMPT_MESSAGE = "Enter a command.\n";
static const void* PROMPT = "$kinda-sh>> ";
static const int INPUT_SIZE = 2048;
static const int TRUNCATED_INPUT = 1024;
static const int PROC_TABLE_SIZE = 64;
static const void* ERROR = "OH NOES! A thing went wrong. :CCC\n";

//Declare process table and critical section mask as global so handlers can access it.
ModHash* table;
sigset_t mask_CS;
//Declare printing queue
LinkedList* printQueue;

//Handlers
static void handleChild(int sig, siginfo_t* siginfo)
{
	sigprocmask(SIG_BLOCK,&mask_CS,0);				//CRITICAL SECTION: Proc Table
	Process* p;
	int status;
	wait(&status);
	pid_t pid = siginfo->si_pid;
	p = (Process*)ModHash_get(table, pid);
	printf("%s finished executing\n",p->name);
	ModHash_remove(table, pid, 0);
	free(p);
	sigprocmask(SIG_UNBLOCK,&mask_CS,0);			//CRITICAL SECTION: Exit
}

static void procDebugger(int sig, siginfo_t* siginfo)
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

int main(int argc, char *argv[])
{
	//Instantiate table and set up status variable
	table = ModHash_init(PROC_TABLE_SIZE);
	int status;
	//Initialize printQueue
	printQueue = LinkedList_init();
	
	//Set up signal masks
	sigemptyset(&mask_CS);
	sigaddset (&mask_CS, SIGTERM);
	sigaddset (&mask_CS, SIGCHLD);
	
	//Set up sigactions
	struct sigaction sa;
	sa.sa_handler = handleChild;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &sa, NULL);
	
	/************MAIN SHELL LOOP***************/
	write(STDOUT_FILENO, PROMPT_MESSAGE, len(PROMPT_MESSAGE));
	while(1){
		
		if(!LinkedList_isEmpty(printQueue) && LinkedList_isNext(printQueue)){
			char* toPrint;
			while((toPrint = LinkedList_next(printQueue)) != 0){
				printf("\t:%s\n", toPrint);
			}
		}
		//Read in and truncate at 1024 using a null terminator.
		char* inputbuffer = calloc(INPUT_SIZE + 1, sizeof(char));
		inputbuffer[TRUNCATED_INPUT+1] = '\0';
		//Prompt user for things.
		write(STDOUT_FILENO, PROMPT, len(PROMPT));
		//Read in and sanitize input
		read(STDIN_FILENO, inputbuffer, INPUT_SIZE);
		sanitize(inputbuffer);
		
		//Handle empty input by starting loop over.
		if(compareStrings(inputbuffer, "") || compareStrings(inputbuffer, "\n")){
			continue;
		}
		
		//Parge into arguments array
		char** arguments = toExecArgs(inputbuffer);
		int executeInBackground = isBackgroundCommand(arguments);
		
		if(executeInBackground){
			free(arguments[len2((const char**)arguments) - 1]);	//Free array holding "&"
			arguments[len2((const char**)arguments) - 1] = 0;	//Get rid of & after read
		}
		perror(0);
		int pid = fork();
		
		if(!pid){
		//Child: Execute.
			if(executeInBackground){				//If in bg, silence file descriptors
				int devnull = open("/dev/null", O_RDWR, 0644);
				dup2(devnull, STDIN_FILENO);
				dup2(devnull, STDOUT_FILENO);
				dup2(devnull, STDERR_FILENO);
			}
			if(isPipe(arguments)){
				executePipe(arguments ,executeInBackground, getpid());
			}else if(countRedirections(arguments)){		//There are redirections
				executeRedirection(arguments, executeInBackground);
				exit(0);
			}else{
				execvp(arguments[0], arguments);
			}
			perror(inputbuffer);
			exit(0);
		}else if(pid > 0){
			sigprocmask(SIG_BLOCK,&mask_CS,0);				//CRITICAL SECTION: Proc Table
			Process* newProcess = Process_init(arguments[0], pid, pid, executeInBackground); //TODO Problems with Process
			ModHash_put(table, newProcess->pid, newProcess);
			setpgid(newProcess->pid, newProcess->group);
			sigprocmask(SIG_UNBLOCK,&mask_CS,0);			//CRITICAL SECTION: Exit
			
			//TODO Parent: Wait for child if in foreground
			if(!executeInBackground){
				waitpid(-pid, &(status), 0);
			}
			//Clear buffer.
			free(inputbuffer);			
		}else{
		//Error
			free(inputbuffer);
			perror(ERROR);
		}
	}
}
