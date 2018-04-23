#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "token.h"
#include "node.h"
#include "pop.h"

int
buildglobalstack(node_t* root, stack_t* stack)
{
    if (root == NULL)
        return 0;
    
    if(strcmp(root->token->instance, "<block>") == 0) {
        stack_t* local = (stack_t*) malloc(sizeof(stack_t));
        local->varstack = (token_t**) malloc(256*sizeof(token_t*));
        local->nvars = 0;
        local->tos = -1;
        if (checkundeclared(root, stack, local))
            return 1;
        return 0;
    }

    if(strcmp(root->token->id, "idTK") == 0) {
        if (!isinstack(root->token, stack)) {
            // add this identifier to stack
            addtostack(root->token, stack);
        } else {
            // already defined
            return 1;
        }
    } 

    int stk;
    int i;
    for (i = 0; i < root->num_children; i++) {
        stk = buildglobalstack(root->children[i], stack);
        if (stk > 0)
            return stk;
    }
    return 0;
}


int
checkundeclared(node_t* root, stack_t* stack, stack_t* local)
{
    if (root == NULL)
        return 0;

    //i  
    if(strcmp(root->token->instance, "<vars>") == 0) {
        //local = (stack_);
    }

    int ud;
    int i;
    for (i = 0; i < root->num_children; i++) {
        ud = checkundeclared(root->children[i], stack, NULL);
        if (ud > 0)
            return ud;
    }

    return 0;
}

void
addtostack(token_t* tk, stack_t* stack)
{
    int n = stack->nvars;
    stack->varstack[n] = (token_t*) malloc(sizeof(token_t));
    copytoken(stack->varstack[n], tk);
    stack->nvars++;
    stack->tos = stack->nvars - 1;
    return;
}

int
isinstack(token_t* tk, stack_t* stack)
{
    const char* varname = tk->instance;
    int i;
    for (i = 0; i < stack->nvars; i++) {
        const char* t = stack->varstack[i]->instance;
        if (strcmp(varname, t) == 0)
            return 1;
    }
    return 0;
}