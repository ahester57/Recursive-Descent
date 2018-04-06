#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "wordlist.h"
#include "token.h"
#include "pop.h"

static token_t* tk = NULL;

// return num tokens
int
parser(token_t** tokenlist, wordlist_t* filter) {
    // build token list
    int i;
    while (1)
    {
        token_t* t = scanner(filter);
        if (t == (token_t*)NULL)
            return -1;
        displaytoken(t);
        tokenlist[i] = t;
        i++;
        if (isEOFtoken(t))
            break;
    }

    // show the list of tokens
    displaytokens(tokenlist, i);

    printf("popping tokens now...\n");
    int j;
    for (j = 0; j < i; j++) {
        tk = (token_t*) pop((void**) tokenlist); 
        displaytoken(tk);
    }

    return i;
}

void
program() {}

void
block() {}

void
vars() {}

void
mvars() {}

void
expr() {}

void
xhelp() {}

void
M() {}

void
R() {}

void
stats() {}

void
mstat() {}

void
stat() {}

void
in() {}

void
out() {}

void
iff() {}

void
loop() {}

void
assign() {}

void
RO() {}

void
lesshelp() {}

void
greathelp() {}

void
equalhelp() {}