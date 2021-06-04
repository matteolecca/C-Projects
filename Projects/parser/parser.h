#include "mvm.h"

struct prog
{
    char **wds;
    int cw; /*Current Word*/
    int nw;
    mvm *variables;
};
typedef struct prog Program;

typedef enum
{
    start,
    end,
    both
} position;
typedef enum {
    nline,
    sline
}endprint;
typedef enum
{
    equal,
    greater,
    lower
} comp;

#define ERROR(PHRASE)                                                                           \
    {                                                                                           \
        fprintf(stderr, "Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); \
        exit(2);                                                                                \
    }


void Prog(Program *p);
void Code(Program *p);
void Statement(Program *p);
int cmp(char *string, char *string1);
int containsC(char *string, char c, position pos);
void concatstring(FILE *fp, char *string, char c);
void printList(Program *prog);
Program *progInit(void);
int readFile(char *progName, Program *prog);
void freeProg(Program *prog);
void rot13(char *string);
void rot5(int n);
int printSt(Program *p, endprint pr);
int jump(Program *p);
void addItem(Program *prog, int i, FILE *fp);
int isNumber(char *s);
void parseOk(void);
int addvar(Program *p);
int addvarname(Program *p);
int ifequal(Program *p, comp c);
int increase(Program *p);
void removechar(char *string);
int isVar(char *string);
int isConst(char *string);
void is(char *string, char *string1);
char *addValue(Program *p);
void rnd(Program *p);
int insertnum(Program *p);
int ifgreater(Program *p);
int compareint(char *string, char *string1);
int in2string(Program *p);
int hashvalue(char *string, int size);
int powerf(int x, int y);
void openfile(Program *p, Program *np);
void declarevar(Program *p);
void abortprog(Program *p);
void print(char *string, endprint pr);
void insertVar(Program *p, char *string);
void skip(Program *p);
void compare(Program *p, comp c, char *string, char *string1);
