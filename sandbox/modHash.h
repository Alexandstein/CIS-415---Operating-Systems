#ifndef __MODHASH_H__
#define __MODHASH_H__

/*
	Primitive hash table which uses modular arithmetic as hash function. Should be good
	enough since the space of numbers is just process ids.
*/
typedef struct ModHash{
	int size;
	void** table;
} ModHash;

/*
	Constructor for ModHash
	args
		int size
			Size of the modular table
*/
ModHash* ModHash_init(int size);

void ModHash_put(ModHash* table, int key, void* value);

/*
	Description
		Gets and element in constant time using the key supplied mod `this->size`.
	Args
		ModHash table
			The table
		int key
			The data to be retrieved. May be NULL
	Return:
		The data requested, or NULL if none occupies the space.
*/
void* ModHash_get(ModHash* table, int key);

/*
	Description
		Remove elements from table
	Args
		ModHash table
			The table
		int key
			The key of the data to be removed.
		int freeOnRemove
			If 0, simply set the removed index to NULL. If 1, the object stored has 
			free() called on it.
	Return:
		(void)
*/
void ModHash_remove(ModHash* table, int key, int freeOnRemove);

/*
	Destructor
		int freeAll
			If set, then free() will be called on all non-NULL table entries
*/
void ModHash_free(ModHash* table, int freeAll);
#endif