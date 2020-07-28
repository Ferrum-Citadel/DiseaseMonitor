#pragma once
#include "../LinkedList/LinkedList.h"

//Treenode struct
typedef struct TreeNode{
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
    int height_left;
    int height_right;
    //In case of duplicate we populate the list, else the list is null and count=0
    LinkedList records_list;
} TreeNode;

//AVL Tree struct
typedef struct AVLTree {
    TreeNode *root;
    int count;
} AVLTree;

//----------------------------Declaration of AVL Tree functions------------------------

AVLTree *AvlCreate();
void AvlDestroy(AVLTree*);
void AvlInsert(AVLTree *, Records *);
int CompareDates(struct tm, struct tm);
