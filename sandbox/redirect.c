#include "utils.h"
#include "tokenizer.h"
#include "redirect.h"
#include "LinkedList.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/////////////////REDIRECTION RELATED///////////////////
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
	
	for(int i = 0; i < len2(args); i++){
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
	int size = len2(args);
	if(countRedirections(args) > 2){
		return 0;
	}
	if(compareStrings(args[0],"<") || compareStrings(args[0],">") || 
	   compareStrings(args[size - 1],"<") || compareStrings(args[size - 1],">"))
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
	char** buffer = calloc(128, sizeof(*buffer));
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] != '>' && args[i][0] != '<'){
			buffer[j] = args[i];					//Start collecting args
			j++;
		}else{										//Redirect detected. Queue.
			LinkedList_queue(stack, buffer);
			
			char** redirect = calloc(1,sizeof(*redirect));			//Add the redirect symbol to the queue
			redirect[0] = args[i];
			LinkedList_queue(stack, redirect);
			
			j = 0;
			buffer = calloc(128, sizeof(*buffer));				//Make new args collector
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
void executeRedirection(char** args, int isBackground){
	if(!isProperRedirection(args)){ //Error!
		exit(-1);
	}
	int status;
	int numOfRedirects = countRedirections(args);
	char** buffer = NULL;
	LinkedList* arguments= getRedirectionQueue(args);
	
	char*** commands = calloc(numOfRedirects + 2, sizeof(*commands));
	char* redirects= calloc(numOfRedirects + 1, sizeof(redirects));
	
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
	char** filesIn		= calloc(numOfRedirects + 1, sizeof(*filesIn));	//Recieving
	char** filesOut		= calloc(numOfRedirects + 1, sizeof(*filesIn));	//Sending
	
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
			int outCopy = open("stdout", O_RDWR | O_CREAT | O_TRUNC, 0644);	//Make fd copies
			int errCopy = open("stderr", O_RDWR | O_CREAT | O_TRUNC, 0644);
			dup2(STDERR_FILENO, outCopy);
			dup2(STDERR_FILENO, errCopy);
			
			dup2(redirectOut, STDOUT_FILENO);	//Use redirectOut for stdout.
			dup2(redirectOut, STDERR_FILENO);
			
//			printf("Bloop: %d\n", 9);
			execvp(executableOut[0], executableOut);
			
			dup2(outCopy, STDOUT_FILENO);	//Recover fds if something goes wrong somehow
			dup2(errCopy, STDERR_FILENO);
			exit(-1);
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
		char** buffer = calloc(1024 + 1, sizeof(*buffer));
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

//////////////////PIPE RELATED//////////////////
int isPipe(char** args){
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] == '|'){
			return 1;
		}else{
			continue;
		}
	}
	return 0;	
}

int isProperPipe(char** args){
	int size = len2(args);
	if(countPipes(args) != 1){	//Only a single pipe symbol for 2-stage pipeline
		return 0;
	}
	if(compareStrings(args[0], "|")|| compareStrings(args[size - 1], "|")){
		return 0;				//Make sure pipes aren't in front or back.
	}
	return 1;
}

int countPipes(char** args){
	int count = 0;
	
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] == '|'){
			count++;
		}else{
			continue;
		}
	}
	return count;
}

LinkedList* getPipeQueue(char** args){
	if(!isProperPipe(args)){
		return NULL;
	}
	LinkedList* stack = LinkedList_init();
	LinkedList* output = LinkedList_init();
	
	int j = 0;
	char** buffer = calloc(128, sizeof(*buffer));
	for(int i = 0; args[i] != NULL; i++){
		if(args[i][0] != '|'){
			buffer[j] = args[i];					//Start collecting args
			j++;
		}else{										//Pipe detected. Queue.
			LinkedList_queue(stack, buffer);
			
			char** pipeline = calloc(1,sizeof(*pipeline));			//Add the pipeline symbol to the queue
			pipeline[0] = args[i];
			LinkedList_queue(stack, pipeline);
			j = 0;
			buffer = calloc(128, sizeof(*buffer));				//Make new args collector
		}
	}
	if(buffer){
		LinkedList_queue(stack, buffer);				//Edge case. Final queue
	}
	output = LinkedList_reverse(stack);				//Reverse stack to get queue
	LinkedList_free(stack);
	
	return output;

}

void executePipe(char** args, int isBackground, int pgid){
	setpgid(getpid(), pgid);
	if(!isProperPipe(args)){ //Error!
		printf("Error: Malformed pipe command.\n");
		exit(-1);
	}
	int status;
	int numOfPipes = 1;
	char** buffer = NULL;
	LinkedList* arguments= getPipeQueue(args);
	
	//Get boths sides of pipe
	char** leftSide = LinkedList_next(arguments);
	LinkedList_next(arguments);
	char** rightSide = LinkedList_next(arguments);	
	
	//Check for errors
	if(isRedirection(rightSide)){
		if(!isProperRedirection(rightSide)){
			printf("Error: Malformed redirection command.\n");
			exit(-1);
		}
	}
	if(isRedirection(leftSide)){
		if(!isProperRedirection(leftSide)){
			printf("Error: Malformed redirection command.\n");
			exit(-1);
		}
	}
	
	//Make pipe
	int fd[2];
	pipe(fd);	
	int pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (!pid){   
		close(fd[1]);
		dup2(fd[0], STDOUT_FILENO);
		if(isRedirection(leftSide)){
			executeRedirection(leftSide, isBackground);
		}else{
			execvp(leftSide[0], leftSide);
		}
		exit(0);
    }else{            
		close(fd[0]);         
		waitpid(pid, &status, 0);
		dup2(fd[1], STDIN_FILENO);
		if(isRedirection(rightSide)){
			executeRedirection(rightSide, isBackground);
		}else{
			execvp(rightSide[0], rightSide);
		}
		exit(0);
    }
}
