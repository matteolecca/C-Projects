#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define MAXTOKENSIZE 300
#define MAXRDMNUM 100
#define Q '"'
#define H '#'
#define LETT 26
#define MAXINTL 3

#include "nal.h"

int main(int argc, char **aargv)
{
#ifdef BASETEST
    test();
    exit(0);
#endif

    if (argc == 2)
    {
        mvm *m;
        /*Init new program*/
        Program *prog = progInit();
        /*Init new mvm_list where variables will be stored*/
        prog->variables = mvm_init();
        /*Pointer to the var list to free it at the end*/
        m = prog->variables;
        srand(time(NULL));

        readFile(aargv[1], prog);
        mvm_free(&m);
        parseOk();
    }
    else
    {
        ERROR("ERROR. Type things like file.nal");
    }

    return 0;
}
int readFile(char *progName, Program *prog)
{
    int i = 0;
    FILE *fp;

    fp = fopen(progName, "r");
    if (fp == NULL)
    {
        ERROR("Incorrect file name");
    }
    /*Alloc space fot array of strings*/
    prog->wds = (char **)malloc(sizeof(char *));
    /*Alloc space for the first string*/
    prog->wds[0] = (char *)malloc(MAXTOKENSIZE);

    /*Scan every word into the text file*/
    while (fscanf(fp, "%s", prog->wds[i]) != EOF)
    {
        addItem(prog, i, fp);
        i++;
    }
    fclose(fp);

    Prog(prog);
    freeProg(prog);

    return 1;
}
void addItem(Program *prog, int i, FILE *fp)
{
    const char space = ' ';

    /*If the string contains a " at index 0*/
    if (containsC(prog->wds[i], Q, start) && !containsC(prog->wds[i], Q, end))
    {
        /*Add a space char at the end*/
        strncat(prog->wds[i], &space, 1);
        /*Call concatstring*/
        concatstring(fp, prog->wds[i], Q);
        /*Remove " from the first and last char*/
        removechar(prog->wds[i]);
    }
    /*If the string contains a # at index 0*/
    if (containsC(prog->wds[i], H, start) && !containsC(prog->wds[i], H, end))
    {
        /*Add a space char at the end*/
        strncat(prog->wds[i], &space, 1);
        /*Call concatstring*/
        concatstring(fp, prog->wds[i], H);
        /*Convert it using rot rule*/
        rot18(prog->wds[i]);
        /*Remove # from the first and last char*/
        removechar(prog->wds[i]);
    }
    /*If the string is a single word var*/
    if (containsC(prog->wds[i], Q, start) && containsC(prog->wds[i], Q, end))
    {
        /*Remove " from the first and last char*/
        removechar(prog->wds[i]);
    }

    /*If the string is a single word constant*/
    if (containsC(prog->wds[i], H, start) && containsC(prog->wds[i], H, end))
    {
        /*Convert it using rot rule*/
        rot18(prog->wds[i]);
        /*Remove # from the first and last char*/
        removechar(prog->wds[i]);
    }

    i++;
    /*Alloc space for the next word*/
    prog->wds = (char **)realloc(prog->wds, sizeof(char *) * (i + 1));
    prog->wds[i] = (char *)malloc(sizeof(char) * MAXTOKENSIZE);
    prog->nw++;
}

void removechar(char *string)
{
    /*Remove first char from string*/
    memmove(string, string + 1, strlen(string));
    /*Remove last char from string*/
    string[(int)strlen(string) - 1] = '\0';
}

void Prog(Program *p)
{

    /*If the first word is not { quit*/
    if (!cmp(p->wds[p->cw], "{"))
    {
        ERROR("No { at the start of file");
        return;
    }
    else
    {
        p->cw++;
        Code(p);
    }
}

void Code(Program *p)
{
    /*If } is found the program has ended, return*/
    if (cmp(p->wds[p->cw], "}"))
    {
        return;
    }
    Statement(p);
    p->cw++;
    Code(p);
}

