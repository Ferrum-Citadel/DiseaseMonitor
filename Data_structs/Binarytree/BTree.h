
#pragma once
#include <stdbool.h>
#include "../CompleteBT/BTitemType.h"


/*Afou ta nodes tha einai pointer se struct orizoyme ayti tin timi gia anaparastasi NIL*/
#define NULLnode NULL
/*O typos komvoy poy tha xrisimopoiei o xristis
einai stin oysia deiktis se komvo*/
typedef struct TreeNode *BTnode;

/*BTree orizoyme enan deikti se struct TreeStruct
i opoia orizetai sto BTPointer*/
typedef struct TreeStruct *BTree;



typedef void (*BTVisit)(BTree, BTnode);

/*----------------------Dilwsi Synartisewn toy Binary Tree------------------*/

BTree BTCreate(int);
void BTInsertRoot(BTree, BTitem);
void BTInsertLeft(BTree, BTnode, BTitem);
void BTInsertRight(BTree, BTnode, BTitem);
int BTHeight(BTree);
int BTSize(BTree);
BTnode BTGetRoot(BTree);
BTnode BTGetParent(BTree, BTnode);
BTnode BTGetChildLeft(BTree, BTnode);
BTnode BTGetChildRight(BTree, BTnode);
bool BTIsNil(BTree, BTnode);
BTitem BTGetItem(BTree, BTnode);
void BTRemove(BTree, BTnode);
void BTChange(BTree, BTnode, BTitem);
void BTPreOrder(BTree, BTVisit);
void BTInOrder(BTree, BTVisit);
void BTPostOrder(BTree, BTVisit);
void BTLevelOrder(BTree, BTVisit);
void BTDestroy(BTree);


