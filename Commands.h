#pragma once
#include "./Data_structs/Hashtable/RecHash.h"

void globalDiseaseStats(char *date1, char *date2, Hashtable *disease_hash);
void diseaseFrequency(char *virusName, char *date1_str, char *date2_str, char *countryName, Hashtable *disease_hash);
void topkDiseases(int k, char *countryName, char *entryDate_str, char * exitDate_str, Hashtable *disease_hash);
void topkCountries(int k, char *countryName, char *entryDate_str, char * exitDate_str, Hashtable *country_hash);
void numCurrentPatients(char *diseaseName, Hashtable *disease_hash);
