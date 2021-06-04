#include "mvm.h"

struct prog
{
    char **wds;
    int cw;
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

typedef enum
{
    nline,
    sline
} endprint;

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

/*Chech the first char of a program*/
void Prog(Program *p);
/*Check */
void Code(Program *p);
void Statement(Program *p);
/*Return 1 only if the two strings are equal*/
int cmp(char *string, char *string1);
/*Return 1 if the string contain the char(c) at position(pos)*/
int containsC(char *string, char c, position pos);
/*Take all the words betwewen two equal chars(quotes or hashes)*/
/*And concatenate them into one single string*/
void concatstring(FILE *fp, char *string, char c);
/*Useful function to print a list of instruction from a nal file*/
void printList(Program *prog);
/*Initialize a program and return a pointer to it*/
Program *progInit(void);
/*Scan all the word contained into a test file and insert them inside*/
/*An array of string*/
int readFile(char *progName, Program *prog);
/*Free the space allocated*/
void freeProg(Program *prog);
/*Encode a string using ROT18*/
void rot18(char *string);
/*Take a value from either string or constant and print it*/
int printSt(Program *p, endprint pr);
/*Jump at a particular index into the code*/
int jump(Program *p);
/*Check the type of word scanned and modify it before inserting*/
void addItem(Program *prog, int i, FILE *fp);
/*Return one if is a number between 0 and 99*/
int isNumber(char *s);
/*Print PARSED OK*/
void parseOk(void);
/*Compare two words when IFEQUAL or IFGREATER is found in the code*/
int ifequal(Program *p, comp c);
/*Add one to a number*/
int increase(Program *p);
/*Remove the first and the last char from a string*/
void removechar(char *string);
/*Return one if the string contains % or $ as the first char*/
int isVar(char *string);
/*Return if the two string are equal, abort if not*/
void is(char *string, char *string1);
/*Check if the current word is a variable or not and return the*/
/*Correspondent value*/
char *getValue(Program *p);
/*Check if a % var has been declared and add a random number as a value*/
void rnd(Program *p);
/*Scan a value from the user and assign it to a int variable*/
int insertnum(Program *p);
/*Return one if the first number is bigger than the second*/
int compareint(char *string, char *string1);
/*Take two inputs from the user and insert them inside two string var*/
int in2string(Program *p);
/*Open new file, initialize new program, copy inside it the var list*/
/*From the previous program*/
void openfile(Program *p, Program *np);
/*Add value to a variable*/
void declarevar(Program *p);
/*Free space and abort program*/
void abortprog(Program *p);
/*Print a string with or without a linefeed*/
void print(char *string, endprint pr);
/*Take one input from the user and insert it into a variable */
void insertVar(Program *p, char *string);
/*Useful func to exit from a loop if a condition is false*/
void skip(Program *p);
/*Compare two string based on the value of 'c'*/
void compare(Program *p, comp c, char *string, char *string1);
/*Basic test on non recursive function*/
void test();
