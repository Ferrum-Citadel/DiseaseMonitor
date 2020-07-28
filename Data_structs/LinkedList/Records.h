#pragma once

#include<time.h>

typedef struct Records{
    char recordID[50];
    char patientFirstName[50];
    char patientLastName[50];
    char disease[50];
    char country[50];
    //We save dates in broken down time structs and output them in the DD-MM-YYYY format
    struct tm entryDate; 
    struct tm exitDate;
} Records;

