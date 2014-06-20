#ifndef REDIRECT_H
#define REDIRECT_H

#include "tokenizer.h"
#include "LinkedList.h"

///////REDIRECTION RELATED//////
int isRedirection(char** args);

int isProperRedirection(char** args);

int countRedirections(char** args);

int redirectPositions(char** args);

LinkedList* getRedirectionQueue(char** args);

void executeRedirection(char** args, int isBackground);

////////PIPE RELATED//////////////
int isPipe(char** args);

int isProperPipe(char** args);

int countPipes(char** args);

LinkedList* getPipeQueue(char** args);

void executePipe(char** args, int isBackground, int pgid);

#endif