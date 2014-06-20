#ifndef __ERRCHECK_H__
#define __ERRCHECK_H__

#include <errno.h>
/*
	Wrapper for checking errno values
	Return 1 if error occurs
*/

int fileNotFoundErr();
int noChildProcErr();
int brokenPipeErr();

#endif