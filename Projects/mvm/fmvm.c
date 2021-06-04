#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "fmvm.h"

#define LETT 26
#define SIZE 135019
#define MAXLEN 100


mvm *mvm_init(void)
{
    mvm *p;
    p = (mvm*)malloc(sizeof(mvm));
    p->array = malloc(sizeof(mvmsubarray) * SIZE);
    if (p == NULL)
    {
        ON_ERROR("Creation Failed\n");
    }
    p->numkeys = 0;

    return p;
}
char **mvm_multisearch(mvm *m, char *key, int *n)
{
    int hash;
    char **result = NULL;
    if (m == NULL || key == NULL)
    {
        return NULL;
    }
    /*Key hash value*/
    hash = hashvalue(key, SIZE);

    /*Until key at index hash is not equal to key*/
    while (strcmp(m->array[hash]->head->key, key) != 0)
    {
        hash--;
        
        
        
    }
    /*Store into result all the word into the link list pointed by */
    /*m->array[hash]->head*/
    result = sub_multisearch(m->array[hash], n);
    return result;
   
}
char **sub_multisearch(mvmsubarray *m, int *n)
{

    char **array;
    int i = 0, count = 1;
    mvmcell *c = m->head;
    array = (char **)calloc(1, sizeof(array));

    while (c != NULL)
    {
        /*Store inside array the data contained  into the struct pointed by c*/
        /*until c is not null*/

        array = (char **)realloc(array, sizeof(array) * count + 1);
        
        array[i] = c->data;
        count++;
        i++;
        *n = i;

        c = c->next;
    }
    return array;
}

char *mvm_search(mvm *m, char *key)
{

    int hash;
    char *result = (char*)malloc(MAXLEN);

    /*Key hash value*/
    hash = hashvalue(key, SIZE);

    /*Until key at index hash is not equal to key*/
    while (strcmp(m->array[hash]->head->key, key) != 0 || hash < 0)
    {

        hash--;
    }
    
    /*Copy data into result*/
    strcpy(result, m->array[hash]->head->data);
    return result;
}
int mvm_size(mvm *m)
{
    if (m == NULL)
    {
        return 0;
    }
    return m->numkeys;
}
void mvm_insert(mvm *m, char *key, char *data)
{
    m->numkeys++;

    /*If the key is a word*/
    if (numspaces(key) == 0)
    {
        adddata(key, data, m->array);
        return;
    }

    /*If the key is a phonemes string*/
    else
    {
        addkey(key, data, m->array);

        return;
    }
}
void adddata(char *key, char *data, mvmsubarray **cellarray)
{

    int hash;

    if (key == NULL || data == NULL || cellarray == NULL)
    {
        return;
    }
    /*Key hash value*/
    hash = hashvalue(key, SIZE);

   
        /*Go back until an empty space is found*/
        while (cellarray[hash] != NULL && hash > 0)
        {
            hash--;
        }
    
    /*Allocate the space for the new cell*/
    cellarray[hash] = sub_init();

    /*Insert key and data*/
    sub_insert(cellarray[hash], key, data);
}
void sub_insert(mvmsubarray *m, char *key, char *data)
{
    mvmcell *c;

    if (m == NULL || key == NULL || data == NULL)
    {
        return;
    }

    /*Allocate the space for the cell*/
    c = (mvmcell *)malloc(sizeof(mvmcell));

    /*Copy key and data inside the cell*/
    c->key = (char*)malloc(strlen(key) + 1);
    strcpy(c->key, key);
    c->data = (char*)malloc(strlen(data) + 1);
    strcpy(c->data, data);
    c->next = m->head;
    m->head = c;
    m->numkeys++;
}
void addkey(char *key, char *data, mvmsubarray **cellarray)
{

    int hash;

    if (key == NULL || data == NULL || cellarray == NULL)
    {
        return;
    }

    /*Get the hash value of key*/
    hash = hashvalue(key, SIZE);

    /*Go back until an empty cell is found*/
    /*Or a cell containing the same phoneme is found*/
    while (cellarray[hash] != NULL && strcmp(cellarray[hash]->head->key, key) != 0)
    {
            hash--;
    }

    /*If a cell containing the same phoneme is found*/
    if (cellarray[hash] != NULL && cellarray[hash]->head->key != NULL)
    {   
        /*Insert the key and the data at the head of the cell*/
        sub_insert(cellarray[hash], key, data);
        return;
    }
    /*If there is space availabe*/
    /*Initialize a new cell*/
    cellarray[hash] = sub_init();

    /*Insert the key and the data at the head of the cell*/
    sub_insert(cellarray[hash], key, data);
}



mvmsubarray *sub_init(void)
{
    /*Inizialize the struct that will contain key-data*/
    mvmsubarray *p;
    p = (mvmsubarray*)malloc(sizeof(mvmsubarray));
    if (p == NULL)
    {
        ON_ERROR("Creation Failed\n");
    }
    p->numkeys = 0;
    p->head = NULL;

    return p;
}

int numspaces(char *string)
{
    int count = 0, i = 0;
    if (string == NULL)
    {
        return 0;
    }
    /*Increase count by one for every empty space*/
    while (string[i] != '\0')
    {
        if (string[i] == ' ')
        {
            count++;
        }

        i++;
    }
    return count;
}


int hashvalue(char *string, int size)
{
    
    int i;
    unsigned long int power, sum = 0;
    int len = (int)strlen(string);

    for (i = 0; i < len; i++)
    {
        power = powerf(LETT, len - (i + 1));
        power *= string[i] - 'A';
        sum += power;
    }

    return sum % size;
}

void mvm_free(mvm **p)
{
    mvm *s = *p;
    int i = 0;
    /*Check all the cell inside the struct array*/
        while (i < SIZE)
    {
        if (s->array[i] != NULL)
        {
            sub_free(&s->array[i]);
        }
        i++;
    }
    printf("Fastest MVM\n");
}
void sub_free(mvmsubarray **p)
{
    /*Free all the cells inside link list pointed by head*/
    mvmsubarray *a = *p;
    mvmcell *c = a->head;
    mvmcell *b;

    while (c != NULL)
    {
        free(c->data);
        free(c->key);
        b = c;
        c = c->next;
        free(b);
    }

    free(a);
    *p = NULL;
}
int powerf(int x, int y){
    int i, result=1;
    for(i=0;i<y;i++){
        result=x*result;
    }
    return result;
}
