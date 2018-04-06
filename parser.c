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

    tk = (token_t*) pop((void**) tokenlist); 
    program(tokenlist);

    return i;
}

void
program(token_t** tokenlist) {
    if (strcmp(tk->id, "programTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        printf("OK\n");
    } else {
        printerror();
        return;
    }
    vars(tokenlist);
    block(tokenlist);
    return;
}

void
block(token_t** tokenlist) {

}

void
vars(token_t** tokenlist) {
    if (strcmp(tk->id, "varTK") == 0) {

    } else return;
}

void
mvars(token_t** tokenlist){}

void
expr(token_t** tokenlist){}

void
xhelp(token_t** tokenlist){}

void
M(token_t** tokenlist){}

void
R(token_t** tokenlist){}

void
stats(token_t** tokenlist){}

void
mstat(token_t** tokenlist){}

void
stat(token_t** tokenlist){}

void
in(token_t** tokenlist){}

void
out(token_t** tokenlist){}

void
iff(token_t** tokenlist){}

void
loop(token_t** tokenlist){}

void
assign(token_t** tokenlist){}

void
RO(token_t** tokenlist){}

void
lesshelp(token_t** tokenlist){}

void
greathelp(token_t** tokenlist){}

void
equalhelp(token_t** tokenlist){}

void
printerror(){
    fprintf(stderr, "Error parsing %s @ line %d.\n",
        tk->instance, tk->line_num);
    return;
}