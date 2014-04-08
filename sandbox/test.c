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

int main(int argc, char *argv[])
{	
	int* horp = malloc(sizeof(int) * 6); //Memleak
	int dorp = 5;
	
	printf("Allo\n");
	printf("Halllo\n");
}
