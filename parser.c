#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "tree.h"
#include "treehelper.h"
#include "node.h"
#include "scanner.h"
#include "wordlist.h"
#include "token.h"
#include "first.h"
#include "pop.h"

static token_t* tk = NULL;
static int error = 0;
static int level = 0;

// return num tokens
node_t*
parser(node_t* root, token_t** tklist, token_t** tc, wordlist_t* filter, int* n)
{
    // build token list
    int i;
    while (1)
    {
        token_t* t = scanner(filter);
        if (t == (token_t*)NULL)
            return (node_t*)NULL;
        //displaytoken(t);
        tklist[i] = t;
        i++;
        if (isEOFtoken(t))
            break;
    }

    // deep copy token list
    copytokenlist(tc, tklist, i);
    displaytokens(tc, i);

    tk = customtoken("<>", "<program>", 0);
    root = NULL;
    root = insert(root, tk,  level++);

    tk = (token_t*) pop((void**) tklist);
    program(root, tklist);
 
    // set numtokens
    *n = i;
    return root;
}

void
program(node_t* root, token_t** tklist) {
    const char* FUNC = "program";


    if (strcmp(tk->id, "programTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        // vars nonterminal
        if (strcmp(tk->id, "varTK") == 0) {
            nonterminal(&vars, "<vars>", root, tklist);
        } 

        if (strcmp(tk->id, "startTK") == 0) {
            nonterminal(&block, "<block>", root, tklist);

        } else {
            printerror(FUNC);
            return;
        }
    } else {
        printerror(FUNC);
        return;
    }

    if (!error) {
        printf("PARSER\n");
        printf("OK\n");
    }
    return;
}


void
block(node_t* root, token_t** tklist) {
    const char* FUNC = "block";

    if (strcmp(tk->id, "startTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        // if there are vars
        if (strcmp(tk->id, "varTK") == 0) {
            nonterminal(&vars, "<vars>", root, tklist);
        } 
        if (first_stat(tk->id)) {
            // we got stats
            nonterminal(&stats, "<stats>", root, tklist);
        }

    } else {
        printerror(FUNC);
        return;
    }


    if (strcmp(tk->id, "stopTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
}

void
vars(node_t* root, token_t** tklist) {
    const char* FUNC = "vars";

    // start a vars
    if (strcmp(tk->id, "varTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    // if vars is empty
    } else {
        return;
    }

    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
    // check = or .
    if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }

    // check expression
    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0 || strcmp(tk->id, "%TK") == 0) {

        nonterminal(&expr, "<expr>", root, tklist);

    } else {
        printerror(FUNC);
        return;
    }

    // we have more vars
    if (strcmp(tk->id, ".TK") == 0 || strcmp(tk->id, ":TK") == 0) {
        nonterminal(&mvars, "<mvars>", root, tklist);
    }
    return;
}

void
mvars(node_t* root, token_t** tklist) {
    const char* FUNC = "mvars";

    if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else if (strcmp(tk->id, ":TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        if (strcmp(tk->id, "idTK") == 0) {
            insert(root, tk, level);
            tk = (token_t*) pop((void**) tklist); 

            if (strcmp(tk->id, ".TK") == 0 || strcmp(tk->id, ":TK") == 0) {

                nonterminal(&mvars, "<mvars>", root, tklist);
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
    // if there are more vars
    if (strcmp(tk->id, "varTK") == 0) {
        nonterminal(&vars, "<vars>", root, tklist);
        return;
    }
    return;
}

void
expr(node_t* root, token_t** tklist) {

    nonterminal(&M, "<M>", root, tklist);

    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
            strcmp(tk->id, "*TK") == 0 || strcmp(tk->id, "/TK") == 0) {

        nonterminal(&xhelp, "<xhelp>", root, tklist);
        return;
    } else {
        return;
    }
}

// this is not necessary to separate right now,
// but it should help later
void
xhelp(node_t* root, token_t** tklist) {

    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
        strcmp(tk->id, "/TK") == 0 || strcmp(tk->id, "*TK") == 0) {

        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        nonterminal(&expr, "<expr>", root, tklist);
    }

    return;
}

void
M(node_t* root, token_t** tklist) {
    const char* FUNC = "M";

    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0) {

        nonterminal(&R, "<R>", root, tklist);
        
        return;
    } else if (strcmp(tk->id, "%TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        nonterminal(&M, "<M>", root, tklist);

        return; 
    } else {
        printerror(FUNC);
        return;
    }
}

void
R(node_t* root, token_t** tklist) {
    const char* FUNC = "R";

    if (strcmp(tk->id, "(TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        nonterminal(&expr, "<expr>", root, tklist);

        if (strcmp(tk->id, ")TK") == 0) {
            insert(root, tk, level++);
            tk = (token_t*) pop((void**) tklist); 
            return;
        } else {
            printerror(FUNC);
            return;
        }
    } else if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, "intTK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
stats(node_t* root, token_t** tklist) {
    nonterminal(&stat, "<stat>", root, tklist);

    if (first_stat(tk->id)) {
        // there are more stats
        nonterminal(&mstat, "<mstat>", root, tklist);
    }
    return;
}

void
mstat(node_t* root, token_t** tklist) {
    if (first_stat(tk->id)) {
        // wow many stats  holy cow
        nonterminal(&stat, "<stat>", root, tklist);

        if (first_stat(tk->id)) {
            // there are more stats
            nonterminal(&mstat, "<mstat>", root, tklist);
        }
        return;
    } else {
        return;
    }
}

void
stat(node_t* root, token_t** tklist) {
    const char* FUNC = "stat";

    if (strcmp(tk->id, "readTK") == 0) {

        nonterminal(&in, "<in>", root, tklist);
        return;

    } else if (strcmp(tk->id, "printTK") == 0) {

        nonterminal(&out, "<out>", root, tklist);
        return;

    } else if (strcmp(tk->id, "startTK") == 0) {

        nonterminal(&block, "<block>", root, tklist);
        return;

    } else if (strcmp(tk->id, "iffTK") == 0) {

        nonterminal(&iffandloop, "<iff>", root, tklist);
        return;

    } else if (strcmp(tk->id, "iterTK") == 0) {

        nonterminal(&iffandloop, "<iter>", root, tklist);
        return;

    } else if (strcmp(tk->id, "letTK") == 0) {

        nonterminal(&assign, "<assign>", root, tklist);
        return;

    } else {
        printerror(FUNC);
        return;
    }
}

void
in(node_t* root, token_t** tklist) {
    const char* FUNC = "in";

    if (strcmp(tk->id, "readTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
    if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
    if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
out(node_t* root, token_t** tklist) {
    const char* FUNC = "out";

    if (strcmp(tk->id, "printTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    } 

    nonterminal(&expr, "<expr>", root, tklist);

    if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
assign(node_t* root, token_t** tklist) {
    const char* FUNC = "assign";

    if (strcmp(tk->id, "letTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    } 
    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
    // check =
    if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }

    nonterminal(&expr, "<expr>", root, tklist);

    if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
iffandloop(node_t* root, token_t** tklist) {
    const char* FUNC = "iffandloop";

    insert(root, tk, level);
    tk = (token_t*) pop((void**) tklist); 

    if (strcmp(tk->id, "(TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }

    nonterminal(&evaluate, "<evaluate>", root, tklist);

    if (strcmp(tk->id, ")TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }

    nonterminal(&stat, "<stat>", root, tklist);
    return;
}

void
evaluate(node_t* root, token_t** tklist)
{
    nonterminal(&expr, "<expr>", root, tklist);

    nonterminal(&RO, "<RO>", root, tklist);

    nonterminal(&expr, "<expr>", root, tklist);
    return;
}

void
RO(node_t* root, token_t** tklist) {
    const char* FUNC = "RO";

    // I deal with these using backtracking/ memoization
    // instead of lookahead. Check trimline.c
    if (strcmp(tk->id, "<TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, "<<TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, ">TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, ">>TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, "==TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
nonterminal(void (*nont)(node_t*,token_t**), char* name,
            node_t* root, token_t** tklist)
{
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;
    tmp = customtoken("<>", name, 0);
    lvl = level;
    root = insert(root, tmp, level++);
    nont(root->children[root->num_children-1], tklist);
    level = lvl;
}

void
printerror(const char* callingFunction){
    error = 1;
    fprintf(stderr, "Error parsing %s @ line %d.\t",
        tk->instance, tk->line_num);
    fprintf(stderr, "[[ in function %s ]]\n", callingFunction);
    return;
}