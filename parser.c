#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "wordlist.h"
#include "token.h"
#include "pop.h"

static token_t* tk = NULL;
static int error = 0;

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
        //displaytoken(t);
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
    const char* FUNC = "program";
    if (strcmp(tk->id, "programTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    vars(tokenlist);
    block(tokenlist);
    printf("OK\n");
    return;
}

void
block(token_t** tokenlist) {
    const char* FUNC = "block";
    if (strcmp(tk->id, "startTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    vars(tokenlist);
    stats(tokenlist);
    if (strcmp(tk->id, "stopTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
}

void
vars(token_t** tokenlist) {
    const char* FUNC = "vars";
    // start a vars
    if (strcmp(tk->id, "varTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    // if vars is empty
    } else return;
    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    // check = or .
    if (strcmp(tk->id, "=TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
    // check integer
    if (strcmp(tk->id, "intTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    mvars(tokenlist);
    return;
}

void
mvars(token_t** tokenlist) {
    const char* FUNC = "mvars";
    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else if (strcmp(tk->id, ":TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        if (strcmp(tk->id, "idTK") == 0) {
            tk = (token_t*) pop((void**) tokenlist); 
            mvars(tokenlist);
            return;
        } else {
            printerror(FUNC);
            return;
        }
    } else {
        printerror(FUNC);
        return;
    }
}

void
expr(token_t** tokenlist) {
    M(tokenlist);
    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
            strcmp(tk->id, "*TK") == 0 || strcmp(tk->id, "/TK") == 0) {
        xhelp(tokenlist);
        return;
    } else return;
}

// this is not necessary to separate right now,
// but it should help later
void
xhelp(token_t** tokenlist) {
    if (strcmp(tk->id, "+TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(tokenlist);
        return;
    } else if (strcmp(tk->id, "-TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(tokenlist);
        return;
    } else if (strcmp(tk->id, "/TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(tokenlist);
        return;
    } else if (strcmp(tk->id, "*TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(tokenlist);
        return;
    } else return;
}

void
M(token_t** tokenlist) {
    const char* FUNC = "M";
    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0) {
        R(tokenlist);
        return;
    } else if (strcmp(tk->id, "%TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        M(tokenlist);
        return; 
    } else {
        printerror(FUNC);
        return;
    }
}

void
R(token_t** tokenlist) {
    const char* FUNC = "R";
    if (strcmp(tk->id, "(TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(tokenlist);
        if (strcmp(tk->id, ")TK") == 0) {
            tk = (token_t*) pop((void**) tokenlist); 
            return;
        } else {
            printerror(FUNC);
            return;
        }
    } else if (strcmp(tk->id, "idTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else if (strcmp(tk->id, "intTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
stats(token_t** tokenlist) {
    stat(tokenlist);
    mstat(tokenlist);
    return;
}

void
mstat(token_t** tokenlist) {
    if (strcmp(tk->id, "readTK") == 0 || strcmp(tk->id, "printTK") == 0 ||
        strcmp(tk->id, "iffTK") == 0 || strcmp(tk->id, "iterTK") == 0 ||
        strcmp(tk->id, "letTK") == 0 || strcmp(tk->id, "startTK") == 0) {
        stat(tokenlist);
        mstat(tokenlist);
        return;
    } else return;
}

void
stat(token_t** tokenlist) {
    const char* FUNC = "stat";
    if (strcmp(tk->id, "readTK") == 0) {
        in(tokenlist);
        return;
    } else if (strcmp(tk->id, "printTK") == 0) {
        out(tokenlist);
        return;
    } else if (strcmp(tk->id, "startTK") == 0) {
        block(tokenlist);
        return;
    } else if (strcmp(tk->id, "iffTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        iffandloop(tokenlist);
        return;
    } else if (strcmp(tk->id, "iterTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        iffandloop(tokenlist);
        return;
    } else if (strcmp(tk->id, "letTK") == 0) {
        assign(tokenlist);
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
in(token_t** tokenlist) {
    const char* FUNC = "in";
    if (strcmp(tk->id, "readTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    if (strcmp(tk->id, "idTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
out(token_t** tokenlist) {
    const char* FUNC = "out";
    if (strcmp(tk->id, "printTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    } 
    expr(tokenlist);
    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
iffandloop(token_t** tokenlist) {
    const char* FUNC = "iffandloop";
    if (strcmp(tk->id, "(TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }

    expr(tokenlist);
    RO(tokenlist);
    expr(tokenlist);

    if (strcmp(tk->id, ")TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }

    stat(tokenlist);
    return;
}

void
assign(token_t** tokenlist) {
    const char* FUNC = "assign";
    if (strcmp(tk->id, "letTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    } 
    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    // check =
    if (strcmp(tk->id, "=TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }

    expr(tokenlist);

    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
RO(token_t** tokenlist) {
    const char* FUNC = "RO";
    if (strcmp(tk->id, "<TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        lesshelp(tokenlist);
        return;
    } else if (strcmp(tk->id, ">TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        greathelp(tokenlist);
        return;
    } else if (strcmp(tk->id, "=TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        equalhelp(tokenlist);
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
lesshelp(token_t** tokenlist) { 
    if (strcmp(tk->id, "<TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else return;
}

void
greathelp(token_t** tokenlist) {
    if (strcmp(tk->id, ">TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else return;
}

void
equalhelp(token_t** tokenlist) {
    if (strcmp(tk->id, "=TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else return;
}

void
printerror(const char* callingFunction){
    error = 1;
    fprintf(stderr, "Error parsing %s @ line %d.\t",
        tk->instance, tk->line_num);
    fprintf(stderr, "[[ in function %s ]]\n", callingFunction);
    return;
}