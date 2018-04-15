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
const int LEFT = 0;
const int RIGHT = 1;
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
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;


    if (strcmp(tk->id, "programTK") == 0) {
        insert(root, tk,  level);
        tk = (token_t*) pop((void**) tklist); 

        // vars nonterminal
        if (strcmp(tk->id, "varTK") == 0) {
            tmp = customtoken("<>", "<vars>", 0);
            lvl = level;
            insert(root, tmp,  level++);
            vars(root->children[root->num_children-1], tklist);
            level = lvl;
        } 

        if (strcmp(tk->id, "startTK") == 0) {
            tmp = customtoken("<>", "<block>", 0);
            lvl = level;
            insert(root, tmp,  level++);
            block(root->children[root->num_children-1], tklist);
            level = lvl;

        } else {
            printerror(FUNC);
            free(tmp);
            return;
        }
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }
    free(tmp);

    if (!error) {
        printf("PARSER\n");
        printf("OK\n");
    }
    return;
}

void
nonterminal(void (*nonterminal)(node_t*,token_t**), char* name)
{

}

void
block(node_t* root, token_t** tklist) {
    const char* FUNC = "block";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;


    if (strcmp(tk->id, "startTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        // if there are vars
        if (strcmp(tk->id, "varTK") == 0) {
            tmp = customtoken("<>", "<vars>", 0);
            lvl = level;
            root = insert(root, tmp,  level++);
            vars(root->children[root->num_children-1], tklist);
            level = lvl;
        } 
        if (first_stat(tk->id)) {
            // we got stats
            tmp = customtoken("<>", "<stats>", 0);
            lvl = level;
            root = insert(root, tmp,  level++);
            stats(root->children[root->num_children-1], tklist);
            level = lvl;
        }

        free(tmp);
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }


    if (strcmp(tk->id, "stopTK") == 0) {
        insert(root, tk, lvl);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
}

void
vars(node_t* root, token_t** tklist) {
    const char* FUNC = "vars";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    // start a vars
    if (strcmp(tk->id, "varTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    // if vars is empty
    } else {
        free(tmp);
        return;
    }

    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }
    // check = or .
    if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        free(tmp);
        return;
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }

    // check expression
    lvl = level;
    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0 || strcmp(tk->id, "%TK") == 0) {

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);
        level = lvl;

    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }

    level = lvl;
    // we have more vars
    if (strcmp(tk->id, ".TK") == 0 || strcmp(tk->id, ":TK") == 0) {
        tmp = customtoken("<>", "<mvars>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        mvars(root->children[root->num_children-1], tklist);
        level = lvl;
    }
    free(tmp);
    return;
}

void
mvars(node_t* root, token_t** tklist) {
    const char* FUNC = "mvars";
    int lvl = level;

    if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else if (strcmp(tk->id, ":TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        if (strcmp(tk->id, "idTK") == 0) {
            insert(root, tk, level);
            tk = (token_t*) pop((void**) tklist); 
            token_t* tmp = (token_t*) malloc(sizeof(token_t));
            if (strcmp(tk->id, ".TK") == 0 || strcmp(tk->id, ":TK") == 0) {
                tmp = customtoken("<>", "<mvars>", 0);
                lvl = level;
                root = insert(root, tmp,  level++);
                mvars(root->children[root->num_children-1], tklist);
                level = lvl;

                return;
            } else {
                printerror(FUNC);
                free(tmp);
                return;
            }
        } else {
            printerror(FUNC);
            return;
        }
    }
    // if there are more vars
    if (strcmp(tk->id, "varTK") == 0) {
        token_t* tmp = (token_t*) malloc(sizeof(token_t));
        tmp = customtoken("<>", "<vars>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        vars(root->children[root->num_children-1], tklist);
        level = lvl;
        free(tmp);
        return;
    }
    return;
}

void
expr(node_t* root, token_t** tklist) {
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    tmp = customtoken("<>", "<M>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    M(root->children[root->num_children-1], tklist);
    level = lvl;

    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
            strcmp(tk->id, "*TK") == 0 || strcmp(tk->id, "/TK") == 0) {
        tmp = customtoken("<>", "<xhelp>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        xhelp(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;
    } else {
        free(tmp);
        return;
    }
}

// this is not necessary to separate right now,
// but it should help later
void
xhelp(node_t* root, token_t** tklist) {
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
        strcmp(tk->id, "/TK") == 0 || strcmp(tk->id, "*TK") == 0) {

        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);
        level = lvl;
    }

    free(tmp);
    return;
}

