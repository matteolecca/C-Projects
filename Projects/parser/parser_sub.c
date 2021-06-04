#include "parser.h"
#define MAXTOKENSIZE 300


void rot13(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        if (string[i] >= 'A' && string[i] <= 'N')
        {
            string[i] += 13;
        }
        if (string[i] >= 'O' && string[i] <= 'Z')
        {
            string[i] -= 13;
        }
        if (string[i] >= 'a' && string[i] <= 'n')
        {
            string[i] += 13;
        }
        if (string[i] >= 'o' && string[i] <= 'z')
        {
            string[i] -= 13;
        }
        i++;
    }
}

int cmp(char *string, char *string1)
{
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
        return string[strlen(string) - 1] == c;
    }
    return 0;
}

void concatstring(FILE *fp, char *string)
{
    char *temp = malloc(MAXTOKENSIZE);
    int q = 0;
    const char space = ' ';

    while (!q)
    {
        fscanf(fp, "%s", temp);
        if (containsC(temp, Q, end))
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
        /*ON_ERROR("Creation Failed\n");*/
    }
    p->cw = 0;
    p->nw = 0;
    

    return p;
}

