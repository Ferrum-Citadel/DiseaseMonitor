#pragma once
#include "Hashtable.h"

//Struct for each bucket entry
typedef struct RecBucketEntry {
    char *key;
    Records *value;
} RecBucketEntry;

//Struct for each bucket
typedef struct RecBucket {
    RecBucketEntry **entries;
    struct RecBucket *next;
    int count;
} RecBucket;

//Sstruct for the Hashtable
typedef struct RecHash {
    size_t capacity;
    size_t used_entries;
    RecBucket **table;
} RecHash;

//-------------------------------Declaration of Hashtable functions-------------------------
//Takes 0 as argument if the hashtable holds AVL trees and 1 if it holds records
RecHash *RecHashCreate(size_t);
void RecHashDestroy(RecHash *);
//Returns 0 if the inserted item already exists and 0 if it doesnt
void RecHashInsert(RecHash **, char *, Records *, size_t);
Records *RecHashGetItem(RecHash *, char *); //Returns NULL if not found