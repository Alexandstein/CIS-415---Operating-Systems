#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

//Some constants
static const void* PROMPT_MESSAGE = "Enter a thing to execute!\n";
static const void* PROMPT = "$>> ";
static const int INPUT_SIZE = 2048;
static const void* ERROR = "UH OH! A thing went wrong. :CCC\n";
static const void* ALARM_ERROR = "Too long: Deaded\n";
static const void* KILLSWITCH = "quit";
//Global variable in order to keep track of child
int childProcess = 0;

/*
int len()
	Description:
		Takes in character array and returns its length.
	args:
		char* input 
			The input to be measured
	return:
		The number of characters in the string. (Not including \0)	
*/
int len(const char* input){
	int i = 0;
	while(input[i] != '\0'){
		i++;
	}
	return i;
}

//Handler
void handler(int signum){
	write(STDERR_FILENO, ALARM_ERROR, len(ALARM_ERROR));
	kill(childProcess, SIGKILL);
}

/*
void sanitize()
	Description:
		Sanitizes input by editing string in-place. Strips whitespace at end.
	args:
		char* input
			Input to be sanitized
	return:
		(void)
*/
void sanitize(char* input){
	int i = 0;
	while(input[i] != '\n'){
		i++;
	}
	//Replace newline with terminator
	input[i] = '\0';
}

int main(int argc, char *argv[])
{	
	//If more than one element in argc, get that last element and try to use it
	//to set the timer length
	int alarmTimeout = 0;
	if(argc > 1){
		alarmTimeout = atoi(argv[argc - 1]);
	}
	//Value wasn't set possibly due to error or lack of arguement, defaults to 10 seconds
	if(alarmTimeout == 0){
		alarmTimeout = 10;
	}
	int status;
	//Set up alarm handler
	signal(SIGALRM, handler);
	
	write(STDOUT_FILENO, PROMPT_MESSAGE, len(PROMPT_MESSAGE));
	while(1){
		void* inputbuffer = calloc(INPUT_SIZE + 1, 1);
		//Prompt user for things.
		write(STDOUT_FILENO, PROMPT, len(PROMPT));
		//Read in and sanitize input
		read(STDIN_FILENO, inputbuffer, INPUT_SIZE);
		sanitize(inputbuffer);
				
		int pid = fork();
		if(!pid){
		//Child: Execute.
			execve(inputbuffer, NULL, NULL);
			perror(inputbuffer);
			exit(0);
		}else if(pid > 0){
		//Parent: Wait for child.
			//Clear buffer.
			free(inputbuffer);
			alarm(alarmTimeout);
			childProcess = pid;
			wait(&status);
			//Cancel alarm.
			alarm(0);
		}else{
		//Error
			free(inputbuffer);
			perror(ERROR);
		}
	}
}
