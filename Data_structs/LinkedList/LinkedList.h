#pragma once
#include <stdlib.h>
#include "Records.h"

//Struct for each list node.
typedef struct Lnode {
    Records *data;
    struct Lnode* next;
} Lnode;

typedef Lnode* LinkedList; //The whole list is represented as a pointer to a list node.

//----------------------Linked List function declarations------------------------

//Creates new single linked list with a dummy node as head
LinkedList ListCreate();  

//Destroys a list
void ListDestroy(LinkedList);

//Inserts node with a given data in the first position of the list
void ListInsertFirst(LinkedList *, Records *);

Records *ListSearch(LinkedList, char*);

//Returnes the number of nodes in the list
int ListNodeCount(LinkedList);

//Returns the number of nodes that match the given country
int ListNodeCountryCount(LinkedList , char *);

//Returns the number of nodes that match the given disease
int ListDiseaseCount(LinkedList, char *);

//Returns the number of nodes with no exit date
int ListStillInHospital(LinkedList);






