#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "AVL.h"

//-------------------------------Definition of static helper funcitons-------------------

static TreeNode *CreateNode(){
    TreeNode *new_node = malloc(sizeof(TreeNode));
    if(new_node == NULL) perror("malloc failed!");

    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->height_left = 0;
    new_node->height_right = 0;
    new_node->records_list = ListCreate();
    return new_node;
}

static int GetMaxHeight(TreeNode *node){
    int h;
    if(node->height_left > node->height_right){
        h = node->height_left;
    }else{
        h = node->height_right;
    }
    return h;
} 
static void UpdateHeight(TreeNode *node){
    node->height_left = 0;
    node->height_right = 0;
    if(node->left != NULL){
        node->height_left = GetMaxHeight(node->left) + 1;
    }
    if(node->right != NULL){
        node->height_right = GetMaxHeight(node->right) + 1;
    }
}

static void RotateLeft(AVLTree *tree, TreeNode *node){
    TreeNode *right = node->right;
    //In case of the pivot is root
    if(node == tree->root){
        tree->root = right;
     //In case its left child
    }else if(node == node->parent->left){
        node->parent->left = right;
    }else{
        node->parent->right = right;
    }
    right->parent = node->parent;
    if(right->left != NULL){
        node->right = right->left;
        node->right->parent = node;
    }else{
        node->right = NULL;
    }
    right->left = node;
    node->parent = right;
    UpdateHeight(node);
    UpdateHeight(right);
}

static void RotateRight(AVLTree *tree, TreeNode *node){
    TreeNode *left = node->left;
    if(node == tree->root){
        tree->root = left;
    }else if(node == node->parent->left){
        node->parent->left = left;
    }else{
        node->parent->right = left;
    }
    left->parent = node->parent;
    if(left->right !=NULL){
        node->left = left->right;
        node->left->parent = node;
    }else{
        node->left = NULL;
    }
    left->right = node;
    node->parent = left;
    UpdateHeight(node);
    UpdateHeight(left);
}

static int GetBalanceFactor(TreeNode *node){
    return ((node->height_left) - (node->height_right));
}

static void Balance(AVLTree *tree, TreeNode *node){
    TreeNode *curr = node;
    while(curr != NULL){
        TreeNode *parent = curr->parent;
        int balance_factor;
        UpdateHeight(curr);
        balance_factor = GetBalanceFactor(curr);
        
        if(balance_factor == -2){
            int right_bf = GetBalanceFactor(curr->right);
            if(right_bf < 0){
                RotateLeft(tree, curr);
            }else{
                RotateRight(tree, curr->right);
                RotateLeft(tree, curr);
            }
        }else if(balance_factor == 2){
            int left_bf = GetBalanceFactor(curr->left);
            if(left_bf >0){
                RotateRight(tree,curr);
            }else{
                RotateLeft(tree, curr->left);
                RotateRight(tree, curr);
            }
        }
        curr = parent;
    }
}
static void DestroyRec(TreeNode *node){
    if(node->left != NULL){
        DestroyRec(node->left);
    }
    if(node->right != NULL){
        DestroyRec(node->right);
    }
    ListDestroy(node->records_list);
    free(node);
}
//------------------------------------Definition of AVL Tree functions--------------------------------

//Returns -1 if a < b, returns 1 if a > b and 0 if they are the same
int CompareDates(struct tm a, struct tm b){
    time_t date1 = mktime(&a);
    time_t date2 = mktime(&b);

    double difference = difftime(date1, date2);

    if(difference < 0){
        return -1;
    }else if( difference > 0){
        return 1;
    }else {
        return 0;
    }
}



void AvlInsert(AVLTree *tree, Records *new_record){
    assert(tree);
    tree->count++;
    TreeNode *new_node = CreateNode();

    if(tree->root == NULL){
        tree->root = new_node;
        ListInsertFirst(&new_node->records_list, new_record);
        return;
    }

    TreeNode *traverse, *trail = NULL;
    traverse = tree->root;
    
    //Traversing the tree until we find the fitting place for the new node
    while(traverse != NULL){
        trail = traverse;
        int res = CompareDates(new_record->entryDate, traverse->records_list->data->entryDate);
        if( res == -1){
            traverse = traverse->left;
        }else if(res == 1){
            traverse = traverse->right;
        }else{ //If the dates match we add the new_record to the node's record list
            ListInsertFirst(&traverse->records_list, new_record);
            ListDestroy(new_node->records_list);
            free(new_node);
            return;
        }
    }

    ListInsertFirst(&new_node->records_list, new_record);
    int res = CompareDates(new_record->entryDate, trail->records_list->data->entryDate);
    if(res < 0){
        trail->left = new_node;
    }else{
        trail->right = new_node;
    }
    new_node->parent = trail;
    Balance(tree, new_node);
}



void AvlDestroy(AVLTree *tree){
    if(tree->root != NULL){
        DestroyRec(tree->root);
    }
    free(tree);
}


AVLTree *AvlCreate(){
    AVLTree *new_tree = malloc(sizeof(AVLTree));

    new_tree->root = NULL;
    new_tree->count = 0;
    return new_tree;
}
