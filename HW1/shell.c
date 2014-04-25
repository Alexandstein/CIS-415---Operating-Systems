#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

//Some constants
static const void* PROMPT_MESSAGE = "Enter a thing to execute!\n";
static const void* PROMPT = "$>> ";
static const int INPUT_SIZE = 2048;
static const int TRUNCATED_INPUT = 1024;
static const void* ERROR = "OH NOES! A thing went wrong. :CCC\n";
static const void* ALARM_ERROR = "Took too long: Dead'd >:( \n";
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

/*
int toDigit()
	Description:
		Takes a character and returns a number from 0-9 on success. -1 if char is not a
		digit.
	args:
		char inputChar
	Return:
		-1 on failure. 0-9 on success.
*/
int toDigit(char inputChar){
	if((inputChar >= 48 && inputChar <= 57)){
		return (int)inputChar - 48;
	}else{
		return -1;
	}
}

/*
int toInt()
	Description:
		Takes in string and tried to parse into an int. On failure 0 is returned.
		If there are space characters (ASCII 0,10,32) in the input string the 
		parsing is truncated at the space character.
	args: 
		char* input
			String to be converted.
	Return
		Int representation of string. Or 0 on failure.
*/
int toInt(char* input){
	int outputNum = 0;
	int inputLength = len(input);
	
	int sign = 1;
	//Check sign
	if(input[0] == 45){
		sign = -1;
	}else if(input[0] == 43 || toDigit(input[0]) >= 0){
		sign = 1;
		if(toDigit(input[0]) > -1){
			outputNum += toDigit(input[0]);	//Initialize if digit
		}
	}else{
		//Invalid character. Return 0
		return 0;
	}
	//Begin parsing loop at second character since first was sign determination
	//Newline and space characters cause read terminations.
	for(int i = 1; i < inputLength && input[i] != 32 && input[i] != 10; i++){
		outputNum *= 10;				//Move over one digit
		if(toDigit(input[i]) >= 0){
			outputNum += toDigit(input[i]);
		}else{
			return 0;
		}
	}
	return outputNum * sign;
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

//Handler
void handler(int signum){
	write(STDERR_FILENO, ALARM_ERROR, len(ALARM_ERROR));
	kill(childProcess, SIGKILL);
}

int main(int argc, char *argv[])
{	
	//If more than one element in argc, get that last element and try to use it
	//to set the timer length
	int alarmTimeout = 0;
	if(argc > 1){
		alarmTimeout = toInt(argv[argc - 1]);
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
		//Read in and truncate at 1024 using a null terminator.
		char* inputbuffer = calloc(INPUT_SIZE + 1, 1);
		inputbuffer[TRUNCATED_INPUT+1] = '\0';
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
