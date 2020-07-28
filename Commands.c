#define _XOPEN_SOURCE 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <time.h>
#include "Commands.h"
#include "./Data_structs/BinaryHeap/BH.h"

//---------------------------------Static helper functions------------------------------------------------

//Prints Disease cases reported in the range of the given dates
static int DiseaseStatsRecurse(TreeNode *node, struct tm date1, struct tm date2){
    
    if(node == NULL) return 0;

    int res1 = CompareDates(date1, node->records_list->data->entryDate);
    int res2 = CompareDates(date2, node->records_list->data->entryDate);

    if(res1 <= 0 && res2 >= 0){
        return DiseaseStatsRecurse(node->left, date1, date2) + DiseaseStatsRecurse(node->right, date1, date2) + ListNodeCount(node->records_list);;
    }else if(res1 > 0){
        return DiseaseStatsRecurse(node->right, date1,date2);
    }else{
        return DiseaseStatsRecurse(node->left, date1, date2);
    }
}

//Prints Disease cases reported in the range of the given dates
static int CountCountriesInTreeDates(TreeNode *node, struct tm date1, struct tm date2, char* country){
    
    if(node == NULL) return 0;

    int res1 = CompareDates(date1, node->records_list->data->entryDate);
    int res2 = CompareDates(date2, node->records_list->data->entryDate);

    if(res1 <= 0 && res2 >= 0){
        return CountCountriesInTreeDates(node->left, date1, date2, country) + CountCountriesInTreeDates(node->right, date1, date2, country) + ListNodeCountryCount(node->records_list, country);
    }else if(res1 > 0){
        return CountCountriesInTreeDates(node->right, date1,date2, country);
    }else{
        return CountCountriesInTreeDates(node->left, date1, date2, country);
    }
}

static int CountDiseasesInTreeDates(TreeNode *node, struct tm date1, struct tm date2, char* disease){
    if(node == NULL) return 0;

    int res1 = CompareDates(date1, node->records_list->data->entryDate);
    int res2 = CompareDates(date2, node->records_list->data->entryDate);

    if(res1 <= 0 && res2 >= 0){
        return CountDiseasesInTreeDates(node->left, date1, date2, disease) + CountDiseasesInTreeDates(node->right, date1, date2, disease) + ListDiseaseCount(node->records_list, disease);
    }else if(res1 > 0){
        return CountDiseasesInTreeDates(node->right, date1,date2, disease);
    }else{
        return CountDiseasesInTreeDates(node->left, date1, date2, disease);
    }
}

static int CountCoutriesInTree(TreeNode *node, char *country){
    if(node == NULL) return 0;
    return CountCoutriesInTree(node->left, country) + CountCoutriesInTree(node->right, country) + ListNodeCountryCount(node->records_list, country);
}

static int CountDiseasesInTree(TreeNode *node, char *disease){
    if(node == NULL) return 0;
    return CountDiseasesInTree(node->left, disease) + CountDiseasesInTree(node->right, disease) + ListDiseaseCount(node->records_list, disease);
}

int static StillInHospital(TreeNode *node){
    if(node == NULL) return 0;
    return StillInHospital(node->left) + StillInHospital(node->right) + ListStillInHospital(node->records_list);
}


//---------------------------------Command functions------------------------------------------------

void globalDiseaseStats(char *date1_str, char *date2_str, Hashtable *disease_hash){
    //If the dates are passed as NULL we implement the no-parameter code
    if(date1_str == NULL){
        //Traversing hashtable table
        for(int i=0; i < disease_hash->capacity; i++){
            Bucket *bckt = disease_hash->table[i];
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    printf("%s %d\n", bckt->entries[j]->key, bckt->entries[j]->value->count);
                }   
                bckt = bckt->next;
            }
        }
    }else{
        struct tm date1,date2;
        memset(&date1, 0, sizeof(struct tm));
        memset(&date2, 0, sizeof(struct tm));
        
        //Inserting dates into tm structs
        strptime(date1_str, "%d-%m-%Y", &date1);
        strptime(date2_str, "%d-%m-%Y", &date2);

        //Traversing hashtable table
        for(int i=0; i < disease_hash->capacity; i++){
            Bucket *bckt = disease_hash->table[i];
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    AVLTree *disease_tree =  bckt->entries[j]->value;
                    int count = DiseaseStatsRecurse(disease_tree->root, date1, date2);
                    printf("%s %d\n", bckt->entries[j]->key, count);
                }   
                bckt = bckt->next;
            }
        }
    }
}

