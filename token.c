#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "states.h"
#include "pop.h"

// I know these first few aren't actually tokens.
// Just so the indices match with enum STATE
const char* TOKENS[] = {
    "errorTK",
    "initialTK",
    "s1TK",
    "s2TK",
    "opTK",
    "delimTK",
    "idTK",
    "intTK",
    "=TK",
    ">TK",
    "<TK",
    ":TK",
    "+TK",
    "-TK",
    "*TK",
    "/TK",
    "%TK",
    ".TK",
    "(TK",
    ")TK",
    ",TK",
    "{TK",
    "}TK",
    ";TK",
    "[TK",
    "]TK",
    "&TK",
    "\"TK",
    "EOFTK"
};

// get the token indexed by state
const char*
gettoken(const enum STATE state)
{
    return TOKENS[state];
}

// is this the EOF token?
int
isEOFtoken(const token_t* token)
{
    if (strcmp(token->id, "EOFTK") == 0)
        return 1;
    return 0;
}

// make a token
void
maketoken(token_t* token,
          const enum STATE state,
          char* string,
          const int line)
{
    token->instance = (char*) malloc(256*sizeof(char));
    strcpy(token->instance, string);
    token->id = gettoken(state);
    token->line_num = line;
}

// display a list of tokens
void
displaytokens(token_t** tokenlist, const int numtokens)
{
    printf("\nFinal token list, in order:\n");
    printf("========================\n");
    int i;
    for (i = 0; i < numtokens; i++) {
        const token_t* t = tokenlist[0];
        printf("%s, %s, %d\n", t->id, t->instance, t->line_num);
        pop((void**)tokenlist);
    }
    printf("========================\n");
}