#include "utils.h"
#include "tokenizer.h"
#include "LinkedList.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


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
	int i;
	for(i = 0; input[i] != NULL; i++){}
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

/*
	Description:
		Takes a string and parses its contents into arguments usable by execvp(3).
		
	args:
		char* inputString
			An unprocessed string to be tokenized.
	return
		A char** carrying the separated tokens in each element of the array.
*/
char** toExecArgs(char* inputString){
	int MAX = 256;
	TOKENIZER *tokenizer;
	char** args = calloc(MAX + 1, 1);
	
	tokenizer = init_tokenizer(inputString);
	
	int i = 0;
	while((args[i] = get_next_token(tokenizer)) != NULL){	
		i++;	
	}
	args[MAX] = NULL;		//Make sure that array ends in NULL for bounding.
	return args;
}

//Convert array to LinkedList
LinkedList* arrayToList(void** array, int size){
	LinkedList* output = LinkedList_init();
	
	for(int i = 0; i < size; i++){
		LinkedList_queue(output, array[i]);
	}
	
	return output;
}

//Convert LinkedLIst to array
void** listToArray(LinkedList* list){
	void** output = calloc(LinkedList_len(list) + 1,1);
	void* buffer;
	
	for(int i = 0;(buffer = LinkedList_next(list)) != NULL; i++){
		output[i] = buffer;
	}
	return output;
}
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
	int redirectOut = open("redirectOut", O_RDWR | O_CREAT, 0644);
	
	if(!pid){									//Redirects data out
		fflush(0);
		if(executableOut){						//Check if executable exists
			dup2(redirectOut, STDOUT_FILENO);	//Use redirectOut for stdout.
			dup2(redirectOut, STDERR_FILENO);
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
		dup2(redirectOut, STDIN_FILENO); 
		char** buffer = calloc(1024 + 1, 1);
		read(STDIN_FILENO, buffer, 1024);
//		printf("Boop: %s\n", test);
		if(executableIn){
			execvp(executableIn[0], executableIn);
		}else if(filesIn[0]){
			int in = open(filesIn[0], O_WRONLY | O_CREAT, 0644);
//			dup2(redirectOut, in);
//			read(in, buffer, 1024);
			write(in, buffer, 1024);
		}
		free(buffer);
	}
}