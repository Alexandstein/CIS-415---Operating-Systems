//Code by Alexander Stein 2013-2014

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*//////////////////////////////////////////////////////////////////////////*/
/*STRUCT: 																	*/
/*		Node																*/
/*Nodes for LinkedList struct												*/
/*																			*/
/*Members: 																	*/
/*		Node* next:															*/
/*			Appended child node												*/
/*		void* data:															*/
/*			Data held in the node											*/
/*//////////////////////////////////////////////////////////////////////////*/
typedef struct Node{
	struct Node* next;
	void* data;
} Node;

/*//////////////////////////////////////////////////////////////////////////*/
/*STRUCT: 																	*/
/*		LinkedList															*/
/*Linked List data structure												*/
/*																			*/
/*Members: 																	*/
/*		Node* head:															*/
/*			The head node of the list										*/
/*		Node* cursor:														*/
/*			Internal pointer												*/
/*//////////////////////////////////////////////////////////////////////////*/
typedef struct LinkedList{
	Node* head;
	Node* cursor;
} LinkedList;
#endif

/*Node FUNCTIONS*/
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
Node* Node_init();

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
Node* Node_initWithInput(void* input);

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
void* Node_getData(Node* dataSource);

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
Node* Node_setData(Node* dataTarget, void* input);

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
Node* Node_getNext(Node* node);

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
Node* Node_append(Node* parent, Node* child);

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
void Node_free(Node* toDelete);

/*Linked List FUNCTIONS*/
/*//////////////////////////////////////////////////////////////////////////*/
/*CONSTRUCTOR: LinkedList* LinkedList_init					  				*/
/*Constructor for LinkedList struct.										*/
/*Args: 																	*/
/*		void																*/
/*Return:																	*/
/*		Initiated LinkedList struct											*/
/*//////////////////////////////////////////////////////////////////////////*/
LinkedList* LinkedList_init();

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
int LinkedList_isEmpty(LinkedList* list);

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void LinkedList_queue							  					*/
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
void LinkedList_queue(LinkedList* list, void* input);

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void* LinkedList_dequeue							  				*/
/*Pops data off from the tail												*/
/*																			*/
/*Args: 																	*/
/*		LInkedList* list:													*/
/*			LinkedList to pop from											*/
/*Return:																	*/
/*		Returns the popped data												*/
/*//////////////////////////////////////////////////////////////////////////*/
void* LinkedList_popFromHead(LinkedList* list);

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
void LinkedList_resetCursor(LinkedList* list);

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		void* LinkedList_next								  				*/
/*Advance the cursor and return the data value								*/
/*																			*/
/*Args: 																	*/
/*		LinkedList* list:													*/
/*			LinkedList to manipulate										*/
/*Return:																	*/
/*		Returns the data of the passed over node							*/
/*//////////////////////////////////////////////////////////////////////////*/
void* LinkedList_next(LinkedList* list);

/*//////////////////////////////////////////////////////////////////////////*/
/*FUNCTION: 																*/
/*		int LinkedList_isNext								  				*/
/*Checks whether there exists a next node 									*/
/*																			*/
/*Args: 																	*/
/*		LinkedList* list:													*/
/*			LinkedList to check												*/
/*Return:																	*/
/*		1 if there is a next node. 0 otherwise								*/
/*//////////////////////////////////////////////////////////////////////////*/
int LinkedList_isNext(LinkedList* list);

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
int LinkedList_len(LinkedList* list);

/*//////////////////////////////////////////////////////////////////////////*/
/*DESTRUCTOR: 																*/
/*		void LinkedList_free								  				*/
/*Frees a LinkedList from memory											*/
/*																			*/
/*Args: 																	*/
/*		LinkedList* list:													*/
/*			LinkedList to delete											*/
/*Return:																	*/
/*		void																*/
/*//////////////////////////////////////////////////////////////////////////*/
void LinkedList_free(LinkedList* list);