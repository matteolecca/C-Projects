/*START OF THE COMMENTS*/

/*The program uses hashing to speed up the search*/

/*In the first table(word=key, phonemes=data) uses simple hashing for the insertion:*/
   /*-1 Calculate hash value of the key*/
   /*-2 Check if the index[hash] of the array is empty*/
        /*-2.1 If it's empty allocate space and insert key-data (head)*/
        /*-2.2 If it's not go back until a NULL cell is found, then insert*/
        

/*In the second table(phonemes=key, word=data) combines hashing with link list:*/
    /*-1 Calculate hash value of the key*/
    /*-2 Check if the index[hash] of the array is empty:*/
        /*-2.1 If it's empty allocate space and insert key-data (head)*/
        /*-2.2 If it's not empty*/
            /*-2.2.1 Check if phon string at index[hash] is the same as the one trying to insert*/
                /*-2.2.1.1 If it's the same, insert it into the link list at index[hash]*/
                /*-2.2.1.2 If it's not, go back and repeat*/
                         /*until the same phonemes string or an empty cell is found*/


/*The searching is faster but the insertion presents some bugs that I haven't manage to solve:*/
    /*Uses a fixed size array*/
    /*Seg fault appens if the user type a word not in the dictionary*/
    /*Memleaks errors*/

/*END OF THE COMMENTS*/
    

#define ON_ERROR(STR)fprintf(stderr, STR);exit(EXIT_FAILURE)

struct mvmcell
{
    char *key;
    char *data;
    struct mvmcell *next;
    int index;
};
typedef struct mvmcell mvmcell;
struct mvmsubarray
{
    mvmcell *head;
    int numkeys;
};
typedef struct mvmsubarray mvmsubarray;

/*Changed struct mvm to contain an array of structures*/
struct mvm
{
    int numkeys;

    mvmsubarray **array;
};
typedef struct mvm mvm;

mvm *mvm_init(void);
/* Number of key/value pairs stored */
int mvm_size(mvm *m);
void mvm_insert(mvm *m, char *key, char *data);
/* Return the corresponding value for a key */
char *mvm_search(mvm *m, char *key);
/*Find the correct index, call sub_multisearch, return string array with results*/
char **mvm_multisearch(mvm *m, char *key, int *n);
/* Free & set p to NULL */
void mvm_free(mvm **p);
/*Return number of spaces in a string*/
int numspaces(char *string);
/*Insert key(word) data(phonemes) into an array of struct using simple hashing*/
void adddata(char *key, char *data, mvmsubarray **cellarray);
/*Insert key(phonemes) data(word) into an array of struct using hashing+link list*/
void addkey(char *key, char *data, mvmsubarray **cellarray);
/*Initialize the subarray struct*/
mvmsubarray *sub_init(void);
/* Insert one key/value pair*/
void sub_insert(mvmsubarray *m, char *key, char *data);
/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char **sub_multisearch(mvmsubarray *m, int *n);
/*Return hash value of string*/
int hashvalue(char *string,int size);
/*Free link list*/
void sub_free(mvmsubarray **p);
/*Return x^y*/
int powerf(int x, int y);
