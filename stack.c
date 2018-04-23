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
        int ls = buildlocalstack(root, stack, local, 1);

        printf("Num Vars = %d \n", local->nvars);
        int i;
        for (i = 0; i < local->nvars; i++) {
            displaytoken(local->varstack[i]);
        } 
        return ls;        
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
buildlocalstack(node_t* root, stack_t* stack, stack_t* local, int first)
{
    if (root == NULL)
        return 0;

    if(strcmp(root->token->instance, "<block>") == 0 &&
        !first) {
            printf("nother block!\n;");
        stack_t* newlocal = (stack_t*) malloc(sizeof(stack_t));
        newlocal->varstack = (token_t**) malloc(256*sizeof(token_t*));
        newlocal->nvars = 0;
        newlocal->tos = -1;
        int j;
        for (j = 0; j < local->nvars; j++) {
           addtostack(local->varstack[j], newlocal);
        }

        int ls = buildlocalstack(root, stack, newlocal, 1);

        printf("Num Vars = %d \n", newlocal->nvars);
        int i;
        for (i = 0; i < newlocal->nvars; i++) {
            displaytoken(newlocal->varstack[i]);
        } 
        return ls;        
    }

    if(strcmp(root->token->instance, "<vars>") == 0) {
        //buildglobalstack(root, local);
    }

    //i  
    if(strcmp(root->token->instance, "<stats>") == 0 || 
        strcmp(root->token->instance, "<stat>") == 0) {

        //checksomething();

    }

    if(strcmp(root->token->id, "idTK") == 0) {
        if (!isinstack(root->token, local)) {
            // add this identifier to stack
            addtostack(root->token, local);
        } else {
            // already defined
            //return 1;
        }
    }

    int ud;
    int i;
    for (i = 0; i < root->num_children; i++) {
        ud = buildlocalstack(root->children[i], stack, local, 0);
        if (ud > 0)
            return ud;
    }

    return 0;
}

int checkundeclared(node_t* root, stack_t* stack)
{
    if (root == NULL)
        return 0;
    // @TODO
    return 1;
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