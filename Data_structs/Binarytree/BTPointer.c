#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BTree.h"

/*Dimiourgia struct me pointers gia ta node tou dentrou*/
typedef struct TreeNode {
    BTitem data;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
} TreeNode;

/*Dimioyrgia TreeStruct poy periexei ton root komvo toy dentroy*/
struct TreeStruct {
    BTnode root;
};


/*-------------------------Voithitikes static synartiseis-------------------------------------*/

/*Voithitiki static synartisi i opoia mporei na leitoyrgisei anadromika*/
static int HeightRec(BTnode temp){
    
    /*Synthiki termatismoy*/
    if (temp == NULLnode){
        return -1;
    }
    /*Proxwrame ena epipedo katw sta paidia*/
    int LeftH=HeightRec(temp->left);
    int RightH=HeightRec(temp->right);

    /*Epistrefoyme to subtree me to megalytero height*/
    if(LeftH > RightH){
        return ++LeftH;
    }else {
        return ++RightH;
    }
}

/*Voithitiki static synartisi i opoia mporei na leitoyrgisei anadromika*/
static int SizeRec(BTnode temp){
    
    /*Synthiki termatismoy*/
    if(temp == NULLnode){
        return 0;
    }else {
        /*Prwxarame pros ta katw sto dentro*/
        int LeftS=SizeRec(temp->left);
        int RightS=SizeRec(temp->right);
        int res=LeftS+RightS+1;
        return res;
    }
}

/*Anadromiki static synartisi me anadromiki dynatotita*/
static void PreOrderRec(BTree T, BTnode N, BTVisit Visit){
    if(N != NULLnode){/*Den xrisimopoioume tin BTIsNil gia megalyteri taxytita*/
        Visit(T, N);
        PreOrderRec(T, N->left, Visit);
        PreOrderRec(T, N->right, Visit);
    }
}

/*Anadromiki static synartisi me anadromiki dynatotita*/
static void InOrderRec(BTree T, BTnode N, BTVisit Visit){
    if(N != NULLnode){/*Den xrisimopoioume tin BTIsNil gia megalyteri taxytita*/
        InOrderRec(T, N->left, Visit);
        Visit(T, N);
        InOrderRec(T, N->right, Visit);
    }
}

/*Anadromiki static synartisi me anadromiki dynatotita*/
static void PostOrderRec(BTree T, BTnode N, BTVisit Visit){
    if(N != NULLnode){ /*Den xrisimopoioume tin BTIsNil gia megalyteri taxytita*/
        PostOrderRec(T, N->left, Visit);
        PostOrderRec(T, N->right, Visit);
        Visit(T, N);
    }
}

/*Anadromiki static synartisi me anadromiki dynatotita*/
void static LevelOrderRec(BTree T, BTnode N, int h, BTVisit Visit){

    /*Synthiki termatismoy*/
    if (BTIsNil(T, N)){
        return;

    }else if(h == 0){

        Visit(T, N);
    }else if(h > 0){
        /*Epistrefoyme anadromika ena level kai episkeptomaste ta paidia toy node*/
        LevelOrderRec(T, N->left, h-1, Visit);
        LevelOrderRec(T, N->right, h-1, Visit);
    }
}

/*Voithitiki static synartisi gia anadromiki diagrafi komvwn*/
void static DeleteRec(BTnode N){
    
    if(N != NULLnode){

        DeleteRec(N->left);
        DeleteRec(N->right);
        free(N);
    }
}

/*-------------------------Ylopoiisi synartisewn me pointers gia Binary Tree-------------------------------------*/

/*Voithitiki static synartisi gia tin dimioyrgia komvwn*/
static BTnode NewNode(BTnode current, BTitem item){

    BTnode new=malloc(sizeof(TreeNode));
    new->data=item;
    new->left = new->right=NULLnode;
    new->parent=current;
    return new;
}

BTree BTCreate(int maxelements){
    /*Desmeysi mnimis gia to struct tou dentroy kai arxikopoiisi*/
    BTree T=malloc(sizeof(struct TreeStruct));
    T->root=NULLnode;
    return T;
}

void BTInsertRoot(BTree T, BTitem I){
    /*Pername NULLnode ws parent toy root komvoy*/
    if(T->root == NULLnode){
        T->root=NewNode(NULLnode, I);
        
    }else {
        printf("Error: Root node already exists!\n");
    }
}

void BTInsertLeft(BTree T, BTnode N, BTitem I){
    /*Ean to aristero paidi den yparxei proxwrame se eisagwgi*/
    if((N->left) == NULLnode){
        N->left=NewNode(N, I);
        
    }else {
        printf("Error: Left child already exists\n");
    }
}

