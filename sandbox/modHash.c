#include "modHash.h"
#include <stdlib.h>


/*
	Constructor for ModHash
	args
		int size
			Size of the modular table
*/
ModHash* ModHash_init(int size){
	ModHash* output = calloc(1,sizeof(*output));
	
	output->size = size;
	output->table = calloc(size,sizeof(void*));
	
	return output;
}



/*
	Description
		Populates a part of the table with an element. If there is a collision, the
		initial value is replaced by the new one. //TODO: Think of better behavior?
	Args
		ModHash* table
			The recieving table
		int key
			The number value which determines the slot the data occupies. If a number
			is larger than the size of the table, then it's reduced modulo `this->size`.
		void* value
			The data to be stored
	Return
		(void)
*/
void ModHash_put(ModHash* table, int key, void* value){
	key = key%table->size;		//Modular arithmetic to reduce key
	table->table[key] = value;
}

/*
	Description
		Gets and element in constant time using the key supplied mod `this->size`.
	Args
		ModHash* table
			The table
		int key
			The data to be retrieved. May be NULL
	Return:
		The data requested, or NULL if none occupies the space.
*/
void* ModHash_get(ModHash* table, int key){
	key = key%table->size;
	return (table->table)[key];
}

/*
	Description
		Remove elements from table
	Args
		ModHash table
			The table
		int key
			The key of the data to be removed.
		int freeOnRemove
			If 0, simply set the removed index to NULL. If else, the object stored has 
			free() called on it.
	Return:
		(void)
*/
void ModHash_remove(ModHash* table, int key, int freeOnRemove){
	key = key%table->size;
	
	if(freeOnRemove){
		void* toRemove;
		toRemove = table->table[key];
		table->table[key] = NULL;
		free(toRemove);
	}else{
		table->table[key] = NULL;
	}
}

/*
	Destructor
	Args
		int freeAll
			If set, then free() will be called on all non-NULL table entries
*/
void ModHash_free(ModHash* table, int freeAll){
	if(freeAll){
		for(int i = 0; i < table->size; i++){
			if(table->table[i] != NULL){
				free(table->table[i]);
			}
		}
	}
	free(table->table);
	free(table);
}