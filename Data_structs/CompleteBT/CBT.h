#pragma once 

#include "../Binarytree/BTree.h"
#include "../BinaryHeap/CBTitemType.h"


/*Ta complete dyadika dentra tha einai stin oysia
dentra opws ta orisame sto BT implementation*/
typedef BTree CBTree;
typedef BTnode CBTnode;
typedef BTVisit CBTVisit;

/*----------Dilwsi Synartisewn toy Complete Binary Tree------------------*/

CBTree CBTCreate(int);
void CBTInsertLast(CBTree, CBTitem);
int CBTHeight(CBTree);
int CBTSize(CBTree);
CBTnode CBTGetRoot(CBTree);
CBTnode CBTGetLast(CBTree);
CBTnode CBTGetParent(CBTree, CBTnode);
CBTnode CBTGetChildLeft(CBTree, CBTnode);
CBTnode CBTGetChildRight(CBTree, CBTnode);
bool CBTIsNil(CBTree, CBTnode);
CBTitem CBTGetItem(CBTree, CBTnode);
void CBTRemoveLast(CBTree);
void CBTChange(CBTree, CBTnode, CBTitem);
void CBTPreOrder(CBTree, CBTVisit);
void CBTInOrder(CBTree, CBTVisit);
void CBTPostOrder(CBTree, CBTVisit);
void CBTLevelOrder(CBTree, CBTVisit);
void CBTDestroy(CBTree);