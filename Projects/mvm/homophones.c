#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include "fmvm.h"

#define MAXLEN 100
/*Default values for flag n*/
#define DEF_N 3
#define NFLAG "-n"


/*Insert values read from text file into link lists*/
void loadMvm(char *string, mvm *m, mvm *m1, int n);
/*Return the number of spaces into a string*/
int countspaces(char *string);
/*Find the phonemes for a string, print word that rhymes*/
void findphon(mvm *m, mvm *m1, char *string);
/*Return a string containing n number of phonemes for a word*/
char *nphon(char *string, int n, int size);
/*Convert every char into a string to caps*/
void capword(char *argv);
/*Print array of string*/
void printarray(char **stringphonarray, int n);
/*Call findphon for every word inside *argv[]*/
void startsearch(int start, int argc, char *argv[], mvm *m, mvm *m1);
/*Print arrow that points to results*/
void printarrow(int len);

void addmarker(char *string);

void test();

int main(int argc, char *argv[])
{

    mvm *m, *m1;
    int n = DEF_N, start = 1;
    clock_t t;
    double timeload, timesearch;
    test();

    if (argc >= 2)
    {
        

        m = mvm_init();
        
        m1 = mvm_init();
        /*If the user typed the -n flag*/
        if (strcmp(argv[1], NFLAG) == 0)
        {
            /*Convert string to int*/
            n = atoi(argv[2]);
            start = DEF_N;
        }
        t = clock();
        /*Load the dictionary into the two linklists*/
        loadMvm("cmudict.txt", m, m1, n);
        t = clock() - t;
        timeload = (double)t / CLOCKS_PER_SEC;

        /*Start searching rhymes and printing*/
        t = clock();
        startsearch(start, argc, argv, m, m1);
        t = clock() - t;
        timesearch = (double)t / CLOCKS_PER_SEC;

        /*Free*/
        mvm_free(&m);
        mvm_free(&m1);
        printf("Time taken to load arrays and text %f\n", timeload);
        printf("Time taken to search %f\n", timesearch);
    }
    else
    {
        /*Print error if user typed incorrect*/
        ON_ERROR("Incorrect usage\n");
    }
    return 0;
}
void startsearch(int start, int argc, char *argv[], mvm *m, mvm *m1)
{
    /*For every word typed by the user*/
    while (start < argc)
    {
        /*Convert to allcaps*/
        capword(argv[start]);
        /*Start searching and printing*/
        findphon(m, m1, argv[start]);
        start++;
    }
}
void findphon(mvm *m, mvm *m1, char *string)
{
    char *stringphon;
    char **stringphonarray;
    int n = 0;

    /*Search for the phonemes of a word and store it in stringphon*/
    stringphon = mvm_search(m, string);

    if (stringphon != NULL)
    {
        /*Print word and phonemes*/
        printf("\n%s = %s", string, stringphon);
        printarrow(strlen(string));

        /*Search for all the word with the same last n phonemes*/
        /*Store them inside stringphonarray*/
        stringphonarray = mvm_multisearch(m1, stringphon, &n);

        /*Simple print*/
        printarray(stringphonarray, n);

        free(stringphonarray);
    }
    else
    {

        printf("No rhymes found for %s\n", string);
    }
}
void printarrow(int len)
{
    int i;
    for (i = 0; i < len ; i++)
    {
        printf(" ");
    }
    printf("|\n");
    for (i = 0; i < len + 1; i++)
    {
        printf(" ");
    }
    printf("V\n");
}
void capword(char *argv)
{
    int i = 0;
    while (argv[i])
    {
        /*Convert every char to caps*/
        argv[i] = toupper((unsigned char)argv[i]);
        i++;
    }
}
void printarray(char **stringphonarray, int n)
{
    int i = 0;
    while (i < n)
    {
        printf("- %s ", stringphonarray[i]);
        i++;
    }
    printf("\n\n");
}

void loadMvm(char *string, mvm *m, mvm *m1, int n)
{

    char *string2 = malloc(sizeof(char) * MAXLEN);
    int i = 0, count;
    char *key, *data;

    /*Open file*/
    FILE *fp = fopen(string, "r");

    /*Store word by word into string2*/
    while (fgets(string2, MAXLEN, fp))
    {

        /*Store the word inside "key"*/
        key = strtok(string2, "#");
        /*Store the phhonemes inside "data"*/
        data = strtok(NULL, "\0");
        /*Count the number of phonemes inside data*/
        count = countspaces(data);

        /*If there are more phonemes than -n flag specifies remove them*/
        if (count >= n)
        {
            nphon(data, n, count);
        }

        addmarker(data);
        /*Insertion word(key) - phon(data)*/
        mvm_insert(m, key, data);

        /*Insertion word(data) - phon(key)*/
        mvm_insert(m1, data, key);

        i++;
    }
    free(string2);

    fclose(fp);
}

void addmarker(char *string){
    char marker =' ';
    if (string == NULL)
    {
        return ;
    }
    strncat(string, &marker, 1);
}

int countspaces(char *string)
{
    int count = 0, i = 0;
    if (string == NULL)
    {
        return 0;
    }
    /*Until the end of word char is found*/
    while (string[i] != '\0')
    {
        /*If a empty space is found*/
        if (string[i] == ' ')
        {
            count++;
        }

        i++;
    }
    /*Return number of spaces(number of phonemes -1)*/
    return count;
}
char *nphon(char *string, int n, int size)
{
    /*Return a string containing the last n phonemes*/
    char *newstring = malloc(strlen(string));
    int i = 0, count = 0, c = 0;
    while (i < (int)strlen(string))
    {
        /*Count spaces*/
        if (string[i] == ' ')
        {
            count++;
        }
        i++;

        /*If the last n phoneme is reached*/
        /*Start copying the phonemes into the new string*/
        if (count > size - n)
        {
            newstring[c] = string[i];
            c++;
        }
    }

    
    strcpy(string, newstring);
    free(newstring);
    return string;
}

void test()
{
    /*Quick testing on the functions modifies the the string*/
    char *string=malloc(100);
    /*Assert that the phonemes contains 7 spaces*/
    assert(countspaces("IH2 N T UW1 AH0 T IH0 V") == 7);
    
    strcpy(string, "IH2 N T UW1 AH0 T IH0 V");

    /*Remove the phonemes inside string except the last 3*/
    string = nphon(string, 3, 7);
    /*Now the phonemes should be 3, so the string should contain 2 spaces*/
    assert(countspaces(string) == 2);
    free(string);
}
