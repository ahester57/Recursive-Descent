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
    } else {
        printerror();
        return;
    }
    vars(tokenlist);
    block(tokenlist);
    printf("OK\n");
    return;
}

void
block(token_t** tokenlist) {
    if (strcmp(tk->id, "startTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror();
        return;
    }
    vars(tokenlist);
    stats(tokenlist);
    if (strcmp(tk->id, "stopTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror();
        return;
    }
}

void
vars(token_t** tokenlist) {
    // start a vars
    if (strcmp(tk->id, "varTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        // check identifier
        if (strcmp(tk->id, "idTK") == 0) {
            tk = (token_t*) pop((void**) tokenlist); 
        } else {
            printerror();
            return;
        }
        // check =
        if (strcmp(tk->id, "=TK") == 0) {
            tk = (token_t*) pop((void**) tokenlist); 
        } else {
            printerror();
            return;
        }
        // check integer
        if (strcmp(tk->id, "intTK") == 0) {
            tk = (token_t*) pop((void**) tokenlist); 
        } else {
            printerror();
            return;
        }
        mvars(tokenlist);
        return;
    // if vars is empty
    } else return;
}

void
mvars(token_t** tokenlist){}

void
expr(token_t** tokenlist) {
    M(tokenlist);
    if (strcmp(tk->id, "+TK") || strcmp(tk->id, "-TK") ||
            strcmp(tk->id, "*TK") || strcmp(tk->id, "/TK")) {
        tk = (token_t*) pop((void**) tokenlist); 
        xhelp(tokenlist);
        return;
    }
    return;
}

void
xhelp(token_t** tokenlist){}

void
M(token_t** tokenlist){}

void
R(token_t** tokenlist){}

void
stats(token_t** tokenlist) {
    stat(tokenlist);
    mstat(tokenlist);
    return;
}

void
mstat(token_t** tokenlist) {
    
}

void
stat(token_t** tokenlist) {
    if (strcmp(tk->id, "readTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        in(tokenlist);
    } else if (strcmp(tk->id, "printTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        out(tokenlist);
    } else {
        printerror();
        return;
    }
}

void
in(token_t** tokenlist){}

void
out(token_t** tokenlist) {

}

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