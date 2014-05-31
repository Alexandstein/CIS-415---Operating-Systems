#ifndef __UTILS_H__
#define __UTILS_H__

#include "tokenizer.h"
#include "LinkedList.h"

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
int len(const char* input);

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
int toDigit(char inputChar);

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
int toInt(char* input);

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
void sanitize(char* input);

/*
	Description:
		Takes a string and parses its contents into arguments usable by execvp(3).
		
	args:
		char* inputString
			An unprocessed string to be tokenized.
	return
		A char** carrying the separated tokens in each element of the array.
*/
char** toExecArgs(char* inputString);

LinkedList* arrayToList(void** array, int size);

void** listToArray(LinkedList* list);
#endif