void
M(node_t* root, token_t** tklist) {
    const char* FUNC = "M";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0) {

        tmp = customtoken("<>", "<R>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        R(root->children[root->num_children-1], tklist);
        level = lvl;
        
        free(tmp);
        return;
    } else if (strcmp(tk->id, "%TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<M>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        M(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return; 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }
}

void
R(node_t* root, token_t** tklist) {
    const char* FUNC = "R";
    int lvl = level;

    if (strcmp(tk->id, "(TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        token_t* tmp = (token_t*) malloc(sizeof(token_t));
        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);
        level = lvl;
        free(tmp);

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
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    tmp = customtoken("<>", "<stat>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    stat(root->children[root->num_children-1], tklist);
    level = lvl;

    if (first_stat(tk->id)) {
        // there are more stats
        tmp = customtoken("<>", "<mstat>", 0);
        level = lvl;
        root = insert(root, tmp,  level++);
        mstat(root->children[root->num_children-1], tklist);
        level = lvl;
    }

    free(tmp);
    return;
}

void
mstat(node_t* root, token_t** tklist) {
    int lvl = level;

    if (first_stat(tk->id)) {
        // wow many stats  holy cow
        token_t* tmp = (token_t*) malloc(sizeof(token_t));

        tmp = customtoken("<>", "<stat>", 0);
        level = lvl;
        root = insert(root, tmp,  level++);
        stat(root->children[root->num_children-1], tklist);
        level = lvl;

        if (first_stat(tk->id)) {
            // there are more stats
            tmp = customtoken("<>", "<mstat>", 0);
            level = lvl;
            root = insert(root, tmp,  level++);
            mstat(root->children[root->num_children-1], tklist);
            level = lvl;
        }

        free(tmp);
        return;
    } else {
        return;
    }
}

void
stat(node_t* root, token_t** tklist) {
    const char* FUNC = "stat";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "readTK") == 0) {

        tmp = customtoken("<>", "<in>", 0);
        lvl = level;
        insert(root, tmp,  level++);
        in(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;

    } else if (strcmp(tk->id, "printTK") == 0) {

        tmp = customtoken("<>", "<out>", 0);
        lvl = level;
        insert(root, tmp,  level++);
        out(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;

    } else if (strcmp(tk->id, "startTK") == 0) {

        tmp = customtoken("<>", "<block>", 0);
        lvl = level;
        insert(root, tmp,  level++);
        block(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;

    } else if (strcmp(tk->id, "iffTK") == 0) {

        tmp = customtoken("<>", "<iff>", 0);
        lvl = level;
        insert(root, tmp,  level++);

        insert(root->children[root->num_children-1], tk, level);
        tk = (token_t*) pop((void**) tklist); 

        iffandloop(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;

    } else if (strcmp(tk->id, "iterTK") == 0) {

        tmp = customtoken("<>", "<iter>", 0);
        lvl = level;
        insert(root, tmp,  level++);

        insert(root->children[root->num_children-1], tk, level);
        tk = (token_t*) pop((void**) tklist); 

        iffandloop(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;

    } else if (strcmp(tk->id, "letTK") == 0) {

        tmp = customtoken("<>", "<assign>", 0);
        lvl = level;
        insert(root, tmp,  level++);
        assign(root->children[root->num_children-1], tklist);
        level = lvl;

        free(tmp);
        return;

    } else {
        printerror(FUNC);
        free(tmp);
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
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "printTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    } 

    tmp = customtoken("<>", "<expr>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    expr(root->children[root->num_children-1], tklist);
    level = lvl;
    free(tmp);

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
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "letTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    } 
    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }
    // check =
    if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }

    tmp = customtoken("<>", "<expr>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    expr(root->children[root->num_children-1], tklist);
    level = lvl;
    free(tmp);

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
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "(TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }

    tmp = customtoken("<>", "<evaluate>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    evaluate(root->children[root->num_children-1], tklist);
    level = lvl;

    if (strcmp(tk->id, ")TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        free(tmp);
        return;
    }

    tmp = customtoken("<>", "<stat>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    stat(root->children[root->num_children-1], tklist);
    level = lvl;

    free(tmp);
    return;
}

void
evaluate(node_t* root, token_t** tklist)
{
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    tmp = customtoken("<>", "<expr>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    expr(root->children[root->num_children-1], tklist);
    level = lvl;

    tmp = customtoken("<>", "<RO>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    RO(root->children[root->num_children-1], tklist);
    level = lvl;

    tmp = customtoken("<>", "<expr>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    expr(root->children[root->num_children-1], tklist);
    level = lvl;

    free(tmp);
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
printerror(const char* callingFunction){
    error = 1;
    fprintf(stderr, "Error parsing %s @ line %d.\t",
        tk->instance, tk->line_num);
    fprintf(stderr, "[[ in function %s ]]\n", callingFunction);
    return;
}