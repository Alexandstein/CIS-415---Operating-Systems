//Code by Alexander Stein 2013-2014

#include "LinkedList.h"
#include <stdlib.h>

/*************NODE FUNCTIONS****************/
/*//////////////////////////////////////////////////////////////////////////*/
/*CONSTRUCTOR: 																*/
/*		Node* Node_init										  				*/
/*Constructs an empty Node													*/
/*																			*/
/*Args: 																	*/
/*		void			:													*/
/*Return:																	*/
/*		Pointer to the new Node												*/
/*//////////////////////////////////////////////////////////////////////////*/
Node* Node_init()
{
	Node* output = (Node*)malloc(sizeof(Node));		//Allocate memory
	
	Node_setData(output, NULL);						//Initialize data to NULL
	Node_append(output, NULL);
	
	return output;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*CONSTRUCTOR: 																*/
/*		Node* Node_initWithInput							  				*/
/*Constructs an initialized Node											*/
/*																			*/
/*Args: 																	*/
/*		void* input:														*/
/*			Pointer to data to use as input									*/
/*Return:																	*/
/*		Pointer to the new Node												*/
/*//////////////////////////////////////////////////////////////////////////*/
Node* Node_initWithInput(void* input)
{
	Node* output = (Node*)malloc(sizeof(Node));		//Allocate memory
	
	Node_setData(output, input);					//Initialize data
	Node_append(output, NULL);						//Next is NULL
	
	return output;
}

/*Retrieves and returns the data contained in a node*/
/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void* Node_getData									  				*/
/*Retrieves data from Node_init												*/
/*																			*/
/*Args: 																	*/
/*		Node* dataSource:													*/
/*			Pointer to Node to retrieve from								*/
/*Return:																	*/
/*		Pointer to the data													*/
/*//////////////////////////////////////////////////////////////////////////*/
void* Node_getData(Node* dataSource)
{
	return dataSource->data;						//Return data
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		Node *Node_setData										  			*/
/*Sets the data for a node													*/
/*																			*/
/*Args: 																	*/
/*		Node* dataTarget:													*/
/*			Pointer to Node to receive the data								*/
/*		void* input:														*/
/*			Data to be set in `dataTarget`									*/
/*Return:																	*/
/*		Pointer to target node												*/
/*//////////////////////////////////////////////////////////////////////////*/
Node* Node_setData(Node* dataTarget, void* input)
{
	dataTarget->data = input;
	
	return dataTarget;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		Node* Node_getNext										  			*/
/*Retrieve Node stored in `next` pointer									*/
/*																			*/
/*Args: 																	*/
/*		Node* node:															*/
/*			Node to retrieve next from										*/
/*Return:																	*/
/*		Pointer to next Node, or NULL if there is no next Node				*/
/*//////////////////////////////////////////////////////////////////////////*/
Node* Node_getNext(Node* node)
{
	return node->next;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		Node* Node_setData										  			*/
/*Attaches child Node as the 'next' variable for parent						*/
/*																			*/
/*Args: 																	*/
/*		Node* parent:														*/
/*			Node who's `next` variable will be set to `child`				*/
/*		Node* child:														*/
/*			Target for parent's `next` variable								*/
/*Return:																	*/
/*		Pointer to `parent`													*/
/*//////////////////////////////////////////////////////////////////////////*/
Node* Node_append(Node* parent, Node* child)
{
	parent->next = child;
	return parent;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*DESTRUCTOR: 																*/
/*		void Node_free										  				*/
/*Frees a Node from memory													*/
/*																			*/
/*Args: 																	*/
/*		Node* toDelete:														*/
/*			Node to delete													*/
/*Return:																	*/
/*		void																*/
/*//////////////////////////////////////////////////////////////////////////*/
void Node_free(Node* toDelete)
{
//	toDelete->next = NULL;
//	if(toDelete->data != 0){
//		free(toDelete->data);
//	}
	free(toDelete);

	return;
}

/**************LINKED LIST FUNCTIONS***************/

/*//////////////////////////////////////////////////////////////////////////*/
/*CONSTRUCTOR: LinkedList* LinkedList_init					  				*/
/*Constructor for LinkedList struct.										*/
/*Args: 																	*/
/*		void																*/
/*Return:																	*/
/*		Initiated LinkedList struct											*/
/*//////////////////////////////////////////////////////////////////////////*/
LinkedList* LinkedList_init()
{
	LinkedList* output = (LinkedList*)malloc(sizeof(LinkedList));
	output->head = NULL;
	output->cursor = NULL;
	
	return output;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: int LinkedList_isEmpty							  				*/
/*Checks if list is empty													*/
/*																			*/
/*Args: 																	*/
/*		LinkedList* list													*/
/*			The list to be checked											*/
/*Return:																	*/
/*		1 (true) if empty, 0 (false) if not empty							*/
/*//////////////////////////////////////////////////////////////////////////*/
int LinkedList_isEmpty(LinkedList* list){
	if(list->head == NULL){
		return 1;
	}else{
		return 0;
	}
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void LinkedList_queue						  						*/
/*DESCRIPTION																*/
/*		Pushes data from the head of the list								*/
/*Args: 																	*/
/*		LinkedList* list:													*/
/*			The receiving list												*/
/*		void* input:														*/
/*			The data to be pushed onto `list`								*/
/*Return:																	*/
/*		void																*/
/*//////////////////////////////////////////////////////////////////////////*/
void LinkedList_queue(LinkedList* list, void* input)
{	
	Node* toPush = Node_initWithInput(input);
	
	if(LinkedList_isEmpty(list)){						//Check if empty
		list->head 	 = toPush;
		list->cursor = toPush;
		return;
	}else{												//Not empty
		Node_append(toPush, list->head);				//Link Nodes together
		list->head = toPush;							//Replace the head with new Node
		LinkedList_resetCursor(list);
		return;
	}
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void* LinkedList_dequeue							  				*/
/*Dequeues data from tail													*/
/*																			*/
/*Args: 																	*/
/*		LInkedList* list:													*/
/*			LinkedList to pop from											*/
/*Return:																	*/
/*		Returns the popped data												*/
/*//////////////////////////////////////////////////////////////////////////*/
void* LinkedList_dequeue(LinkedList* list)
{
	Node* beforeNext = NULL;							//Keep track of node before cursor
	Node* toDelete;										//Remember to free memory!
	void* output;

	if(LinkedList_isEmpty(list)){						//Check if list is empty
		return NULL;									//Empty; return NULL
	}else{												//List is not empty; continue
		while(LinkedList_isNext(list)){					//Iterate to end.
			beforeNext = list->cursor;
			LinkedList_next(list);
		}
		output = list->cursor->data;
		toDelete = list->cursor;
		if(beforeNext){									//beforeNext is not NULL
			beforeNext->next = NULL;					//Sever connection
		}else{
			list->cursor = NULL;						//Else, set cursor to NULL
			list->head	 = NULL;
		}

		LinkedList_resetCursor(list);					//Reset cursor.

		Node_free(toDelete);							//Clean up
		printf("Output: %s\n", output);					//TODO DEBUG
		return output;	
	}
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void LinkedList_resetCursor							  				*/
/*Resets `list->cursor` to the head of the list								*/
/*																			*/
/*Args: 																	*/
/*		LInkedList* list:													*/
/*			LinkedList to manipulate										*/
/*Return:																	*/
/*		void																*/
/*//////////////////////////////////////////////////////////////////////////*/
void LinkedList_resetCursor(LinkedList* list)
{
	list->cursor = list->head;
	return;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void* LinkedList_next								  				*/
/*Advance the cursor and return the data value								*/
/*																			*/
/*Args: 																	*/
/*		LInkedList* list:													*/
/*			LinkedList to manipulate										*/
/*Return:																	*/
/*		Returns the data of the passed over node							*/
/*//////////////////////////////////////////////////////////////////////////*/
void* LinkedList_next(LinkedList* list)
{
	if(list->cursor == NULL){
		return NULL;							//return null if there is no node on cursor.
	}
	
	void* output = Node_getData(list->cursor);
	list->cursor = Node_getNext(list->cursor);
		
	return output;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		int LinkedList_isNext								  				*/
/*Checks whether there exists a next node 									*/
/*																			*/
/*Args: 																	*/
/*		LInkedList* list:													*/
/*			LinkedList to check												*/
/*Return:																	*/
/*		1 if there is a next node. 0 otherwise								*/
/*//////////////////////////////////////////////////////////////////////////*/
int LinkedList_isNext(LinkedList* list)
{
	if(list == NULL || Node_getNext(list->cursor) == NULL){
		return 0;
	}
		return 1;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		int LinkedList_len									  				*/
/*Checks size of LinkedList													*/
/*																			*/
/*Args: 																	*/
/*		LinkedList* list:													*/
/*			LinkedList to check												*/
/*Return:																	*/
/*		Size of list														*/
/*//////////////////////////////////////////////////////////////////////////*/
int LinkedList_len(LinkedList* list){
	Node* saved = list->cursor;				//Save cursor position
	
	int i;
	for(i = 0; LinkedList_next(list); i++){}
	
	return i;
}

/*//////////////////////////////////////////////////////////////////////////*/
/*DESTRUCTOR: 																*/
/*		void LinkedList_free								  				*/
/*Frees a LinkedList from memory											*/
/*																			*/
/*Args: 																	*/
/*		LInkedList* list:													*/
/*			LinkedList to delete											*/
/*Return:																	*/
/*		void																*/
/*//////////////////////////////////////////////////////////////////////////*/
void LinkedList_free(LinkedList* list)
{
	Node* node;
	while((node = LinkedList_next(list)) != NULL){			//Keep iterating through to delete all nodes.
//		free(LinkedList_next(list));
	}
	free(list);								//Finally, the list itself
	
	return;
}