void diseaseFrequency(char *virusName, char *date1_str, char *date2_str, char *countryName, Hashtable *disease_hash){
    struct tm date1,date2;
    memset(&date1, 0, sizeof(struct tm));
    memset(&date2, 0, sizeof(struct tm));
    
    //Inserting dates into tm structs
    strptime(date1_str, "%d-%m-%Y", &date1);
    strptime(date2_str, "%d-%m-%Y", &date2);
    //In case no country parameter was given
    if(countryName == NULL){
        AVLTree *disease_tree = HashGetItem(disease_hash, virusName);
        int res = DiseaseStatsRecurse(disease_tree->root, date1, date2);
        printf("%s %d\n", virusName, res);
    }else{
        AVLTree *disease_tree = HashGetItem(disease_hash, virusName);
        int res = CountCountriesInTreeDates(disease_tree->root, date1, date2, countryName);
        printf("%s %d\n", virusName, res);
    }
}


void topkDiseases(int k, char *countryName, char *entryDate_str, char * exitDate_str, Hashtable *disease_hash){
    BHeap heap = BHCreate(disease_hash->used_entries);
    if(entryDate_str == NULL){
       
        for(int i=0; i < disease_hash->capacity; i++){
            Bucket *bckt = disease_hash->table[i];
            Bucket *next_bckt = NULL;
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    int cases = CountCoutriesInTree(bckt->entries[j]->value->root, countryName);
                    BHInsert(heap, cases, bckt->entries[j]->key);
                }

                bckt = next_bckt;
            }
        }
    }else{
        struct tm date1,date2;
        memset(&date1, 0, sizeof(struct tm));
        memset(&date2, 0, sizeof(struct tm));
        
        //Inserting dates into tm structs
        strptime(entryDate_str, "%d-%m-%Y", &date1);
        strptime(exitDate_str, "%d-%m-%Y", &date2);

        for(int i=0; i < disease_hash->capacity; i++){
            Bucket *bckt = disease_hash->table[i];
            Bucket *next_bckt = NULL;
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    int cases = CountCountriesInTreeDates(bckt->entries[j]->value->root, date1, date2, countryName);
                    BHInsert(heap, cases, bckt->entries[j]->key);
                }

                bckt = next_bckt;
            }
        }
    }

    for(int i=0; i<k; i++){
        printf("%s %d\n", BHGetMaxItem(heap), BHGetMaxPriority(heap));
        BHRemove(heap);
    }
    BHDestroy(heap);
    
}

void topkCountries(int k, char *diseaseName, char *entryDate_str, char * exitDate_str, Hashtable *country_hash){
    BHeap heap = BHCreate(country_hash->used_entries);
    
    if(entryDate_str == NULL){
        for(int i=0; i < country_hash->capacity; i++){
            Bucket *bckt = country_hash->table[i];
            Bucket *next_bckt = NULL;
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    int cases = CountDiseasesInTree(bckt->entries[j]->value->root, diseaseName);
                    BHInsert(heap, cases, bckt->entries[j]->key);
                    
                }

                bckt = next_bckt;
            }
        }
    }else{
        struct tm date1,date2;
        memset(&date1, 0, sizeof(struct tm));
        memset(&date2, 0, sizeof(struct tm));
        
        //Inserting dates into tm structs
        strptime(entryDate_str, "%d-%m-%Y", &date1);
        strptime(exitDate_str, "%d-%m-%Y", &date2);

        for(int i=0; i < country_hash->capacity; i++){
            Bucket *bckt = country_hash->table[i];
            Bucket *next_bckt = NULL;
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    int cases = CountDiseasesInTreeDates(bckt->entries[j]->value->root, date1, date2, diseaseName);
                    BHInsert(heap, cases, bckt->entries[j]->key);
                    
                }

                bckt = next_bckt;
            }
        }
    }
    
    for(int i=0; i<k; i++){
        printf("%s %d\n", BHGetMaxItem(heap), BHGetMaxPriority(heap));
        BHRemove(heap);
    }
    BHDestroy(heap);
    
}

void numCurrentPatients(char *diseaseName, Hashtable *disease_hash){
    if(diseaseName == NULL){
        for(int i=0; i < disease_hash->capacity; i++){
            Bucket *bckt = disease_hash->table[i];
            //Traversing bucket list
            while(bckt != NULL){
                //Traversing the array of BucketEntry pointers in each bucket
                for(int j=0; j < bckt->count; j++){
                    int res = StillInHospital(bckt->entries[j]->value->root);
                    printf("%s %d\n",bckt->entries[j]->key, res);
                }

                bckt = bckt->next;
            
            }
        }
    }else{
        AVLTree *disease_tree = HashGetItem(disease_hash, diseaseName);
        int res = StillInHospital(disease_tree->root);
        printf("%s %d\n",diseaseName, res);
    }
}

