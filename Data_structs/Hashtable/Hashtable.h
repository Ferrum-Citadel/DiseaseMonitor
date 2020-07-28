#pragma once
#include "../AVL/AVL.h"

//Struct for each bucket entry
typedef struct BucketEntry {
    char *key;
    AVLTree *value;
} BucketEntry;

//Struct for each bucket
typedef struct Bucket {
    BucketEntry **entries;
    struct Bucket *next;
    int count;
} Bucket;

//Sstruct for the Hashtable
typedef struct Hashtable {
    size_t capacity;
    size_t used_entries;
    Bucket **table;
} Hashtable;

//-------------------------------Declaration of Hashtable functions-------------------------
//Takes 0 as argument if the hashtable holds AVL trees and 1 if it holds records
Hashtable *HashCreate(size_t);
void HashDestroy(Hashtable *);
//Returns 0 if the inserted item already exists and 0 if it doesnt
void HashInsert(Hashtable **, char *, AVLTree *, size_t);
AVLTree *HashGetItem(Hashtable *, char *); //Returns NULL if not found