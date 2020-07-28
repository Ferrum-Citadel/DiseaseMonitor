#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "CBT.h"

/*-------------------------Voithitikes static synartiseis-------------------------------------*/

/*Static voithitiki synartisi poy epistrefei true ean enas komvos einai aristero paidi
kai false ean den einai*/
static bool IsLeft(CBTree T, CBTnode N){
    CBTnode parent=CBTGetParent(T, N);
    if(CBTGetChildLeft(T, parent) == N) return true;
    else return false;

}

/*Static voithitiki synartisi i opoia  diasxizei ta aristera paidia me afetiria enan dedomeno
komvo mexri na synantisi NIL*/
static CBTnode GoLeft(CBTree T, CBTnode N){
    CBTnode Left;
    Left=CBTGetChildLeft(T, N);
    while(!CBTIsNil(T, Left)){
        N=Left;
        Left=CBTGetChildLeft(T, N);
    }
    return N;
}

/*Static voithitiki synartisi stin opoia dinontai mono aristera paidia
kai epistrefei to antistixo deksi */
static CBTnode RightSibling(CBTree T, CBTnode N){
    
    CBTnode parent=CBTGetParent(T, N);
    return CBTGetChildRight(T, parent);
}

/*-------------------------Ylopoiisi synartisewn gia Complete Binary Tree-------------------------------------*/

CBTree CBTCreate(int maxelements){
    return BTCreate(maxelements);
}

CBTnode CBTGetRoot(CBTree T){
    return BTGetRoot(T);
}

CBTnode CBTGetParent(CBTree T, CBTnode N){
    return BTGetParent(T, N);
}

CBTnode CBTGetChildLeft(CBTree T, CBTnode N){
    return BTGetChildLeft(T, N);
}

CBTnode CBTGetChildRight(CBTree T, CBTnode N){
    return BTGetChildRight(T, N);
}

CBTitem CBTGetItem(CBTree T, CBTnode N){
    return BTGetItem(T, N);
}

bool CBTIsNil(CBTree T, CBTnode N){
    return BTIsNil(T, N);
}

void CBTChange(CBTree T, CBTnode N, CBTitem I){
    BTChange(T, N, I);
}

int CBTHeight(CBTree T){
    return BTHeight(T);
}

int CBTSize(CBTree T){
    return BTSize(T);
}

void CBTPreOrder(CBTree T, CBTVisit Visit){
    BTPreOrder(T, Visit);
}

void CBTInOrder(CBTree T, CBTVisit Visit){
    BTInOrder(T, Visit);
}

void CBTPostOrder(CBTree T, CBTVisit Visit){
    BTPostOrder(T, Visit);
}

void CBTLevelOrder(CBTree T, CBTVisit Visit){
    BTLevelOrder(T, Visit);
}


void CBTInsertLast(CBTree T, CBTitem I){
    /*Ean to dentro einai keno, eisagwgoume root*/
    CBTnode root=CBTGetRoot(T);
    if (CBTIsNil(T, root)){
        BTInsertRoot(T, I);
        return;
    }

    int size=CBTSize(T);
    int height=CBTHeight(T);
    /*Ean to dentro einai teleio,diladi me gemato kai to teleytaio level,
    kanoyme eisagwgi sto aristero paidi toy left most komvoy*/
   if(size == pow(2,height+1) - 1){
        CBTnode temp=root;
        temp=GoLeft(T, temp);
        BTInsertLeft(T, temp, I);
        return;
    /*Ean to dentro exei zygo arithmo stoixeiwn,simainei pws to teleytaio node einai aristero paidi
    ara kanoyme eisagwgi sta deksia*/
    }else if(size % 2 == 0){
        CBTnode last=CBTGetLast(T);
        CBTnode last_parent=CBTGetParent(T, last);
        BTInsertRight(T, last_parent, I);
        return;
        
    /*Alliws diasxizoyme pros ta epanw mexri na synantisoyme kapoio aristero paidi i to root,
    stin synexeia metapidame sto antistoixo deksio paidi kai diasxizoume ta aristera paidia pros ta katw*/
    }else {
        CBTnode temp=CBTGetLast(T);
        while(!IsLeft(T, temp) && temp != root){
            temp=CBTGetParent(T, temp);
        }

        if(IsLeft(T, temp)){
            temp=RightSibling(T, temp);
        }

        temp=GoLeft(T, temp);

        BTInsertLeft(T, temp, I);

    }
}


/*!I synartisi exei ylopoiithei exontas ypopsin oti se ena complete binary tree, otan metatrepoyme to size
stin dyadiki toy morfi kai afairesoyme to prwto psifio, tote akolouthontas ton kanona na kinoymaste deksia gia ta psifia
me timi "1" kai aristera gia ta psifia me timi "0", ksekinontas apo ton root, ftanoyme sto teleytaio node!*/
CBTnode CBTGetLast(CBTree T){
    /*Elegxos gia keno dentro*/
    if (CBTIsNil(T, CBTGetRoot(T))){
        printf("Error: The tree is empty\n");
        return NULLnode;
    }

    CBTnode root=CBTGetRoot(T);
    int size=CBTSize(T);
    int temp=size;

    /*Ean to dentro exei ena stoixeio epistrefoume to root*/
    if(size == 1){
        return root;
    }

    /*afairoyme to prwto psifio apo tin dyadiki morfi toy size*/
    while (temp & (temp - 1)){
        temp &= temp - 1;   
    }
    temp >>=1;

    /*Synthiki termatismoy*/
    while (temp) {
        /*Bitwise elegxos*/
        if (size & temp) {
            if (!CBTIsNil(T, CBTGetChildRight(T, root))){
                root = CBTGetChildRight(T, root);
            }else{
                printf("Error: Out of bounds!\n");
                return NULLnode;
            }
        }else {
            if (!CBTIsNil(T, CBTGetChildLeft(T, root))){
                    root = CBTGetChildLeft(T, root);
            }else{
                printf("Error: Out of bounds!\n");
                return NULLnode;
            }

        }

    /* Epomeno vima*/
    temp >>= 1;
    }

    
    return root;

}

void CBTRemoveLast(CBTree T){
    CBTnode last=CBTGetLast(T);
    if(!CBTIsNil(T, last)){
        BTRemove(T, last);
    }
}

void CBTDestroy(BTree T){
    
    BTDestroy(T);
}


