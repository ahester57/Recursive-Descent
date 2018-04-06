#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "wordlist.h"
#include "token.h"

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
        printf("\t\t%s, %s, %d\n", t->id, t->instance, t->line_num);
        tokenlist[i] = t;
        i++;
        if (isEOFtoken(t))
            break;
    }
    //tk = pop() 

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