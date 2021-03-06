#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "RecHash.h"

//-------------------------------Definition of static functions---------------------------
//Djb2 hash function for strings
static int Hashfunction(char *key, size_t capacity){
    unsigned int hash = 5381;
        int c;
        while ((c = *key++))
            hash = ((hash << 5) + hash) + c; 
        return hash % capacity;
}

//Returns the bucket entry of the given key if its found in the hashtable and NULL if its not
static Records *SearchBucketList(RecHash *hash_table, char *K, RecBucket *searched_bucket){
    //Traversing the list of buckets
    while(searched_bucket != NULL){
        //Traversing the array of RecBucketEntry pointers in each bucket
        for(int i=0; i < searched_bucket->count; i++){
            if(strcmp(K, searched_bucket->entries[i]->key) == 0){
                return searched_bucket->entries[i]->value;
            }
        }
        searched_bucket = searched_bucket->next;
    }
    return NULL;
}

static RecHash *Rehash(RecHash *old_hash, size_t bucket_size){
    //Creating new hashtable with double the capacity of the old one
    RecHash *new_hash = RecHashCreate(2 * old_hash->capacity);

    //Traversing every valid pair of key and value of the old hashtable and inserting them to the new one
    for(int i=0; i < old_hash->capacity; i++){

        RecBucket *curr = old_hash->table[i];
        RecBucket *next = NULL;
        while(curr != NULL){
            for(int j=0; j < curr->count; j++){
                char *curr_key = curr->entries[j]->key;
                Records *curr_value = curr->entries[j]->value;
                RecHashInsert(&new_hash, curr_key, curr_value, bucket_size);
                free(curr_key);
                free(curr->entries[j]);
            }
            free(curr->entries);
            next = curr->next;
            free(curr);
            curr = next;
        }
        
    }
    free(old_hash->table);
    free(old_hash);
    return new_hash;
}

//-----------------------------Definition of RecHash functions----------------------------------
//Takes 0 as argument if the hashtable holds AVL trees and 1 if it holds records
RecHash *RecHashCreate(size_t table_size){
    //Allocating memory for RecHash struct and its table of buckets
    RecHash *new_hash = malloc(sizeof(struct RecHash));
    new_hash->table = malloc(table_size * sizeof(RecBucket *));

    //Initialization of RecHash members
    new_hash->capacity = table_size;
    new_hash->used_entries = 0;

    //Initializing all the table entries with NULL
    for(int i=0; i < new_hash->capacity; i++){
        new_hash->table[i] = NULL;
    }
    
    return new_hash;
}

//Returns 0 if the inserted item already exists in the hashtable and 0 if it doesnt
void RecHashInsert(RecHash **hash_table, char *K, Records *item, size_t bucket_size){
    //Calculating the number of bucket entries that fit in each bucket considering the given bucket size
    int num_of_entries = (bucket_size - sizeof(RecBucket)) / sizeof(RecBucketEntry);

    //We need enough memory for at least one entry in each bucket
    if(num_of_entries < 1){
        printf("The given bucket size is not enough to host any bucket entries, give a bigger number\n");
        exit(EXIT_FAILURE);
    }
    //Calculating hashcode using the a statically declared hashfunction
    int hashed_index = Hashfunction(K, (*hash_table)->capacity);

    //If the entry already exists there is nothing else to do
    if(SearchBucketList(*hash_table, K, (*hash_table)->table[hashed_index]) != NULL) return;

    //Wrapping the key and the value in a RecBucketEntry struct
    RecBucketEntry *new_entry = malloc(sizeof(RecBucketEntry));
    new_entry->key = malloc(strlen(K) +1);
    strcpy(new_entry->key, K);
    new_entry->value = item;
    
    //In case of empty bucket
    if((*hash_table)->table[hashed_index] == NULL){
        //Allocating memory for new bucket and a containing array of entries
        RecBucket *new_bucket = malloc(sizeof(RecBucket));
        new_bucket->entries = malloc(num_of_entries * sizeof(RecBucketEntry *));

        new_bucket->entries[0] =  new_entry;
        new_bucket->count = 1;
        new_bucket->next = NULL;

        (*hash_table)->table[hashed_index] = new_bucket;
    //In case of existing bucket   
    }else{
        RecBucket *curr_bucket = (*hash_table)->table[hashed_index];
        
        //Get to the end of the bucket list (pun not intended)
        while(curr_bucket->next != NULL){
            curr_bucket = curr_bucket->next;
        }
        //If the bucket can take more entries we insert in the bucket's table
        if(curr_bucket->count < num_of_entries){
            //Inserting the new entry to the buckets table of entries
            curr_bucket->entries[curr_bucket->count] = new_entry;
            curr_bucket->count++;
        }else{ //In case the bucket is full we insert a new one into the list
        //and then insert the entry there
            curr_bucket->next = malloc(sizeof(RecBucket));
            curr_bucket = curr_bucket->next;
            curr_bucket->entries = malloc(num_of_entries * sizeof(RecBucketEntry *));

            curr_bucket->entries[0] = new_entry;
            curr_bucket->count = 1;
            curr_bucket->next = NULL;
        }
    }  
    (*hash_table)->used_entries++; 

    //Multiplying with 1.0 in order to get result of type double
    double load_factor = (1.0 * (*hash_table)->used_entries) / (*hash_table)->capacity;

    if(load_factor >= 0.9){
        // printf("\x1B[31m" "-The hashtable's load factor exceeded its limit.\n-Rehashing...\n" "\x1B[0m");
        *hash_table = Rehash(*hash_table, bucket_size);
    }
    return;
}

Records *RecHashGetItem(RecHash *hash, char *K){
    //Calculating hashcode using the a statically declared hashfunction
    int hashed_index = Hashfunction(K, hash->capacity);
    //Searching the buckets of the hashed table index for the item
    return SearchBucketList(hash, K, hash->table[hashed_index]);
}

void RecHashDestroy(RecHash *hash){
    for(int i=0; i < hash->capacity; i++){
        RecBucket *bckt = hash->table[i];
        RecBucket *next_bckt = NULL;
        //Traversing bucket list
        while(bckt != NULL){
        //Traversing the array of RecBucketEntry pointers in each bucket
        for(int i=0; i < bckt->count; i++){
            free(bckt->entries[i]->key);
            free(bckt->entries[i]->value);
            free(bckt->entries[i]);
        }

        free(bckt->entries);
        next_bckt = bckt->next;
        free(bckt);
        bckt = next_bckt;
        }
    }
    free(hash->table);
    free(hash);
}