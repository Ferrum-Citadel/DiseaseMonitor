#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "BH.h"

/*-------------------------Voithitikes static synartiseis-------------------------------------*/
/*Voithitiki static synartisi poy epistrefeu "true" ean ena node einai fyllo
kai "false" ean den einai*/
static bool IsLeaf(BHeap H, BHnode N){
    BHnode Lchild=CBTGetChildLeft(H, N);
    BHnode Rchild=CBTGetChildRight(H, N);
    if(CBTIsNil(H, Lchild) && CBTIsNil(H, Rchild)){
        return true;
    }else return false;
}

/*Voithitiki static synartisi poy antalassei ta item dyo komvwn*/
static void Swap(BHeap H, BHnode N1, BHnode N2){
    BHitem item1, item2;
    
    item1=CBTGetItem(H, N1);
    item2=CBTGetItem(H, N2);

    CBTChange(H, N1, item2);
    CBTChange(H, N2, item1);
}

/*Static synartisi poy ylopoiei ton algorithmo Heapify Up*/
static void HeapifyUp(BHeap H, BHnode N){

    if(N == CBTGetRoot(H)) return;
    BHitem item, pitem;
    BHnode parent;

    parent=CBTGetParent(H, N);
    item=CBTGetItem(H, N);
    pitem=CBTGetItem(H, parent);

   
    if(item->priority <= pitem->priority){
        return;
    }else {
        Swap(H, N, parent);
        HeapifyUp(H, parent);
    }
}

/*Static voithitiki synartisi i opoia eisagei se ena complete binary tree
ta item me ta priorities toys apo toys antistoixoys pinakes*/
static BHeap ImportToCBT(int maxelements, int *Priority_array, BHitemType *Data_array){
    BHeap H=BHCreate(maxelements);

    for(int i=0; i<maxelements; i++){
    BHitem new=malloc(sizeof(struct BHitemNode));
    new->priority = Priority_array[i];
    new->data = Data_array[i];
    CBTInsertLast(H, new);
    }
    return H;
}

/*Efoson ta item ta orizoume ws stuct pername tin parakatw synartisi
sta traversals gia na kanei free ton xwro poy desmeysame gia ta item*/
static void VisitFreeItem(BHeap H, BHnode N){
    BHitem item;
    item = CBTGetItem(H, N);
    free(item);
}

/*Static voithitiki synartisi i opoia ylopoiei ton algorithmo Heapify Down*/
static void HeapifyDown(BHeap H, BHnode N){

if(IsLeaf(H, N)) return;

BHnode Lchild=CBTGetChildLeft(H, N);
BHnode Rchild=CBTGetChildRight(H, N);
BHitem item=CBTGetItem(H, N);
/*Pernoyme ta item twn paidiwn afoy vevaiwthoume pws yparxoyn oi
antistoixoi nodes kai meta kanoyme tin sygrisi anamesa sta priorities toys*/
if(!CBTIsNil(H, Rchild)){
    BHitem Ritem=CBTGetItem(H, Rchild);
    if(item->priority < Ritem->priority){
        Swap(H, N, Rchild);
        HeapifyDown(H, Rchild);
    }
}else if(!CBTIsNil(H, Lchild)){
    BHitem Litem=CBTGetItem(H, Lchild);
    if(item->priority < Litem->priority){
        Swap(H, N, Lchild);
        HeapifyDown(H, Lchild);
    }
}
}

/*-------------------------Ylopoiisi synartisewn gia Binary Heap-------------------------------------*/

BHeap BHCreate(int maxelements){
    return CBTCreate(maxelements);
}

bool BHIsEmpty(BHeap H){
    BHnode root=CBTGetRoot(H);
    return CBTIsNil(H, root);
}

void BHRemove(BHeap H){
    /*Elegxos kenoy heap*/
    if(BHIsEmpty(H)){
        printf("Error: The heap is already empty!\n");
        return;
    }
    BHnode root=CBTGetRoot(H);
    BHnode last=CBTGetLast(H);

    Swap(H, root, last);
    CBTRemoveLast(H);

    HeapifyDown(H, root);
}

void BHInsert(BHeap H, int p, BHitemType I){
    /*Desmevoyme mnimi gia neo item kai toy dinoyme to data 
    kai to priority toy komvoy pros eisagwgi*/
   
    BHitem new = malloc(sizeof(struct BHitemNode));
    new->data = I;
    new->priority = p;
   
    /*Kanoyme eisagwgi sto telos kai efarmozoyme Heapify up apo to teleytaio node*/
    CBTInsertLast(H, new);
    BHnode last=CBTGetLast(H);
    HeapifyUp(H, last);
}

BHeap BHNaiveHeapify(int maxelements, int *Priority_array, BHitemType *Data_array){
    BHeap H=BHCreate(maxelements);

    for(int i=0; i<maxelements; i++){
    BHitem new=malloc(sizeof(struct BHitemNode));
    new->priority = Priority_array[i];
    new->data = Data_array[i];
    BHInsert(H, Priority_array[i], Data_array[i]);
    }
    return H;
}


BHeap BHHeapify(int maxelements, int *Priority_array, BHitemType *Data_array){
    
    BHeap H = ImportToCBT(maxelements, Priority_array, Data_array);
    CBTPostOrder(H, HeapifyDown);

    return H;
}

int BHGetMaxPriority(BHeap H){
    if(BHIsEmpty(H)){
        printf("Error: The heap is empty!\n");
        return -1;
    }else{
        BHnode root=CBTGetRoot(H);
        BHitem root_item=CBTGetItem(H, root);
        return root_item->priority;
    }
   

}

BHitemType BHGetMaxItem(BHeap H){
    if(BHIsEmpty(H)){
        printf("Error: The heap is empty!\n");
        return NULL;
    }else{
        BHnode root=CBTGetRoot(H);
        BHitem root_item=CBTGetItem(H, root);
        return root_item->data;
    }
}

/*I synartisi graftike gia na xrisimopoiithei apo tin VisitPrint poy orizetai stin main*/
BHitemType BHGetItemValue(BHeap H, BHnode N){
    BHitem item=CBTGetItem(H, N);
    return item->data;
}

int BHGetItemPriority(BHeap H, BHnode N){
    BHitem item=CBTGetItem(H, N);
    return item->priority;
}

/*I synartisi ftiaxtike wste na mporoyme na ektypwnoyme kapoio heap se levelored
pairnontas tin VisitPrint apo tin main*/
void BHLevelOrder(BHeap H, BHVisit Visit){
    CBTLevelOrder(H, Visit);
}

void BHDestroy(BHeap H){
    CBTLevelOrder(H, VisitFreeItem);
    CBTDestroy(H);
}













