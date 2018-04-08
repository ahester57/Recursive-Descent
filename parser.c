#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "tree.h"
#include "scanner.h"
#include "wordlist.h"
#include "token.h"
#include "pop.h"

static token_t* tk = NULL;
static int error = 0;
static int left = 0;
static int level = 0;

// return num tokens
int
parser(node_t* root, token_t** tokenlist, wordlist_t* filter) {
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
    program(root, tokenlist);

    return i;
}

void
program(node_t* root, token_t** tokenlist) {
    const char* FUNC = "program";
    insert(root, tk, left, level++);
    left = 1;
    if (strcmp(tk->id, "programTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    vars(root, tokenlist);
    block(root, tokenlist);
    if (!error)
        printf("OK\n");
    return;
}

void
block(node_t* root, token_t** tokenlist) {
    const char* FUNC = "block";
    insert(root, tk, left, level++);
    if (strcmp(tk->id, "startTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
    vars(root, tokenlist);
    stats(root, tokenlist);
    if (strcmp(tk->id, "stopTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }
}

void
vars(node_t* root, token_t** tokenlist) {
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
    mvars(root, tokenlist);
    return;
}

void
mvars(node_t* root, token_t** tokenlist) {
    const char* FUNC = "mvars";
    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else if (strcmp(tk->id, ":TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        if (strcmp(tk->id, "idTK") == 0) {
            tk = (token_t*) pop((void**) tokenlist); 
            mvars(root, tokenlist);
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
expr(node_t* root, token_t** tokenlist) {
    M(root, tokenlist);
    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
            strcmp(tk->id, "*TK") == 0 || strcmp(tk->id, "/TK") == 0) {
        xhelp(root, tokenlist);
        return;
    } else return;
}

// this is not necessary to separate right now,
// but it should help later
void
xhelp(node_t* root, token_t** tokenlist) {
    if (strcmp(tk->id, "+TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "-TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "/TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "*TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(root, tokenlist);
        return;
    } else return;
}

void
M(node_t* root, token_t** tokenlist) {
    const char* FUNC = "M";
    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0) {
        R(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "%TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        M(root, tokenlist);
        return; 
    } else {
        printerror(FUNC);
        return;
    }
}

void
R(node_t* root, token_t** tokenlist) {
    const char* FUNC = "R";
    if (strcmp(tk->id, "(TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        expr(root, tokenlist);
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
stats(node_t* root, token_t** tokenlist) {
    stat(root, tokenlist);
    mstat(root, tokenlist);
    return;
}

void
mstat(node_t* root, token_t** tokenlist) {
    if (strcmp(tk->id, "readTK") == 0 || strcmp(tk->id, "printTK") == 0 ||
        strcmp(tk->id, "iffTK") == 0 || strcmp(tk->id, "iterTK") == 0 ||
        strcmp(tk->id, "letTK") == 0 || strcmp(tk->id, "startTK") == 0) {
        stat(root, tokenlist);
        mstat(root, tokenlist);
        return;
    } else return;
}

void
stat(node_t* root, token_t** tokenlist) {
    const char* FUNC = "stat";
    if (strcmp(tk->id, "readTK") == 0) {
        in(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "printTK") == 0) {
        out(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "startTK") == 0) {
        block(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "iffTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        iffandloop(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "iterTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        iffandloop(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "letTK") == 0) {
        assign(root, tokenlist);
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
in(node_t* root, token_t** tokenlist) {
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
out(node_t* root, token_t** tokenlist) {
    const char* FUNC = "out";
    if (strcmp(tk->id, "printTK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    } 
    expr(root, tokenlist);
    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
iffandloop(node_t* root, token_t** tokenlist) {
    const char* FUNC = "iffandloop";
    if (strcmp(tk->id, "(TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }

    expr(root, tokenlist);
    RO(root, tokenlist);
    expr(root, tokenlist);

    if (strcmp(tk->id, ")TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
    } else {
        printerror(FUNC);
        return;
    }

    stat(root, tokenlist);
    return;
}

void
assign(node_t* root, token_t** tokenlist) {
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

    expr(root, tokenlist);

    if (strcmp(tk->id, ".TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
RO(node_t* root, token_t** tokenlist) {
    const char* FUNC = "RO";
    if (strcmp(tk->id, "<TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        lesshelp(root, tokenlist);
        return;
    } else if (strcmp(tk->id, ">TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        greathelp(root, tokenlist);
        return;
    } else if (strcmp(tk->id, "=TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        equalhelp(root, tokenlist);
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
lesshelp(node_t* root, token_t** tokenlist) { 
    if (strcmp(tk->id, "<TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else return;
}

void
greathelp(node_t* root, token_t** tokenlist) {
    if (strcmp(tk->id, ">TK") == 0) {
        tk = (token_t*) pop((void**) tokenlist); 
        return;
    } else return;
}

void
equalhelp(node_t* root, token_t** tokenlist) {
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