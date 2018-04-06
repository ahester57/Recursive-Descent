#include <stdlib.h>
#include <stdio.h>
#include "keywords.h"
#include "find.h"
#include "states.h"

static const char* keywords[] =
    {"start", "stop", "iter", "void", "var", "return",
     "read", "print", "program", "iff", "then", "let", NULL};

// return 1 if it is a keyword
// return 0 otherwise
int
iskeyword(char* word)
{
    printf("%s\n", keywords[1]);
    return findword(word, keywords) + 1;
}

enum STATE
whichkeyword(char* word)
{
    int i = findword(word, keywords);
    enum STATE which = START + i;
    return which;
}