int printSt(Program *p, endprint pr)
{
    char *temp;
    p->cw++;
    /*Silenziate warning when parse is called*/
    if (pr)
    {
    }
    /*If the word contains % or $ at the start*/
    if (isVar(p->wds[p->cw]))
    {
        /*Take the corresp value using mvm_search*/
        temp = mvm_search(p->variables, p->wds[p->cw]);
        if (temp == NULL)
        {
            printf("\nNo variables called %s\n", p->wds[p->cw]);
            ERROR("");
        }
#ifdef INTERP
        print(temp, pr);
#endif
    }
    else
    {
#ifdef INTERP
        temp = malloc(strlen(p->wds[p->cw]) + 1);
        /*If is not a variable simply copy the string*/
        strcpy(temp, p->wds[p->cw]);
        print(temp, pr);
        free(temp);

#endif
    }
    return 1;
}

void print(char *string, endprint pr)
{

    if (pr == nline)
    {
        printf("%s ", string);
    }
    else if (pr == sline)
    {
        printf("%s\n", string);
    }
}

void Statement(Program *p)
{
    Program *nprog = NULL;

    if (cmp(p->wds[p->cw], "PRINTN"))
    {
        printSt(p, nline);
        return;
    }
    if (cmp(p->wds[p->cw], "PRINT"))
    {
        printSt(p, sline);
        return;
    }
    if (cmp(p->wds[p->cw], "JUMP"))
    {
        jump(p);
        return;
    }
    if (cmp(p->wds[p->cw], "ABORT"))
    {
        abortprog(p);
    }
    if (isVar(p->wds[p->cw]))
    {
        declarevar(p);
        return;
    }
    if (cmp(p->wds[p->cw], "IFEQUAL"))
    {
        ifequal(p, equal);
        return;
    }
    if (cmp(p->wds[p->cw], "IFGREATER"))
    {
        ifequal(p, greater);
        return;
    }
    if (cmp(p->wds[p->cw], "INC"))
    {
        increase(p);
        return;
    }
    if (cmp(p->wds[p->cw], "RND"))
    {
        rnd(p);
        return;
    }
    if (cmp(p->wds[p->cw], "INNUM"))
    {
        insertnum(p);
        return;
    }
    if (cmp(p->wds[p->cw], "IN2STR"))
    {
        in2string(p);
        return;
    }
    if (cmp(p->wds[p->cw], "FILE"))
    {
        openfile(p, nprog);
        return;
    }
    ERROR("Parsing");
}
void abortprog(Program *p)
{
    /*Free space allocated before aborting*/
    mvm_free(&p->variables);
    freeProg(p);
    exit(0);
}
void declarevar(Program *p)
{
    p->cw++;
    /*Check var declaration syntax*/
    is(p->wds[p->cw], "=");
    p->cw++;
    /*Insert as the key the string at index -2 (%C(0) =(1) VALUE(2)  )*/
    mvm_insert(p->variables, p->wds[p->cw - 2], p->wds[p->cw]);
}

void openfile(Program *p, Program *np)
{
    char *string;
    p->cw++;
    /*Initialize a new prog variable*/
    np = progInit();
    /*Copy the variable list from the previous program*/
    np->variables = p->variables;
    /*Alloc space for file name*/
    string = malloc(strlen(p->wds[p->cw]) + 1);
    /*Copy file name into string*/
    strcpy(string, p->wds[p->cw]);
    readFile(string, np);
    free(string);
    return;
}
void insertVar(Program *p, char *string)
{
    p->cw++;
    /*If the word start with % or $*/
    if (isVar(p->wds[p->cw]))
    {
#ifdef INTERP
        scanf("%s", string);
#endif
        /*Insert the scanned value into the var list*/
        mvm_insert(p->variables, p->wds[p->cw], string);
    }
    else
    {
        ERROR("Incorrect variable declaration")
    }
}
int in2string(Program *p)
{
    char string[MAXTOKENSIZE];
    char string1[MAXTOKENSIZE];

    printf("\n");
    p->cw++;
    /*Check if the word is (*/
    is(p->wds[p->cw], "(");
    /*Insert user's input into list*/
    insertVar(p, string);
    p->cw++;
    is(p->wds[p->cw], ",");
    insertVar(p, string1);
    p->cw++;
    is(p->wds[p->cw], ")");
    return 1;
}

