#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <time.h>

#define MAXTOKENSIZE 300
#define MAXRDMNUM 100
#define PROGNAME "escape211.nal"
#define Q '"'
#define H '#'
#define LETT 26
#define MAXINTL 3
#include "parser.h"

void test();
int main(int argc, char **aargv)
{
    if (argc == 2)
    {
        mvm *m;
        Program *prog = progInit();
        prog->variables = mvm_init();
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
    prog->wds = (char **)malloc(sizeof(char *));
    prog->wds[0] = (char *)malloc(MAXTOKENSIZE);
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

    if (containsC(prog->wds[i], Q, start) && !containsC(prog->wds[i], Q, end))
    {
        strncat(prog->wds[i], &space, 1);
        concatstring(fp, prog->wds[i], Q);
        removechar(prog->wds[i]);
    }
    if (containsC(prog->wds[i], H, start) && !containsC(prog->wds[i], H, end))
    {
        strncat(prog->wds[i], &space, 1);
        concatstring(fp, prog->wds[i], H);
        rot13(prog->wds[i]);
        removechar(prog->wds[i]);
    }
    if (containsC(prog->wds[i], Q, start) && containsC(prog->wds[i], Q, end))
    {
        removechar(prog->wds[i]);
    }
    if (containsC(prog->wds[i], H, start) && containsC(prog->wds[i], H, end))
    {
        rot13(prog->wds[i]);
        removechar(prog->wds[i]);
    }

    i++;
    prog->wds = (char **)realloc(prog->wds, sizeof(char *) * (i + 1));
    prog->wds[i] = (char *)malloc(sizeof(char) * MAXTOKENSIZE);
    prog->nw++;
}

void removechar(char *string)
{
    memmove(string, string + 1, strlen(string));
    string[(int)strlen(string) - 1] = '\0';
}

void Prog(Program *p)
{

    if (!cmp(p->wds[p->cw], "{"))
    {
        ERROR("Should start with {\n");
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
    if (isVar(p->wds[p->cw]))
    {
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
        temp = malloc(MAXTOKENSIZE);
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
    ERROR("Error parsing \n");
}
void abortprog(Program *p)
{
    mvm_free(&p->variables);
    freeProg(p);
    exit(0);
}
void declarevar(Program *p)
{
    p->cw++;
    is(p->wds[p->cw], "=");
    p->cw++;
    mvm_insert(p->variables, p->wds[p->cw - 2], p->wds[p->cw]);
}

void openfile(Program *p, Program *np)
{
    char *string = malloc(MAXTOKENSIZE);
    p->cw++;
    np = progInit();
    np->variables = p->variables;
    strcpy(string, p->wds[p->cw]);
    readFile(string, np);
    free(string);
    return;
}
void insertVar(Program *p, char *string){
    p->cw++;
    if (isVar(p->wds[p->cw]))
    {
#ifdef INTERP
        scanf("%s", string);
#endif
        mvm_insert(p->variables, p->wds[p->cw], string);
    }
}
int in2string(Program *p)
{
    char string[MAXTOKENSIZE];
    char string1[MAXTOKENSIZE];

    p->cw++;
    is(p->wds[p->cw], "(");
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
    if (containsC(p->wds[p->cw], '%', start))
    {
#ifdef INTERP
        scanf("%s", input);
#endif
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
    if (containsC(p->wds[p->cw], '%', start))
    {
        rndnstring = malloc(MAXTOKENSIZE);
        randomn = rand() % MAXRDMNUM;

        snprintf(rndnstring, 3, "%d", randomn);
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
        if (containsC(p->wds[p->cw], '%', start))
        {
            nstring = malloc(MAXTOKENSIZE);
            temp = mvm_search(p->variables, p->wds[p->cw]);
            if (temp == NULL)
            {
                printf("No var called %s\n", p->wds[p->cw]);
                ERROR("");
            }
            strcpy(nstring, temp);
            n = atoi(nstring);
            n++;
            snprintf(nstring, MAXINTL, "%d", n);
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
        ERROR("Error");
    }
}

int isConst(char *string)
{
    if (string)
    {
    }
    return 1;
}
char *addValue(Program *p)
{
    char *string = NULL;

    if (isVar(p->wds[p->cw]))
    {
        string = malloc(MAXTOKENSIZE);
        strcpy(string, mvm_search(p->variables, p->wds[p->cw]));
        p->cw++;
        return string;
    }
    else if (isConst(p->wds[p->cw]) ||
             isNumber(p->wds[p->cw]))
    {
        string = malloc(MAXTOKENSIZE);
        strcpy(string, p->wds[p->cw]);
        p->cw++;
        return string;
    }
    else
    {
        ERROR("Erorr");
    }
}
void compare(Program *p, comp c, char *string, char *string1){
    if (c == equal && cmp(string, string1))
    {
        free(string1);
        free(string);
        Prog(p);

    }
    else if (c == greater && compareint(string, string1))
    {
        
        free(string1);
        free(string);
        Prog(p);

    }
    else{
        skip(p);
    }
}
void skip(Program *p){
    int numStat = 1, endloop = 0;
    while (!endloop)
    {
        p->cw++;
        if (cmp(p->wds[p->cw], "{"))
        {
            numStat++;
        }
        if (cmp(p->wds[p->cw], "}"))
        {
            numStat--;
        }
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
    string = addValue(p);
    is(p->wds[p->cw], ",");
    p->cw++;
    string1 = addValue(p);
    is(p->wds[p->cw], ")");
    p->cw++;
    compare(p,c,string,string1);

    return 0;
}
int compareint(char *string, char *string1)
{
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

    if (i >= 0 && i <= p->nw)
    {
#ifdef INTERP
        p->cw = i;
        Statement(p);
#endif
        return 1;
    }
    ERROR("   ");
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

void rot13(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        if ((string[i] >= 'A' && string[i] <= 'M') ||
            (string[i] >= 'a' && string[i] <= 'm'))
        {
            string[i] += 13;
        }
        else if ((string[i] >= 'N' && string[i] <= 'Z') ||
                 (string[i] >= 'n' && string[i] <= 'z'))
        {
            string[i] -= 13;
        }
        else if (string[i] >= '0' && string[i] <= '4')
        {
            string[i] += 5;
        }
        else if (string[i] >= '5' && string[i] <= '9')
        {
            string[i] -= 5;
        }

        i++;
    }
}

int cmp(char *string, char *string1)
{
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
