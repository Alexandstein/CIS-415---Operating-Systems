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

int main(int argc, char *argv[])
{	
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
}