int insertnum(Program *p)
{
    char input[MAXTOKENSIZE];
    p->cw++;

    is(p->wds[p->cw], "(");
    p->cw++;
    /*Check if the word is an integer var*/
    if (containsC(p->wds[p->cw], '%', start))
    {
#ifdef INTERP
        scanf("%s", input);
#endif
        /*Insert var name as the key, input as the value*/
        mvm_insert(p->variables, p->wds[p->cw], input);
        p->cw++;
        is(p->wds[p->cw], ")");
        return 1;
    }
    return 0;
}

void rnd(Program *p)
{
    int randomn;
    char *rndnstring;

    p->cw++;
    is(p->wds[p->cw], "(");
    p->cw++;

    /*Check if the word is an integer var*/
    if (containsC(p->wds[p->cw], '%', start))
    {
        /*Alloc space for the random number*/
        rndnstring = malloc(MAXINTL);
        /*Generate random number*/
        randomn = rand() % MAXRDMNUM;
        /*Cast the int value of randomn into a string*/
        sprintf(rndnstring, "%d", randomn);
        /*Insert var name as the key, rndstring as the value*/
        mvm_insert(p->variables, p->wds[p->cw], rndnstring);
        p->cw++;
        free(rndnstring);
    }
    is(p->wds[p->cw], ")");
}
int increase(Program *p)
{
    int n;
    char *nstring, *temp;
    p->cw++;

    is(p->wds[p->cw], "(");
    p->cw++;

    /*Check if the word is an integer var*/
    if (containsC(p->wds[p->cw], '%', start))
    {
        /*Alloc space for the string that will contain the number*/
        nstring = malloc(MAXTOKENSIZE);
        /*Get the number to increase from the var list*/
        temp = mvm_search(p->variables, p->wds[p->cw]);
        if (temp == NULL)
        {
            printf("No variables called %s\n", p->wds[p->cw]);
            ERROR("");
        }
        /*Take a copy*/
        strcpy(nstring, temp);
        /*Convert string number to int*/
        n = atoi(nstring);
        /*Add one*/
        n++;
        /*Insert the new number back into nstring*/
        sprintf(nstring, "%d", n);
        /*Insert the new value inside the var list*/
        /*There is no need to delete the previous value as mvm_insert*/
        /*will insert the new one at the head of the list*/
        mvm_insert(p->variables, p->wds[p->cw], nstring);
        p->cw++;
        free(nstring);
        is(p->wds[p->cw], ")");
    }

    return 0;
}

int isVar(char *string)
{
    return containsC(string, '%', start) ||
           containsC(string, '$', start);
}
void is(char *string, char *string1)
{
    if (cmp(string, string1))
    {
        return;
    }
    else
    {
        ERROR("Syntax");
    }
}

char *getValue(Program *p)
{
    char *string = NULL;

    /*If is a int or string variable*/
    if (isVar(p->wds[p->cw]))
    {
        string = malloc(MAXTOKENSIZE);

        if (mvm_search(p->variables, p->wds[p->cw]) == NULL)
        {
            ERROR("Variable not initialized");
        }
        /*Get the correspondent value from the var list*/
        strcpy(string, mvm_search(p->variables, p->wds[p->cw]));
        p->cw++;
        return string;
    }
    else
    {
        string = malloc(MAXTOKENSIZE);
        /*Simply copy the value from the list of words*/
        strcpy(string, p->wds[p->cw]);
        p->cw++;
        return string;
    }
    return string;
}
void compare(Program *p, comp c, char *string, char *string1)
{
    /*If we're looking for two equal words*/
    if (c == equal && cmp(string, string1))
    {
        free(string1);
        free(string);
        Prog(p);
    }

    /*If we're looking for the first one bigger*/
    else if (c == greater && compareint(string, string1))
    {

        free(string1);
        free(string);
        Prog(p);
    }
    else
    {
        free(string1);
        free(string);
        skip(p);
    }
}
void skip(Program *p)
{
    /*Function to exit from a loop if a condition is false*/
    int numStat = 1, endloop = 0;
    while (!endloop)
    {
        p->cw++;
        /*For every opened brackets add one*/
        if (cmp(p->wds[p->cw], "{"))
        {
            numStat++;
        }
        /*For every closed brakets minus one*/
        if (cmp(p->wds[p->cw], "}"))
        {
            numStat--;
        }
        /*If we reach the 0 it means that we're out of the loop*/
        if (numStat == 0)
        {
            endloop++;
        }
    }
}
int ifequal(Program *p, comp c)
{
    char *string = NULL, *string1 = NULL;
    p->cw++;
    is(p->wds[p->cw], "(");
    p->cw++;
    /*Get the first value*/
    string = getValue(p);
    is(p->wds[p->cw], ",");
    p->cw++;
    /*Get the second value*/
    string1 = getValue(p);
    is(p->wds[p->cw], ")");
    p->cw++;
    /*Compare them*/
    compare(p, c, string, string1);

    return 0;
}
int compareint(char *string, char *string1)
{
    /*Return one if the first number is bigger than the second*/
    int fstn, scndn;
    fstn = atoi(string);
    scndn = atoi(string1);
    return fstn > scndn;
}

