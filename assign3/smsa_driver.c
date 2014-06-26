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
//ERROR HELPER
////////////////////////////////////////////////////////////

void printErr(int errnum){
	logMessage(1, smsa_error_string(errnum));
}

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
	if(status){
		printErr(status);
	}
	return status;
}

//Move head to block
int opSeekBlock(int block){
	uint32_t op = generateOP(SMSA_SEEK_BLOCK, 0, block);
	int status = smsa_operation(op, NULL);
	if(status){
		printErr(status);
	}
	return status;
}

//Return move to given address position.
int opSeek(SMSA_VIRTUAL_ADDRESS addr){
	int block = getBlock(addr);
	int disk  = getDisk(addr);
	int status= 0;
	uint32_t op = 0;
	
	logMessage(16, "OP: Seek Disk: %d Block: %d (Addr=%d) ", disk, block, addr);
	
	opSeekDisk(disk);
	opSeekBlock(block);
	
	return 0;
}

////////////////////////////////////////////////////////////
//READ/WRITE
////////////////////////////////////////////////////////////

//Wrapper for writing. Writes to a single block.
//moveHead means to change head position to addr prior to writing
int opWrite(SMSA_VIRTUAL_ADDRESS addr, unsigned char* buffer, int moveHead){
	if(moveHead == 0){					//Don't change head positions
		logMessage(16, "Write w/o Seek\n");
	}else{							//Change head position
		SMSA_VIRTUAL_ADDRESS address = addr;
		opSeek(address);
	}
	
	uint32_t op = generateOP(SMSA_DISK_WRITE, 0, 0);
	int status = smsa_operation(op, buffer);
	if(status){
		printErr(status);
	}
	return status;
}

//Read wrapper. Reads a single block.
//moveHead means to change head position to addr prior to writing
int opRead(SMSA_VIRTUAL_ADDRESS addr, unsigned char* buffer, int moveHead){
	if(moveHead == 0){				//Don't change head positions
		logMessage(16, "Write w/o Seek\n");
	}else{							//Change head position
		SMSA_VIRTUAL_ADDRESS address = addr;
		opSeek(address);
	}
	
	uint32_t op = generateOP(SMSA_DISK_READ, 0, 0);
	int status = smsa_operation(op, buffer);
	if(status){
		printErr(status);
	}
	return status;
}

//Wipe the contents of a block
int formatBlockByAddr(SMSA_VIRTUAL_ADDRESS addr){
	char* blank = calloc(SMSA_BLOCK_SIZE, sizeof(*blank));				//Make blank block
	int status = opWrite(addr, blank, 1);
	free(blank);
	return status;
}

//Format entire SMSA disk
int formatSMSA(){
	int i;
	int status;
	disableLogLevels(4);
	for(i = 0; i < MAX_SMSA_VIRTUAL_ADDRESS; i += SMSA_BLOCK_SIZE){
		status = formatBlockByAddr(i);
		if(status){										//Error check
			printErr(status);
			return status;
		}
	}
	return 0;
}

//Store testing code
void testing(){
	enableLogLevels(16);
	int test = 0;
	unsigned char* buffer = calloc(SMSA_BLOCK_SIZE * 5, sizeof(*buffer));
	unsigned char* input  = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	unsigned char* input2 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	smsa_vwrite(test, 400, input);
//	smsa_vwrite(test + 1, 258, input2);
	smsa_print(250, 25);
	free(buffer);
}

//Print region of disk for testing reasons.
void smsa_print(SMSA_VIRTUAL_ADDRESS start, int len){
	unsigned char* buffer = calloc(SMSA_BLOCK_SIZE * 10, sizeof(*buffer));
	smsa_vread(start, len, buffer);
	logMessage(16, buffer);
	free(buffer);
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
	registerLogLevel("DEBUG", 16);					//Register debugging
	disableLogLevels(16 | 4);

	uint32_t op = generateOP(SMSA_MOUNT, 0, 0);		//Mount disk
	int status = smsa_operation(op, NULL);
	if(status){										//Error check
		printErr(status);
		return -1;
	}
													//TODO Read data
	status = formatSMSA();							//Format disk
	if(status){										//Error check
		printErr(status);
		return -1;
	}
	testing();	return -1;		//DEBUGGING STUFF
	return 0;
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
	return 0;
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
	logMessage(16, "Read of length: %d at addr %d\n", len, addr);
	
	int i = 0;
	int status = 0;
	int offset = addr % SMSA_BLOCK_SIZE;
	int offsetEnd = len + addr;
	unsigned char* offsetBuffer;
	
	if(addr+len > MAX_SMSA_VIRTUAL_ADDRESS){			//Check if within bounds of addr	
		logMessage(LOG_ERROR_LEVEL, "Out of Bounds Error. Attempted acces to addr: %d\n", len + addr);
	}
	int numOfBlocks = (offset + len - 1)/SMSA_BLOCK_SIZE + 1;	//Get number of blocks to read
	offsetBuffer = calloc(numOfBlocks*SMSA_BLOCK_SIZE + 1, sizeof(*offsetBuffer));
	
	opSeek(addr);
	for(i = 0; i < numOfBlocks; i++){
		status = opRead(addr + i*SMSA_BLOCK_SIZE, offsetBuffer + i*SMSA_BLOCK_SIZE, 1);
		if(status){
			printErr(status);
			return -1;
		}
	}
	//TODO Allow non-block offsets
	//Copy buffer over, but with offset
	for(i = 0; i < len; i++){
		buf[i] = offsetBuffer[i + offset];
	}
	
	return 0;
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
	int i = 0;
	int status = 0;
	int offset = addr % SMSA_BLOCK_SIZE;
	int offsetEnd = len + addr;
	unsigned char* offsetBuffer;
	
	if(addr+len > MAX_SMSA_VIRTUAL_ADDRESS){			//Check if within bounds of addr	
		logMessage(LOG_ERROR_LEVEL, "Out of Bounds Error. Attempted acces to addr: %d\n", len + addr);
	}
	int numOfBlocks = (offset + len - 1)/SMSA_BLOCK_SIZE + 1;	//Get number of blocks to read
	offsetBuffer = calloc(numOfBlocks*SMSA_BLOCK_SIZE + 1, sizeof(*offsetBuffer));
	
	opRead(addr, offsetBuffer, 1);			//Preload last and first blocks for splicing
	opRead((numOfBlocks-1)*SMSA_BLOCK_SIZE, offsetBuffer + (numOfBlocks-1)*SMSA_BLOCK_SIZE, 1);			
	
	
	//Copy buffer over, but with offset
	for(i = 0; i < len; i++){
		offsetBuffer[i + offset] = buf[i];
	}
	
	logMessage(16, "Write of length: %d at addr %d (Target=%d\n", len, addr, len+addr);
	opSeek(addr);
	for(i = 0; i < numOfBlocks; i++){
		status = opWrite(addr + i*SMSA_BLOCK_SIZE, offsetBuffer + i*SMSA_BLOCK_SIZE, 1); //Increment address
		if(status){
			printErr(status);
			return -1;
		}
	}
	//TODO Allow non-block offsets [X]
	//TODO Read from file to offset buffer to splice last and first blocks on non-aligned writes[x]
	//TODO Fix truncation problem at last blocks.
	
	return 0;
}

//logMessage(16, "Test %d\n", 0);