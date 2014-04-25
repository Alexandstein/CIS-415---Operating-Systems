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

//Handler
void handler(int signum){
	perror("OH NOOOOO\n");
	exit(0);
}

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

int main(int argc, char *argv[])
{
	char* thing = "+909090";
	printf("Thing is %d\n", toInt(thing));	
/*
	//Alarm/Signal example
	signal(SIGALRM, handler);
	alarm(1);
	while(1);
	//execve example
	execve("hello\n", NULL, NULL);
	//Write example
	const void* buf = "123456789\n";
	write(STDOUT_FILENO,buf,1);
	//Fork example
	int status;
	pid_t pid = fork();	//Forks at this point. Child never has pid set, differentiate like this.
	if(pid == 0){
		printf("I'm a child!\n");
	}else{
		waitpid(pid, &status, 0);
		printf("I'm a parent!\n");
	}
		printf("%d\n",len(buf));
*/
}
