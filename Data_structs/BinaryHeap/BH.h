#pragma once

#include "../CompleteBT/CBT.h"
#include "../BHitemType.h"



typedef CBTree BHeap;
typedef CBTnode BHnode;
typedef CBTVisit BHVisit;


/*----------------------Dilwsi Synartisewn toy Complete Binary Tree------------------*/

BHeap BHCreate(int);
bool BHIsEmpty(BHeap);
int BHGetMaxPriority(BHeap);
BHitemType BHGetMaxItem(BHeap);
BHitemType BHGetItemValue(BHeap, BHnode);
int BHGetItemPriority(BHeap H, BHnode N);
void BHRemove(BHeap);
void BHInsert(BHeap, int, BHitemType);
BHeap BHNaiveHeapify(int , int *, BHitemType *);
BHeap BHHeapify(int, int *, BHitemType *);
void BHDestroy(BHeap);
void BHLevelOrder(BHeap, BHVisit);