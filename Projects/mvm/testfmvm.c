#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

#include "fmvm.h"

int main(){
    char string[5][35] = {
        "FERNANDO#F ER0 N AE1 N D OW0",
        "DRABIK#D R AA1 B IH0 K",
        "BOHMER#B OW1 M ER0",
        "BENEDIKT#IH0 K T",
        "WAGENKNECHT#IH0 K T"
        };
    char *key,*data;
    char **result;
    char *word,*corrphon;
    int  j,n=0;
    
    mvm *m,*m1;
    printf("Basic MVM Tests ... Start\n");

    /*Checking the func that return the hash value*/
    assert(hashvalue("BOHMER", 100) == 5);

    /*Checking the func that return the number of spaces*/
    assert(numspaces("B OW1 M ER0") == 3);

    /*Checking the func that return x^y*/
    assert(powerf(3,7)==2187);

    /*Inizializing*/
    m = mvm_init();
    m1 = mvm_init();
    assert(m != NULL);
    assert(mvm_size(m) == 0);

    /*Multiple insertion*/
    for (j = 0; j < 5; j++)
    {
        key = strtok(string[j], "#");
        data = strtok(NULL, "\0");
        mvm_insert(m, key, data);
        mvm_insert(m1, data, key);
    }


    /*Check the new size*/
    assert(mvm_size(m) == 5);
    assert(mvm_size(m1) == 5);

    /*Assertion failed*/
    /*result=mvm_multisearch(m,NULL,&n);*/

    /*Search for the phoneme in the first list*/
    /*inserting a name*/
    word=mvm_search(m,"FERNANDO");
    assert(strcmp(word, "F ER0 N AE1 N D OW0")==0);

    /*Check for a name in the second list*/
    /*inserting the phonemes*/
    corrphon = mvm_search(m1, word);
    assert(strcmp(corrphon, "FERNANDO") == 0);
    free(word);
    free(corrphon);

    /*Search in the middle of the list*/
    word=mvm_search(m,"BOHMER");
    assert(strcmp(word, "B OW1 M ER0")==0);
    corrphon = mvm_search(m1, word);
    assert(strcmp(corrphon, "BOHMER") == 0);

    /*Multi searching*/
    result = mvm_multisearch(m1, "IH0 K T",&n);
    assert(n==2);
    assert(strcmp(result[0], "WAGENKNECHT") == 0);
    assert(strcmp(result[1], "BENEDIKT") == 0);
    free(result);

    printf("Basic MVM Tests ... Stop\n");
    return 0;
}