int jump(Program *p)
{
    int i;
    p->cw++;
    i = atoi(p->wds[p->cw]);

    /*Check if the index to jump to is valid*/
    if (i >= 0 && i <= p->nw)
    {
#ifdef INTERP
        p->cw = i;
        Statement(p);
#endif
        return 1;
    }
    ERROR("JUMP");
    return 0;
}
int isNumber(char *s)
{
    int i = atoi(s);
    if (i >= 0 && i <= 100)
    {
        return 1;
    }

    return 0;
}
void parseOk(void)
{
    printf("PARSING OK\n");
}

void rot18(char *string)
{
    int i = 0;
    /*For every char*/
    while (string[i] != '\0')
    {
        /*If the char is a letter between a/A and m/M*/
        if ((string[i] >= 'A' && string[i] <= 'M') ||
            (string[i] >= 'a' && string[i] <= 'm'))
        {
            string[i] += 13;
        }
        /*If the char is a letter between n/N and z/Z*/
        else if ((string[i] >= 'N' && string[i] <= 'Z') ||
                 (string[i] >= 'n' && string[i] <= 'z'))
        {
            string[i] -= 13;
        }
        /*If the char is a number between 0 and 4*/
        else if (string[i] >= '0' && string[i] <= '4')
        {
            string[i] += 5;
        }
        /*If the char is a number between 5 and 9*/
        else if (string[i] >= '5' && string[i] <= '9')
        {
            string[i] -= 5;
        }

        i++;
    }
}

int cmp(char *string, char *string1)
{
    /*Return 1 only if the two strings are equal*/
    if (string1 == NULL || string == NULL)
    {
        return 0;
    }
    if (strcmp(string, string1) == 0)
    {
        return 1;
    }
    return 0;
}
void freeProg(Program *prog)
{
    int i;
    for (i = 0; i <= prog->nw; i++)
    {
        free(prog->wds[i]);
    }

    free(prog->wds);
    free(prog);
}

void printList(Program *prog)
{
    int i;
    for (i = 0; i < prog->nw; i++)
    {

        printf("%d - %s\n", i, prog->wds[i]);
    }
}

int containsC(char *string, char c, position pos)
{
    /*Return one only if the string contain*/
    /*the char(c) at position(pos)*/
    if (string == NULL)
    {
        return 0;
    }
    if (pos == end)
    {
        return string[strlen(string) - 1] == c;
    }
    if (pos == start)
    {
        return string[0] == c;
    }
    if (pos == both)
    {
        return string[strlen(string) - 1] == c &&
               string[0] == c;
    }
    return 0;
}

void concatstring(FILE *fp, char *string, char c)
{
    char *temp = malloc(MAXTOKENSIZE);
    int q = 0;
    const char space = ' ';

    while (!q)
    {
        fscanf(fp, "%s", temp);
        /*Keep scanning until the string not contains a certain*/
        /*char at the end*/
        if (containsC(temp, c, end))
        {
            q = 1;
        }
        else
        {
            strncat(temp, &space, 1);
        }
        strcat(string, temp);
    }
    free(temp);
}

Program *progInit(void)
{
    Program *p;
    p = malloc(sizeof(Program));
    if (p == NULL)
    {
        ERROR("Creation Failed\n");
    }

    p->cw = 0;
    p->nw = 0;

    return p;
}
