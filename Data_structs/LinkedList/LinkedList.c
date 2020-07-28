#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include "LinkedList.h"

//-------------------Definition of linked list functions----------------------

//Creates new single linked list with a dummy node as head
LinkedList ListCreate(){
    return NULL;
}

//Destroys a list
void ListDestroy(LinkedList list){
    Lnode *temp = list;
    while(temp != NULL){
        Lnode *ahead = temp->next;         
        free(temp);
        temp = ahead;
    }
}


//Inserts node with a given string as data in the first position of the list
void ListInsertFirst(LinkedList *list, Records *new_data){
    Lnode *new = malloc(sizeof(Lnode));
    if(new == NULL) perror("malloc failed!");
    //data member points to the given new_data address.For records we allocate memory before inserting into list
    new->data = new_data;
    new->next = *list;
    *list = new;
    return;
}

Records *ListSearch(LinkedList list, char *id){
    Lnode *temp = list;
    while(temp != NULL){
        if(strcmp(temp->data->recordID, id) == 0) return temp->data;
        temp = temp->next;
    }
    return NULL;
}

int ListNodeCount(LinkedList list){
    Lnode *temp = list;
    int count = 0;
    while(temp != NULL){

        count++;
        temp = temp->next;
    }
    return count;
}

int ListNodeCountryCount(LinkedList list, char *country){
    Lnode *temp = list;
    int count = 0;
    while(temp != NULL){
        if(strcmp(temp->data->country, country) == 0) count++;
        temp = temp->next;
    }
    return count;
}

int ListDiseaseCount(LinkedList list, char *disease){
    Lnode *temp = list;
    int count = 0;
    while(temp != NULL){
        if(strcmp(temp->data->disease, disease) == 0) count++;
        temp = temp->next;
    }
    return count;
}

int ListStillInHospital(LinkedList list){
    Lnode *temp = list;
    int count = 0;
    struct tm zero_date;
    memset(&zero_date, 0, sizeof(struct tm));
    while(temp != NULL){
        if(memcmp(&temp->data->exitDate, &zero_date, sizeof(struct tm)) == 0){
            count++;
        }
        temp = temp->next;
    }
    return count;
}

