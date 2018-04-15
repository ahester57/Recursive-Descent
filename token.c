#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "states.h"
#include "pop.h"

// I know these first few aren't actually tokens.
// Just so the indices match with enum STATE
char* TOKENS[] = {
    "errorTK",
    "initialTK",
    "s1TK",
    "s2TK",
    "opTK",
    "delimTK",
    "startTK",
    "stopTK",
    "iterTK",
    "voidTK",
    "varTK",
    "returnTK",
    "readTK",
    "printTK",
    "programTK",
    "iffTK",
    "thenTK",
    "letTK",
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
char*
gettoken(enum STATE state)
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
    return;
}

token_t*
customtoken(char* id, char* string, int line)
{
    token_t* token = (token_t*) malloc(sizeof(token_t));
    token->instance = (char*) malloc(256*sizeof(char));
    strcpy(token->instance, string);
    token->id = (char*) malloc(256*sizeof(char));
    strcpy(token->id, id);
    token->line_num = line;
    return token;
}

void
copytokenlist(token_t** dest, token_t** src, int n)
{
    if (src == (token_t**)NULL)
        return;
    int i;
    for (i = 0; i < n; i++) {
        // Copy original token list
        dest[i] = (token_t*) malloc(sizeof(token_t));
        copytoken(dest[i], src[i]);
    }
}

void
copytoken(token_t* dest, token_t* src)
{
    if (dest == (token_t*)NULL)
        return;
    dest->id = (char*) malloc(256*sizeof(char));
    strcpy(dest->id, src->id);
    dest->instance = (char*) malloc(256*sizeof(char));
    strcpy(dest->instance, src->instance);
    dest->line_num = src->line_num;
}


// display a list of tokens
void
displaytokens(token_t** tokenlist, const int numtokens)
{
    printf("\nFinal token list, in order:\n");
    printf("========================\n");
    int i;
    for (i = 0; i < numtokens; i++) {
        const token_t* t = tokenlist[i];
        if (t == (token_t*)NULL)
            break;
        displaytoken(t);
    }
    printf("========================\n");
    printf("\n");
}

void
displaytoken(const token_t* t)
{
    printf("%s, \'%s\', %d\n", t->id, t->instance, t->line_num);
}