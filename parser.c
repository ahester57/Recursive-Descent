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

    tk = customtoken("<>", "<program>", 0);
    root = NULL;
    root = insert(root, tk,  level++);

    tk = (token_t*) pop((void**) tklist);
    program(root, tklist);
    printf("fuck\n");
 
    //displaytokens(tc, i);
    //printf("~%d\n", root->num_children);
    traversepreorder(root);

    // set numtokens
    *n = i;
    return root;
}

void
program(node_t* root, token_t** tklist) {
    const char* FUNC = "program";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;
    printf("fuck\n");


    if (strcmp(tk->id, "programTK") == 0) {
        insert(root, tk,  level);
        tk = (token_t*) pop((void**) tklist); 

        // vars nonterminal
        if (strcmp(tk->id, "varTK") == 0) {
            level = lvl;
            tmp = customtoken("<>", "<vars>", 0);
            insert(root, tmp,  lvl);
            vars(root->children[root->num_children-1], tklist);
        } 

        if (strcmp(tk->id, "startTK") == 0) {
            tmp = customtoken("<>", "<block>", 0);
            insert(root, tmp,  lvl);
            level = lvl;
            block(root->children[root->num_children-1], tklist);

        } else {
            printerror(FUNC);
            return;
        }
    } else {
        printerror(FUNC);
        return;
    }

    if (!error)
        printf("OK\n");
    return;
}

void
block(node_t* root, token_t** tklist) {
    const char* FUNC = "block";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;


    if (strcmp(tk->id, "startTK") == 0) {
        insert(root, tk, ++level);
        tk = (token_t*) pop((void**) tklist); 

        // if there are vars
        if (strcmp(tk->id, "varTK") == 0) {
            tmp = customtoken("<>", "<vars>", 0);
            lvl = level;
            root = insert(root, tmp,  ++level);
            vars(root->children[root->num_children-1], tklist);
            level = lvl;
        } 
        printf("fuckkkkkkkkkk\n");
        if (strcmp(tk->id, "readTK") == 0 || strcmp(tk->id, "printTK") == 0 ||
            strcmp(tk->id, "iffTK") == 0 || strcmp(tk->id, "iterTK") == 0 ||
            strcmp(tk->id, "letTK") == 0 || strcmp(tk->id, "startTK") == 0) {
            // we got stats
            tmp = customtoken("<>", "<stats>", 0);
            lvl = level;
            root = insert(root, tmp,  level++);
            stats(root->children[root->num_children-1], tklist);
        }

    } else {
        printerror(FUNC);
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
        insert(root, tk, ++level);
        tk = (token_t*) pop((void**) tklist); 
    // if vars is empty
    } else return;

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

    // check integer
    lvl = level;
    if (strcmp(tk->id, "(TK") == 0 || strcmp(tk->id, "idTK") == 0 ||
            strcmp(tk->id, "intTK") == 0 || strcmp(tk->id, "%TK") == 0) {
        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);
    } else {
        printerror(FUNC);
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
        //mvars(root, tklist);
    }
    return;
}

