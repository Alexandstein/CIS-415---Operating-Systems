#include "utils.h"
#include "tokenizer.h"
#include "redirect.h"
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
