#include "utils.h"
#include "tokenizer.h"
#include "redirect.h"
#include "LinkedList.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

/////////////////REDIRECTION RELATED/////////////
/*
int isRedirection()
	Description:
		Tells whether or not a command uses redirections.
	args:
		char** args
			Array of tokens
	return:
		1 if redirections, 0 elsewise.
*/
int isRedirection(char** args){
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] == '>' || args[i][0] == '<'){
			return 1;
		}else{
			continue;
		}
	}
	return 0;	
}

/*
int isProperRedirection()
	Description:
		Tells whether or not a command is a functioning redirection.
	args:
		char** args
			Array of tokens
	return:
		1 if true, 0 if false.
*/
int isProperRedirection(char** args){
	int size = 0;//len(args);
	if(countRedirections(args) > 2){
		return 0;
	}
	if(args[0][0] == '<' || args[0][0] == '>')
//	   args[size - 1][0] == '<' || args[1 - size][0] == '>')
	{
		return 0;
	}
	return 1;
}

/*
int countRedirections()
	Description:
		Counts number of redirections.
	args:
		char** args
			Array of tokens
	return:
		Int containing number of redirects.
*/
int countRedirections(char** args){
	int count = 0;
	
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] == '>' || args[i][0] == '<'){
			count++;
		}else{
			continue;
		}
	}
	return count;
}

/*
LinkedList* getRedirectionQueue()
	Description:
		Creates a queue that can be iterated through using _next()
	args:
		char** args
			Array of tokens
	return:
		LinkedList reverse so that _next() iterates through in order of args.
		Args are grouped with redirects in the middle of them.
		Returns NULL if not a proper redirection.
*/
LinkedList* getRedirectionQueue(char** args){
	if(!isProperRedirection(args)){
		return NULL;
	}
	LinkedList* stack = LinkedList_init();
	LinkedList* output = LinkedList_init();

	int j = 0;
	char** buffer = calloc(128, 1);
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] != '>' && args[i][0] != '<'){
			buffer[j] = args[i];					//Start collecting args
			j++;
		}else{										//Redirect detected. Queue.
			LinkedList_queue(stack, buffer);
			
			char** redirect = calloc(1,1);			//Add the redirect symbol to the queue
			redirect[0] = args[i];
			LinkedList_queue(stack, redirect);
			
			j = 0;
			buffer = calloc(128, 1);				//Make new args collector
		}
	}
	if(buffer){
		LinkedList_queue(stack, buffer);				//Edge case. Final queue
	}
	output = LinkedList_reverse(stack);				//Reverse stack to get queue
	LinkedList_free(stack);
	return output;
}

/*
void executeRedirection()
	Description:
		Performs a redirection using the arguments passed in.
*/
void executeRedirection(char** args){
	if(!isProperRedirection(args)){ //Error!
		exit(-1);
	}
	int status;
	int numOfRedirects = countRedirections(args);
	char** buffer = NULL;
	LinkedList* arguments= getRedirectionQueue(args);
	
	char*** commands = calloc(numOfRedirects + 2, 1);
	char* redirects= calloc(numOfRedirects + 1, 1);
	
	int i = 0;
	int j = 0;
	while((buffer = LinkedList_next(arguments)) != NULL){	//Sort
		if(buffer[0][0] == '>' || buffer[0][0] == '<'){
			redirects[i] = buffer[0][0];
			i++;
		}else{
			commands[j] = buffer;
			j++;
		}
	}		
	
	char** executableIn = NULL;				//Recieved input
	char** executableOut= NULL;				//Sends input out
	char** filesIn		= calloc(numOfRedirects + 1, 1);	//Recieving
	char** filesOut		= calloc(numOfRedirects + 1, 1);	//Sending
	
	//Sort into in/out
	i = 0;									//Files in cursor
	j = 0;									//Files out
	//TODO Handle more than one redirect.
	if(redirects[0] == '>'){
		executableOut = commands[0];
		filesIn[i] = commands[1][0];
	}else if(redirects[0] =='<'){
		executableIn = commands[0];
		filesOut[i] = commands[1][0];
	}
	//Begin execution cycle
	int pid = fork();
	int redirectOut = open("redirectOut", O_RDWR | O_CREAT | O_TRUNC, 0644);
	
	if(!pid){									//Redirects data out
		fflush(0);
		if(executableOut){						//Check if executable exists
			dup2(redirectOut, STDOUT_FILENO);	//Use redirectOut for stdout.
			dup2(redirectOut, STDERR_FILENO);
//			printf("Bloop: %d\n", 9);
			execvp(executableOut[0], executableOut);
		}else if(filesOut[0]){					//Else, get file
			char** buffer = calloc(1024 + 1, 1);
//			dup2(redirectOut, STDOUT_FILENO);
			int out = open(filesOut[0], O_RDONLY, 0644);
			read(out, buffer, 1024);
//			printf("Boop: %s\n", filesOut[0]);
			write(STDOUT_FILENO, buffer, 1024);
		}
		exit(0);
	}else if(pid > 0){							//Takes in input
		waitpid(pid, &status, 0);
		fflush(0);
		dup2(redirectOut, STDIN_FILENO); 
		char** buffer = calloc(1024 + 1, 1);
		read(STDIN_FILENO, buffer, 1024);
		if(executableIn){
			execvp(executableIn[0], executableIn);
		}else if(filesIn[0]){
			int in = open(filesIn[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			read(in, buffer, 1024);
			write(in, buffer, 1024);
		}
		free(buffer);
	}
}