void BTInsertRight(BTree T, BTnode N, BTitem I){
    /*Ean to deksio paidi den yparxei proxwrame se eisagwgi*/
    if((N->right) == NULLnode){
        N->right=NewNode(N, I);
        
    }else {
        printf("Error: Left child already exists\n");
        return;
    }
}


int BTHeight(BTree T){
    /*Dinoyme ton root stin anadromiki synartisi*/
    return (HeightRec(T->root));
}


int BTSize(BTree T){
    /*Dinoyme ton root stin anadromiki synartisi*/
    return (SizeRec(T->root));
}

bool BTIsNil(BTree T, BTnode N){

    if(N == NULLnode) return true;
    else return false;
}

BTnode BTGetRoot(BTree T){
    /*Afinete sto xeri toy xristi symfwna me tin ekfwnisi o elegxos gia NULLnode*/
    return T->root;
}

BTnode BTGetParent(BTree T, BTnode N){

    /*Elegxos gia to an o N einai o root*/
    if(N->parent == NULLnode){
        printf("Error: The given node is the root!\n");
        return NULLnode;
    }else {
        return N->parent;
    }
}

BTnode BTGetChildLeft(BTree T, BTnode N){
    /*Afinete sto xeri toy xristi symfwna me tin ekfwnisi o elegxos gia NULLnode*/
    return N->left;   
}

BTnode BTGetChildRight(BTree T, BTnode N){
    /*Afinete sto xeri toy xristi symfwna me tin ekfwnisi o elegxos gia NULLnode*/
    return N->right;
}


/*Epistrefei "-1" se periptwsi poy den yparxei o komvos poy dothike*/
BTitem BTGetItem(BTree T, BTnode N){

    /*Elegxos gia NULLnode node*/
    if (BTIsNil(T, N)){
        printf("Error: Trying to access NIL node!\n");
        return NULLitem;
    }else {
        return N->data;
    }
}

/*I Synartisi exei prosarmostei wste na diagrafei kai nodes me 1 paidi*/
void BTRemove(BTree T, BTnode N){
    /*Elegxos yparksis N*/
    if(BTIsNil(T, N)){
        printf("The given node doesnt't exist\n");
        return;
    }
    /*Diagrafi an to N einai leaf*/
    if(BTIsNil(T, N->left) && BTIsNil(T, N->right)){
        
        if(N->parent->left == N){
            N->parent->left=NULLnode;
        }else {
            N->parent->right=NULLnode;
        }
        free(N);
    /*Diagrafi node me 1 paidi*/
    }else if(BTIsNil(T, N->left) || BTIsNil(T, N->right)) {
    
        if(N->parent->left == N){
            if(!BTIsNil(T, N->left)){
                N->parent->left = N->left;
                N->left->parent = N->parent;
            }else {
                N->parent->left = N->right;
                N->right->parent = N->parent;
            }
        }else if(N->parent->right == N){
            if(!BTIsNil(T, N->left)){
                N->parent->right = N->left;
                N->left->parent = N->parent;
            }else {
                N->parent->right = N->right;
                N->right->parent = N->parent;
            }
        }
        
        free(N);
    }else {
        printf("Error: The given node has 2 childen! Why would you kill it? ಠ_ಠ\n");
    }
}

void BTChange(BTree T, BTnode N, BTitem I){

    /*Elegxos gia egyro komvo*/
    if(BTIsNil(T, N)){
        printf("Error: The given node doesn't exist!\n");
    }else {
        N->data=I;
    }
}


void BTPreOrder(BTree T, BTVisit Visit){
    
    if(BTIsNil(T, T->root)){
        printf("Error: The tree is empty!\n");
    }else PreOrderRec(T, T->root, Visit);
}


void BTInOrder(BTree T, BTVisit Visit){

   if(BTIsNil(T, T->root)){
        printf("Error: The tree is empty!\n");
    }else InOrderRec(T, T->root, Visit);
}


void BTPostOrder(BTree T, BTVisit Visit){

    if(BTIsNil(T, T->root)){
        printf("Error: The tree is empty!\n");
    }else PostOrderRec(T, T->root, Visit);
}


void BTLevelOrder(BTree T, BTVisit Visit){

    /*Elegxos gia keno dentro*/
    if(BTIsNil(T,T->root)){
        printf("The tree is empty!");
        return;
    }
    int height = BTHeight(T);

    /*Me to i metrame se poio level toy dentroy eimaste*/
    for(int i=0; i<=height; i++){
        LevelOrderRec(T, T->root, i, Visit);
    }

}

void BTDestroy(BTree T){
    
    if(BTIsNil(T, T->root)){
        printf("Destroying empty tree\n");
    }
    DeleteRec(T->root);
    free(T);
}
















