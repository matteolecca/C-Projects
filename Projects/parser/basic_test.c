#include "nal.c"


void test()
{
    char s[] = {"#Base string#"};
    char s1[] = {"Base string"};
    char s2[] = {"#Base string#"};
    char s3[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    char r13[] = {"NOPQRSTUVWXYZABCDEFGHIJKLM"};
    char s4[] = {"abcdefghijklmnopqrstuvwxyz"};
    char r13_4[] = {"nopqrstuvwxyzabcdefghijklm"};
    char s5[] = {"0123456789"};
    char r_5[] = {"5678901234"};
    char prog_array[7][20] = {
        {"{"},
        {"PRINT"},
        {"HELLO WORLD"},
        {"$G"},
        {"="},
        {"Matteo"},
        {"}"}

    };

    int i = 0;

    FILE *fp;
    mvm *m;
    /*Init new program*/
    Program *prog = progInit();
    /*Init new mvm_list where variables will be stored*/
    prog->variables = mvm_init();
    /*Pointer to the var list to free it at the end*/
    m = prog->variables;
    srand(time(NULL));

    printf("Basic NAL Test Start\n");
    assert(containsC(NULL, '#', start)==0);
    assert(containsC(s, '#', both) == 1);
    assert(containsC(s, '#', start) == 1);
    assert(containsC(s1, '#', start) == 0);

    assert(cmp(s, s1) == 0);
    assert(cmp(s, s2) == 1);
    assert(cmp(NULL, s) == 0);
    assert(cmp(s, NULL) == 0);

    rot18(s3);
    assert(cmp(s3, r13) == 1);

    rot18(s4);
    assert(cmp(s4, r13_4) == 1);

    rot18(s5);
    assert(cmp(s5, r_5) == 1);

    removechar(s);
    assert(cmp(s1, s) == 1);

    fp = fopen("test1.nal", "r");
    /*Alloc space fot array of strings*/
    prog->wds = (char **)malloc(sizeof(char *));
    /*Alloc space for the first string*/
    prog->wds[0] = (char *)malloc(MAXTOKENSIZE);

    while (fscanf(fp, "%s", prog->wds[i]) != EOF)
    {
        addItem(prog, i, fp);
        assert(cmp(prog->wds[i], prog_array[i]) == 1);
        i++;
    }
    Prog(prog);

    assert(isVar("$G") == 1);
    assert(isVar("G") == 0);

    assert(cmp("Matteo", mvm_search(prog->variables, "$G")) == 1);

    assert(isNumber("30") == 1);
    freeProg(prog);
    mvm_free(&prog->variables);

    printf("Basic NAL Test Stop\n");
}