void
mvars(node_t* root, token_t** tklist) {
    const char* FUNC = "mvars";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, ".TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else if (strcmp(tk->id, ":TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
        if (strcmp(tk->id, "idTK") == 0) {
            insert(root, tk, level);
            tk = (token_t*) pop((void**) tklist); 
            if (strcmp(tk->id, ".TK") == 0 || strcmp(tk->id, ":TK") == 0) {
                tmp = customtoken("<>", "<mvars>", 0);
                lvl = level;
                root = insert(root, tmp,  level++);
                mvars(root->children[root->num_children-1], tklist);
                level = lvl;
            }
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
expr(node_t* root, token_t** tklist) {
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    tmp = customtoken("<>", "<M>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    M(root->children[root->num_children-1], tklist);

    if (strcmp(tk->id, "+TK") == 0 || strcmp(tk->id, "-TK") == 0 ||
            strcmp(tk->id, "*TK") == 0 || strcmp(tk->id, "/TK") == 0) {
        tmp = customtoken("<>", "<xhelp>", 0);
        root = insert(root, tmp,  lvl++);
        level = lvl;
        xhelp(root->children[root->num_children-1], tklist);
        return;
    } else return;
}

// this is not necessary to separate right now,
// but it should help later
void
xhelp(node_t* root, token_t** tklist) {
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "+TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);

        return;
    } else if (strcmp(tk->id, "-TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);

        return;
    } else if (strcmp(tk->id, "/TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);

        return;
    } else if (strcmp(tk->id, "*TK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);

        return;
    } else return;
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
        
        return;
    } else if (strcmp(tk->id, "%TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<M>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        M(root->children[root->num_children-1], tklist);

        return; 
    } else {
        printerror(FUNC);
        return;
    }
}

void
R(node_t* root, token_t** tklist) {
    const char* FUNC = "R";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "(TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        tmp = customtoken("<>", "<expr>", 0);
        lvl = level;
        root = insert(root, tmp,  level++);
        expr(root->children[root->num_children-1], tklist);

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

    if (strcmp(tk->id, "readTK") == 0 || strcmp(tk->id, "printTK") == 0 ||
        strcmp(tk->id, "iffTK") == 0 || strcmp(tk->id, "iterTK") == 0 ||
        strcmp(tk->id, "letTK") == 0 || strcmp(tk->id, "startTK") == 0) {
        // there are more stats
        tmp = customtoken("<>", "<mstat>", 0);
        level = lvl;
        root = insert(root, tmp,  level);
        mstat(root->children[root->num_children-1], tklist);
        level = lvl;
    }

    return;
}

void
mstat(node_t* root, token_t** tklist) {
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "readTK") == 0 || strcmp(tk->id, "printTK") == 0 ||
        strcmp(tk->id, "iffTK") == 0 || strcmp(tk->id, "iterTK") == 0 ||
        strcmp(tk->id, "letTK") == 0 || strcmp(tk->id, "startTK") == 0) {
        // wow many stats  holy cow

        tmp = customtoken("<>", "<stat>", 0);
        level = lvl;
        root = insert(root, tmp,  level);
        stat(root->children[root->num_children-1], tklist);
        level = lvl;

        if (strcmp(tk->id, "readTK") == 0 || strcmp(tk->id, "printTK") == 0 ||
            strcmp(tk->id, "iffTK") == 0 || strcmp(tk->id, "iterTK") == 0 ||
            strcmp(tk->id, "letTK") == 0 || strcmp(tk->id, "startTK") == 0) {
            // there are more stats
            tmp = customtoken("<>", "<mstat>", 0);
            level = lvl;
            root = insert(root, tmp,  level);
            mstat(root->children[root->num_children-1], tklist);
        }

        return;
    } else return;
}

void
stat(node_t* root, token_t** tklist) {
    const char* FUNC = "stat";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "readTK") == 0) {
        in(root, tklist);
        return;
    } else if (strcmp(tk->id, "printTK") == 0) {
        out(root, tklist);
        return;
    } else if (strcmp(tk->id, "startTK") == 0) {

        tmp = customtoken("<>", "<block>", 0);
        insert(root, tmp,  lvl);
        level = lvl;
        block(root->children[root->num_children-1], tklist);
        return;

    } else if (strcmp(tk->id, "iffTK") == 0) {

        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        iffandloop(root, tklist);
        return;

    } else if (strcmp(tk->id, "iterTK") == 0) {

        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        iffandloop(root, tklist);
        return;

    } else if (strcmp(tk->id, "letTK") == 0) {
        assign(root, tklist);
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
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "printTK") == 0) {
        insert(root, tk, level);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    } 

    tmp = customtoken("<>", "<expr>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    expr(root->children[root->num_children-1], tklist);

    level = lvl;
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
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }

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

    if (strcmp(tk->id, ")TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }

    tmp = customtoken("<>", "<stat>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    stat(root->children[root->num_children-1], tklist);
    level = lvl;

    return;
}

void
assign(node_t* root, token_t** tklist) {
    const char* FUNC = "assign";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "letTK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    } 
    // check identifier
    if (strcmp(tk->id, "idTK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }
    // check =
    if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
    } else {
        printerror(FUNC);
        return;
    }

    tmp = customtoken("<>", "<expr>", 0);
    lvl = level;
    root = insert(root, tmp,  level++);
    expr(root->children[root->num_children-1], tklist);
    level = lvl;

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
RO(node_t* root, token_t** tklist) {
    const char* FUNC = "RO";
    token_t* tmp = (token_t*) malloc(sizeof(token_t));
    int lvl = level;

    if (strcmp(tk->id, "<TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        if (strcmp(tk->id, "<TK") == 0) {
            tmp = customtoken("<>", "<lesshelp>", 0);
            lvl = level;
            root = insert(root, tmp,  level++);
            lesshelp(root->children[root->num_children-1], tklist);
            level = lvl;
        }

        return;
    } else if (strcmp(tk->id, ">TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        if (strcmp(tk->id, ">TK") == 0) {
            tmp = customtoken("<>", "<greathelp>", 0);
            lvl = level;
            root = insert(root, tmp,  level++);
            greathelp(root->children[root->num_children-1], tklist);
            level = lvl;
        }

        return;
    } else if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 

        if (strcmp(tk->id, "=TK") == 0) {
            tmp = customtoken("<>", "<equalhelp>", 0);
            lvl = level;
            root = insert(root, tmp,  level++);
            equalhelp(root->children[root->num_children-1], tklist);
            level = lvl;
        }

        return;
    } else {
        printerror(FUNC);
        return;
    }
}

void
lesshelp(node_t* root, token_t** tklist) { 
    if (strcmp(tk->id, "<TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else return;
}

void
greathelp(node_t* root, token_t** tklist) {
    if (strcmp(tk->id, ">TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
        return;
    } else return;
}

void
equalhelp(node_t* root, token_t** tklist) {
    if (strcmp(tk->id, "=TK") == 0) {
        insert(root, tk, level++);
        tk = (token_t*) pop((void**) tklist); 
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