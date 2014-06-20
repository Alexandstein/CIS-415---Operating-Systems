////////////////////////////////////////////////////////////////////////////////
//
//  File           : smsa_driver.c
//  Description    : This is the driver for the SMSA simulator.
//
//   Author        : 
//   Last Modified : 
//

// Include Files

// Project Include Files
#include <smsa_driver.h>
#include <cmpsc311_log.h>
#include <stdlib.h>

// Defines

// Functional Prototypes

////////////////////////////////////////////////////////////
//OP CODE HELPERS
////////////////////////////////////////////////////////////

//Takes operation type, drum, and block and returns an op code by OR-ing them together
//along with some bit shifts
uint32_t generateOP(int operation, int drum, int block){
	uint32_t output = 0;
	operation = operation << 26;	//Move bits to proper spot.
	drum = drum << 22;
	
	output = operation | drum | block;
	return output;
}

////////////////////////////////////////////////////////////
//ADDRESS TRANSLATION FUNCTIONS
////////////////////////////////////////////////////////////

//Takes an address and returns the number of a block.
int getBlock(SMSA_VIRTUAL_ADDRESS addr){
	if(addr > MAX_SMSA_VIRTUAL_ADDRESS){
		return -7;	//Address too big; invalid block error
	}else{
		addr = addr%SMSA_DISK_SIZE;			//Reduce mod disk size to get the block addr
		return addr/SMSA_MAX_BLOCK_ID;
	}
}

//Takes an address and returns the number of a disk
int getDisk(SMSA_VIRTUAL_ADDRESS addr){
	if(addr > MAX_SMSA_VIRTUAL_ADDRESS){
		return -6;							//Address too big; invalid disk error
	}else{
		return addr/SMSA_DISK_SIZE;
	}
}

////////////////////////////////////////////////////////////
//SEEKING FUNCIONS
////////////////////////////////////////////////////////////

//Move head to drum
int opSeekDisk(int drum){
	uint32_t op = generateOP(SMSA_SEEK_DRUM, drum, 0);
	int status = smsa_operation(op, NULL);
	return status;
}

//Move head to block
int opSeekBlock(int block){
	uint32_t op = generateOP(SMSA_SEEK_BLOCK, 0, block);
	int status = smsa_operation(op, NULL);
	return status;
}

//Return move to given address position.
int opSeek(SMSA_VIRTUAL_ADDRESS addr){
	int block = getBlock(addr);
	int disk  = getDisk(addr);
	int status= 0;
	uint32_t op = 0;
	
	logMessage(LOG_OUTPUT_LEVEL, "OP: Seek Disk: %d Block: %d (Addr=%d) ", disk, block, addr);
	
	opSeekDisk(disk);
	opSeekBlock(block);
	
	return 0;
}

////////////////////////////////////////////////////////////
//READ/WRITE
////////////////////////////////////////////////////////////

//Wrapper for writing
//Negative address means do not change head position prior to writing
int opWrite(int addr, char* buffer, int len){
	if(addr < 0){					//Don't change head positions
		logMessage(LOG_OUTPUT_LEVEL, "Write w/o Seek\n");
	}else{							//Change head position
		SMSA_VIRTUAL_ADDRESS address = addr;
		opSeek(address);
	}
	
	uint32_t op = generateOP(SMSA_DISK_WRITE, 0, 0);
	int status = smsa_operation(op, buffer);
	return status;
}

int opRead(int addr, char* buffer, int len){
	if(addr < 0){					//Don't change head positions
		logMessage(LOG_OUTPUT_LEVEL, "Write w/o Seek\n");
	}else{							//Change head position
		SMSA_VIRTUAL_ADDRESS address = addr;
		opSeek(address);
	}
	
	uint32_t op = generateOP(SMSA_DISK_READ, 0, 0);
	int status = smsa_operation(op, buffer);
	return status;
}


//Wipe the contents of a block
int formatBlock(SMSA_VIRTUAL_ADDRESS addr){
	//TODO Everything
	return -1;
}

//Store testing code
int testing(){
	int test = 256;
	char* buffer = calloc(256, sizeof(char));
	opWrite(test, "BLOOP\n", 6);
	opRead(test, buffer, 6);
	logMessage(LOG_OUTPUT_LEVEL, buffer);
	
}
//
// Global data

// Interfaces

////////////////////////////////////////////////////////////////////////////////
//
// Function     : smsa_vmount
// Description  : Mount the SMSA disk array virtual address space
//
// Inputs       : none
// Outputs      : -1 if failure or 0 if successful

int smsa_vmount( void ) {
	uint32_t op = generateOP(SMSA_MOUNT, 0, 0);		//Mount disk
	int status = smsa_operation(op, NULL);
													//TODO Read data
	testing();
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : smsa_vunmount
// Description  :  Unmount the SMSA disk array virtual address space
//
// Inputs       : none
// Outputs      : -1 if failure or 0 if successful

int smsa_vunmount( void )  {
	uint32_t op = generateOP(SMSA_UNMOUNT, 0, 0);		//Mount disk
	int status = smsa_operation(op, NULL);
														//TODO Save data
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : smsa_vread
// Description  : Read from the SMSA virtual address space
//
// Inputs       : addr - the address to read from
//                len - the number of bytes to read
//                buf - the place to put the read bytes
// Outputs      : -1 if failure or 0 if successful

int smsa_vread( SMSA_VIRTUAL_ADDRESS addr, uint32_t len, unsigned char *buf ) {
	
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : smsa_vwrite
// Description  : Write to the SMSA virtual address space
//
// Inputs       : addr - the address to write to
//                len - the number of bytes to write
//                buf - the place to read the read from to write
// Outputs      : -1 if failure or 0 if successful

int smsa_vwrite( SMSA_VIRTUAL_ADDRESS addr, uint32_t len, unsigned char *buf )  {

}

//logMessage(LOG_OUTPUT_LEVEL, "Test %d\n", 0);