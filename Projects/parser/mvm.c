#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mvm.h"

#define EXTRACHAR 6


mvm *mvm_init(void)
{
    mvm *p;
    p = malloc(sizeof(mvm));
    if (p == NULL)
    {
        ON_ERROR("Creation Failed\n");
    }
    p->numkeys = 0;
    p->head = NULL;

    return p;
}
int mvm_size(mvm *m)
{
    if(m==NULL){
        return 0;
    }
    return m->numkeys;
}
void mvm_insert(mvm *m, char *key, char *data)
{
    mvmcell *c;

    if(m==NULL||key==NULL||data==NULL){
        return;
    }

    c = (mvmcell *) malloc(sizeof(mvmcell));
    c->key = malloc(strlen(key)+1);
    strcpy(c->key, key);
    c->data = malloc(strlen(data)+1);
    strcpy(c->data, data);

    c->next = m->head;
    m->head = c;
    m->numkeys++;
}
char *mvm_print(mvm *m)
{
    mvmcell *c = m->head;
    int counter=0;
    char *string=NULL ;
    char *list = NULL;
    int lenstring, lenlist=0;
    if(c==NULL){
        return NULL;
    }

    string=(char*)calloc(1,sizeof(char));
    list=(char*)calloc(1,sizeof(char));
    

    while (c != NULL)
    {
        lenstring = strlen(c->data) + strlen(c->key)  + EXTRACHAR;
        string=(char*)realloc(string,sizeof(char)*lenstring);
        sprintf(string, "[%s](%s) ", c->key, c->data);
        counter++;
        lenlist+=lenstring;
        list=(char*)realloc(list,sizeof(char)*lenlist+1);

        list=strcat(list, string);
        c = c->next;
    }

    free(string);
    return list;
}
void mvm_delete(mvm *m, char *key)
{
    mvmcell *c; 

    if(m==NULL||key==NULL){
        return;
    }
    c= m->head;

    if (strcmp(c->key, key) == 0)
    {
        m->head = c->next;
        free(c->key);
        free(c->data);

        free(c);
        
        m->numkeys--;
        return;
    }
    
    while (c->next->next != NULL)
    {
        if (strcmp(c->next->key, key) == 0)
        {
            c->next = c->next->next;
             m->numkeys--;

            return;
        }
        c = c->next;
    }
}
char *mvm_search(mvm *m, char *key)
{
    mvmcell *c = m->head;

   
    while (c != NULL)
    {
        if (strcmp(c->key, key) == 0)
        {
            return c->data;
        }
        c = c->next;
    }
    return NULL;
}
char **mvm_multisearch(mvm *m, char *key, int *n)
{
    char **array;
    int i = 0, count=1, counter=0;
    mvmcell *c = m->head;
    array = (char**) calloc(1,sizeof(array));


    while (c != NULL)
    {
        counter++;
        if (strcmp(c->key, key) == 0)
        {
            
            array=(char**)realloc(array,sizeof(array)*count+1);
            array[i] = c->data;

            count++;
            i++;
            *n = i;
        }
        c = c->next;
    }
    return array;
}
void mvm_free(mvm **p){
    
    mvm *a=*p;
    mvmcell *c = a->head;
    mvmcell *b;

    while(c!=NULL){
        free(c->data);
        free(c->key);
        b=c;
        c=c->next;
        free(b);
    }

    
    free(a);
    *p=NULL;
    

}

