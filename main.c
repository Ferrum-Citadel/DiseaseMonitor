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

int main(int argc, char **argv)
{
    int options, disease_entries, country_entries, bucket_size;
    char *records_file, *id, *first_name, *last_name, *virus, *country, *entry_str, *exit_str;

    FILE *fp;

    //Parsing main's arguments using POSIX's getopt_long_only
    static struct option long_options[] = {
        {"h1", required_argument, NULL, '1'},
        {"h2", required_argument, NULL, '2'},
        {0, 0, NULL, 0}};

    int long_index = 0;
    while ((options = getopt_long_only(argc, argv, ":p:1:2:b:", long_options, &long_index)) != -1)
    {
        switch (options)
        {
        case 'p':
            //We allocate one extra char for the null terminator
            records_file = malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(records_file, optarg);
            //If the next string starts with "-" its an option and we missed an option value
            if (optarg[0] == '-')
            {
                printf("-One or more values are missing from the arguments!\n");
                printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
                exit(EXIT_FAILURE);
            }
            break;

        case '1':
            disease_entries = (unsigned int)strtol(optarg, NULL, 10);
            //If the next string starts with "-" its an option and we missed an option value
            if (optarg[0] == '-')
            {
                printf("-One or more values are missing from the arguments!\n");
                printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
                exit(EXIT_FAILURE);
            }
            break;

        case '2':
            country_entries = (unsigned int)strtol(optarg, NULL, 10);
            //If the next string starts with "-" its an option and we missed an option value
            if (optarg[0] == '-')
            {
                printf("-One or more values are missing from the arguments!\n");
                printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
                exit(EXIT_FAILURE);
            }
            break;

        case 'b':
            bucket_size = (unsigned int)strtol(optarg, NULL, 10);
            //If the next string starts with "-" its an option and we missed an option value
            if (optarg[0] == '-')
            {
                printf("-One or more values are missing from the arguments!\n");
                printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
                exit(EXIT_FAILURE);
            }
            break;

        case ':':
            printf("-One or more values are missing from the arguments!\n");
            printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
            exit(EXIT_FAILURE);

        case '?':
            printf("-Unknown arguments were given!\n");
            printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
            printf("OPTARG: %s\n", optarg);
            exit(EXIT_FAILURE);
        }
    }

    if (argc != 9)
    {
        printf("Wrong number of arguments\n");
        printf("-The program will now exit, The correct argument format is: ./diseaseMonitor -p (value) -h1 (value) -h2 (value) -b (value).\n");
        exit(EXIT_FAILURE);
    }

    //Opening file given by option -p
    fp = fopen(records_file, "r");
    if (fp == NULL)
        perror("fopen failed!");

    char *line = NULL;
    size_t len = 0;
    //Counting the file's number of lines
    size_t count = 0;
    while ((getline(&line, &len, fp)) != -1)
    {
        count++;
    }
    rewind(fp);

    //Creating Hashtable for the patients records
    RecHash *records_hash = RecHashCreate(count);
    //Creating Hashtable for diseases
    Hashtable *disease_hash = HashCreate(disease_entries);
    //Creating Hashtable for countries
    Hashtable *country_hash = HashCreate(country_entries);

    //Definition and initialization of date structs
    struct tm entry_date, exit_date;
    memset(&entry_date, 0, sizeof(struct tm));
    memset(&exit_date, 0, sizeof(struct tm));

    size_t get = getline(&line, &len, fp);
    while (get != -1)
    {
        char *token = strtok(line, " ");
        id = token;

        token = strtok(NULL, " ");
        first_name = token;

        token = strtok(NULL, " ");
        last_name = token;

        token = strtok(NULL, " ");
        virus = token;

        token = strtok(NULL, " ");
        country = token;

        token = strtok(NULL, " ");
        entry_str = token;

        token = strtok(NULL, " ");
        exit_str = token;

        //Inserting dates into tm structs
        strptime(entry_str, "%d-%m-%Y", &entry_date);
        strptime(exit_str, "%d-%m-%Y", &exit_date);

        if (exit_str[0] != '-')
        {

            time_t time_entry = mktime(&entry_date);
            time_t time_exit = mktime(&exit_date);
            double difference = difftime(time_entry, time_exit);
            if (difference > 0)
            {
                printf("Patient record with record id: %s, is rejected because the exit date is older than the entry date.\n", id);
                get = getline(&line, &len, fp);
                continue;
            }
        }
        //Creating the record to be inserted into the hashtable
        Records *patient = malloc(sizeof(Records));
        strcpy(patient->country, country);
        strcpy(patient->disease, virus);
        strcpy(patient->patientFirstName, first_name);
        strcpy(patient->patientLastName, last_name);
        strcpy(patient->recordID, id);
        patient->entryDate = entry_date;
        patient->exitDate = exit_date;

        if (RecHashGetItem(records_hash, id) != NULL)
        {
            printf("The file contains patient record with pre-existing record id! : %s\nExiting...\n", id);
            exit(EXIT_FAILURE);
        }
        //Inserting the patient's record into the records hashtable
        RecHashInsert(&records_hash, id, patient, bucket_size);

        //Inserting the virus into the hashtable if it's no there already and iserting the record into the tree
        AVLTree *virus_tree = HashGetItem(disease_hash, virus);
        if (virus_tree == NULL)
        {
            AVLTree *new_virus_tree = AvlCreate();
            HashInsert(&disease_hash, virus, new_virus_tree, bucket_size);
            AvlInsert(new_virus_tree, patient);
        }
        else
        {
            AvlInsert(virus_tree, patient);
        }

        //Inserting the virus into the hashtable if it's no there already and iserting the record into the tree
        AVLTree *country_tree = HashGetItem(country_hash, country);
        if (country_tree == NULL)
        {
            AVLTree *new_country_tree = AvlCreate();
            HashInsert(&country_hash, country, new_country_tree, bucket_size);
            AvlInsert(new_country_tree, patient);
        }
        else
        {

            AvlInsert(country_tree, patient);
        }

        //Moving on to the next line in the file
        get = getline(&line, &len, fp);
    }

    //-----Promt the user to choose one of the available commands----
    printf("\x1B[36m" "The patients' records have been inserted from the file.\n");
    printf("Give a command from the following list to continue.\nBrackets symbolize optional parameters\n" "\x1B[0m");
    printf("\x1B[35m" " 1. /globalDiseaseStats  [date1 date2]\n 2. /diseaseFrequency virusName [country] date1 date2\n 3. /topk-Diseases k country  [date1 date2]\n 4. /topk-Countries k disease [date1 date2]\n 5. /insertPatientRecord recordID patientFirstName patientLastName diseaseID\n 6. /recordPatientExit recordID exitDate\n 7. /numCurrentPatients [disease]\n 8. /exit\n" "\x1B[0m");

    //Get command fromm user
    char input[150];
    while (1)
    {
        fgets(input, sizeof(input), stdin);
        char *command = strtok(input, " ");
        char *token;

        if (strcmp(command, "/globalDiseaseStats") == 0 || strcmp(command, "/globalDiseaseStats\n") == 0)
        {
            //Checking that no dates are given or both dates are given
            if ((token = strtok(NULL, " ")) != NULL)
            {
                char *date1_str = token;
                char *date2_str = strtok(NULL, " ");
                if (date2_str == NULL)
                {
                    printf("Only one date was given! The command runs with either no dates or two dates.\n");
                    continue;
                }
                //Removing trailing new_line char
                date2_str[strlen(date2_str) - 1] = '\0';
                globalDiseaseStats(date1_str, date2_str, disease_hash);
            }
            else
            {
                globalDiseaseStats(NULL, NULL, disease_hash);
            }
        }
        else if (strcmp(command, "/diseaseFrequency") == 0)
        {

            char *virusName = strtok(NULL, " ");
            char *date1_str = strtok(NULL, " ");
            char *date2_str = strtok(NULL, " ");
            char *countryName = strtok(NULL, " ");
            if (countryName != NULL)
            {
                //Removing trailing new_line char
                countryName[strlen(countryName) - 1] = '\0';
                diseaseFrequency(virusName, date1_str, date2_str, countryName, disease_hash);
            }
            else
            {
                //Removing trailing new_line char
                date2_str[strlen(date2_str) - 1] = '\0';
                diseaseFrequency(virusName, date1_str, date2_str, NULL, disease_hash);
            }
        }
        else if (strcmp(command, "/insertPatientRecord") == 0)
        {

            id = strtok(NULL, " ");
            first_name = strtok(NULL, " ");
            last_name = strtok(NULL, " ");
            virus = strtok(NULL, " ");
            country = strtok(NULL, " ");
            entry_str = strtok(NULL, " ");
            exit_str = strtok(NULL, " ");
            if (exit_str != NULL)
            {
                exit_str[strlen(exit_str) - 1] = '\0'; //Remove trailing new_line char
            }
            else
            {
                entry_str[strlen(entry_str) - 1] = '\0'; //Remove trailing new_line char
            }
            //Definition and initialization of date structs
            struct tm entry_date, exit_date;
            memset(&entry_date, 0, sizeof(struct tm));
            memset(&exit_date, 0, sizeof(struct tm));

            //Inserting dates into tm structs
            strptime(entry_str, "%d-%m-%Y", &entry_date);
            if (exit_str != NULL)
            {
                strptime(exit_str, "%d-%m-%Y", &exit_date);
            }

            if (exit_str != NULL)
            {

                time_t time_entry = mktime(&entry_date);
                time_t time_exit = mktime(&exit_date);
                double difference = difftime(time_entry, time_exit);
                if (difference > 0)
                {
                    printf("Patient record with record id: %s, is rejected because the exit date is older than the entry date.\n", id);
                    continue;
                }
            }

            //Creating the record to be inserted into the hashtable
            Records *patient_rec = malloc(sizeof(Records));
            strcpy(patient_rec->country, country);
            strcpy(patient_rec->disease, virus);
            strcpy(patient_rec->patientFirstName, first_name);
            strcpy(patient_rec->patientLastName, last_name);
            strcpy(patient_rec->recordID, id);
            patient_rec->entryDate = entry_date;
            patient_rec->exitDate = exit_date;

            if (RecHashGetItem(records_hash, id) != NULL)
            {
                printf("A patient record with this record id was already inserted before! : %s\nExiting...\n", id);
                exit(EXIT_FAILURE);
            }
            //Inserting the patient's record into the records hashtable
            RecHashInsert(&records_hash, id, patient_rec, bucket_size);

            //Inserting the virus into the hashtable if it's no there already and iserting the record into the tree
            AVLTree *virus_tree = HashGetItem(disease_hash, virus);
            if (virus_tree == NULL)
            {
                AVLTree *new_virus_tree = AvlCreate();
                HashInsert(&disease_hash, virus, new_virus_tree, bucket_size);
                AvlInsert(new_virus_tree, patient_rec);
            }
            else
            {
                AvlInsert(virus_tree, patient_rec);
            }

            //Inserting the virus into the hashtable if it's no there already and iserting the record into the tree
            AVLTree *country_tree = HashGetItem(country_hash, country);
            if (country_tree == NULL)
            {
                AVLTree *new_country_tree = AvlCreate();
                HashInsert(&country_hash, country, new_country_tree, bucket_size);
                AvlInsert(new_country_tree, patient_rec);
            }
            else
            {

                AvlInsert(country_tree, patient_rec);
            }
            printf("Record added\n");
        }
        else if (strcmp(command, "/recordPatientExit") == 0)
        {
            char *recordID = strtok(NULL, " ");
            char *exitDate_str = strtok(NULL, " ");
            //Removing trailing new_line char
            exitDate_str[strlen(exitDate_str) - 1] = '\0';

            Records *patient_rec = RecHashGetItem(records_hash, recordID);
            struct tm exit;
            memset(&exit_date, 0, sizeof(struct tm));
            strptime(exitDate_str, "%d-%m-%Y", &exit);
            patient_rec->exitDate = exit;
            printf("Record updated\n");
        }
        else if (strcmp(command, "/topk-Diseases") == 0)
        {
            char *k_str = strtok(NULL, " ");
            int k = (int)strtol(k_str, NULL, 10);

            char *countryName = strtok(NULL, " ");
            char *date1_str = strtok(NULL, " ");
            if (date1_str != NULL)
            {
                char *date2_str = strtok(NULL, " ");
                if (date2_str == NULL)
                {
                    printf("Only one date was given! The command runs with either no dates or two dates.\n");
                    continue;
                }
                //Removing trailing new_line char
                date2_str[strlen(date2_str) - 1] = '\0';
                topkDiseases(k, countryName, date1_str, date2_str, disease_hash);
            }
            else
            {

                //Removing trailing new_line char
                countryName[strlen(countryName) - 1] = '\0';
                topkDiseases(k, countryName, NULL, NULL, disease_hash);
            }
        }
        else if (strcmp(command, "/topk-Countries") == 0)
        {
            char *k_str = strtok(NULL, " ");
            int k = (int)strtol(k_str, NULL, 10);

            char *diseaseName = strtok(NULL, " ");
            char *date1_str = strtok(NULL, " ");
            if (date1_str != NULL)
            {
                char *date2_str = strtok(NULL, " ");
                if (date2_str == NULL)
                {
                    printf("Only one date was given! The command runs with either no dates or two dates.\n");
                    continue;
                }
                //Removing trailing new_line char
                date2_str[strlen(date2_str) - 1] = '\0';
                topkCountries(k, diseaseName, date1_str, date2_str, country_hash);
            }
            else
            {

                //Removing trailing new_line char
                diseaseName[strlen(diseaseName) - 1] = '\0';
                topkCountries(k, diseaseName, NULL, NULL, country_hash);
            }
        }
        else if (strcmp(command, "/numCurrentPatients") == 0 || strcmp(command, "/numCurrentPatients\n") == 0)
        {
            char *diseaseName = strtok(NULL, " ");
            if (diseaseName == NULL)
            {
                numCurrentPatients(diseaseName, disease_hash);
            }
            else
            {
                diseaseName[strlen(diseaseName) - 1] = '\0';
                numCurrentPatients(diseaseName, disease_hash);
            }
        }
        else if (strcmp(command, "/exit\n") == 0)
        {
            printf("exiting\n");
            break;
        }
    }

    RecHashDestroy(records_hash);
    HashDestroy(disease_hash);
    HashDestroy(country_hash);
    fclose(fp);
    free(line);
    free(records_file);
    exit(EXIT_SUCCESS);
}