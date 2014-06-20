#include <errno.h>
/*
	Wrapper for checking errno values
	Return 1 if error occurs
*/

int fileNotFoundErr(){
	if(errno == ENOENT){
		return 1;
	}
	return 0;
}

int noChildProcErr(){
	if(errno == ECHILD){
		return 1;
	}
	return 0;
}

int brokenPipeErr(){
	if(errno == EPIPE){
		return 1;
	}
	return 0;
}