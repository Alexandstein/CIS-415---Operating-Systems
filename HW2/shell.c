#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "LinkedList.h"
#include "redirect.h"
#include "tokenizer.h"
#include "utils.h"
#include "modHash.h"

//Some constants
static const void* PROMPT_MESSAGE = "Enter a command.\n";
static const void* PROMPT = "$>> ";
static const int INPUT_SIZE = 2048;
static const int TRUNCATED_INPUT = 1024;
static const void* ERROR = "OH NOES! A thing went wrong. :CCC\n";

//Global variable in order to keep track of child
int childProcess = 0;
ModHash* procTable;

//Execution wrapper

int main(int argc, char *argv[])
{
	procTable = ModHash_init(1024);
	int status;
	
	write(STDOUT_FILENO, PROMPT_MESSAGE, len(PROMPT_MESSAGE));
	while(1){
		//Read in and truncate at 1024 using a null terminator.
		char* inputbuffer = calloc(INPUT_SIZE + 1, 1);
		inputbuffer[TRUNCATED_INPUT+1] = '\0';
		//Prompt user for things.
		write(STDOUT_FILENO, PROMPT, len(PROMPT));
		//Read in and sanitize input
		read(STDIN_FILENO, inputbuffer, INPUT_SIZE);
		sanitize(inputbuffer);
		
		//Parge into arguments array
		char** arguments = toExecArgs(inputbuffer);

		int pid = fork();
		if(!pid){
		//Child: Execute.
			if(countRedirections(arguments)){		//There are redirections
				executeRedirection(arguments);
				exit(0);
			}else{
				execvp(arguments[0], arguments);
			}
			perror(inputbuffer);
			exit(0);
		}else if(pid > 0){
			//Parent: Wait for child.
			//Clear buffer.
			free(inputbuffer);			
		}else{
		//Error
			free(inputbuffer);
			perror(ERROR);
		}
	}
}
