#pragma once

#include "../BHitemType.h"

typedef struct BHitemNode{
    BHitemType data;
    int priority;
} *BHitem;

/*Dilwsi timis poy tha xrisimopoieite gia NIL item*/
#define NULLitem NULL

/*To dentro mas tha apothikeyei int*/
typedef BHitem CBTitem;




