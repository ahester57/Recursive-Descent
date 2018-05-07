/* Austin Hester
CS 4280 sp18
C.Z. Janikow */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "token.h"
#include "node.h"
#include "pop.h"

/*
    There is so much recursion here that I would not 
    recommend even looking at this unless you enjoy
    hurting your brain.

    Note: build*stack(...) are for static semantics.
    They are quite complicated. buildglobalstack() either
    directly and/or indirectly calls every function except
    the functions that begin with 'just'

    // buildlocalstack is the most complicated. 
    // it checks for variables in all of the parent stacks
    // as well as its own stack.
    // It recursively calls itself recursively, recursively,
    // if there is a block inside of a block. beware.

    Also note: justbuild*stack(...) are for code gen.
    They make much more sense. What they do not do is go
    deeper into each block or statement.

*/

// Build the global var stack
// for static semantics
int
buildglobalstack(node_t* root, stack_t* stack)
{
    if (root == NULL)
        return 0;
    
    // If we enter a block
    if(strcmp(root->token->instance, "<block>") == 0) {
        // Create a new local stack
        stack_t* local = (stack_t*) malloc(4*sizeof(stack_t));
        if (local == (stack_t*)NULL)
            return -1;
        local->varstack = (token_t**) malloc(128*sizeof(token_t*));
        local->nvars = 0;
        local->tos = -1;
        // Build the local stack
        // Also does checking for use of undefined vars
        int ls = buildlocalstack(root, stack, local, 1, 0);
         printf("Num Vars = %d \n", local->nvars);
        int i;
        for (i = 0; i < local->nvars; i++) {
            displaytoken(local->varstack[i]);
        }  
        free(local);
        return ls;        
    } 
    
    // Add the new var
    if(strcmp(root->token->id, "idTK") == 0) {
        if (isinstack(root->token, stack) == -1) {
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
        // recursion
        stk = buildglobalstack(root->children[i], stack);
        if (stk > 0)
            return stk;
    }
    return 0;
}

// For code generation
int
justbuildglobalstack(node_t* root, stack_t* stack)
{
    if (root == NULL)
        return 0;
    
    // If we enter a block
    if(strcmp(root->token->instance, "<block>") == 0) {
            fprintf(stderr, "[BLOC:] SHIT\n");
        return 0;
    } 
    
    // Add the new var
    if(strcmp(root->token->id, "idTK") == 0) {
        if (isinstack(root->token, stack) == -1) {
            // add this identifier to stack
            addtostack(root->token, stack);
        } else {
            // already defined
            //addtostack(root->token, stack);
            fprintf(stderr, "[BLOC:] NOOP\n");
            return 1;
        }
    } 

    int stk;
    int i;
    for (i = 0; i < root->num_children; i++) {
        // recursion
        stk = justbuildglobalstack(root->children[i], stack);
        if (stk > 0)
            return stk;
    }
    return 0;
}

int
justbuildlocalstack(node_t* root, stack_t* stack)
{
    if (root == NULL)
        return 0;
    
    // If we enter a block
    if(strcmp(root->token->instance, "<block>") == 0) {
            fprintf(stderr, "[BLOC:] SHIT\n");
        return 0;
    } 

    if( strcmp(root->token->instance, "<in>") == 0 || 
        strcmp(root->token->instance, "<out>") == 0 ||
        strcmp(root->token->instance, "<iff>") == 0 ||
        strcmp(root->token->instance, "<iter>") == 0 ||
        strcmp(root->token->instance, "<assign>") == 0) {
        // Check for the use of vars
        return 1;  
    }
    
    // Add the new var
    if(strcmp(root->token->id, "idTK") == 0) {
        if (isinstack(root->token, stack) == -1) {
            // add this identifier to stack
            addtostack(root->token, stack);
        } else {
            // already defined
            //addtostack(root->token, stack);
            fprintf(stderr, "[BLOC:] NOOP\n");
            return 1;
        }
    } 

    int stk;
    int i;
    for (i = 0; i < root->num_children; i++) {
        // recursion
        stk = justbuildlocalstack(root->children[i], stack);
        if (stk > 0)
            return stk;
    }
    return 0;
}

int
buildlocalstack(node_t* root, stack_t* stack, stack_t* local,
                int first, int numborrowed)
{
    if (root == NULL)
        return 0;

    if (root->token == NULL || root->token->instance == NULL) {
        fprintf(stderr, "Memory error. Retry.\n");
        return -1;
    }

    // If we enter an even deeper scope
    if(strcmp(root->token->instance, "<block>") == 0 &&
        !first) {
            //printf("nother block!\n;");
        // Create a new local stack
        stack_t* newlocal = (stack_t*) malloc(4*sizeof(stack_t));
        if (newlocal == (stack_t*)NULL)
            return -1;
        newlocal->varstack = (token_t**) malloc(128*sizeof(token_t*));
        newlocal->nvars = 0;
        newlocal->tos = -1;
        // Add the current local stack
        int j;
        for (j = 0; j < local->nvars; j++) {
           addtostack(local->varstack[j], newlocal);
           // Don't worry, `numborrowed` allows overwriting 
        }
        // Build the local stack
        // Also does checking for use of undefined vars
        /* meta-recursion */
        int ls = buildlocalstack(root, stack, newlocal, 1, newlocal->nvars);
        int i;
        for (i = 0; i < newlocal->nvars; i++) {
            displaytoken(newlocal->varstack[i]);
        }  
        // Free the now useless stack
        free(newlocal);
        return ls;        
    }

    // Check to make sure variables are declared.
    if( strcmp(root->token->instance, "<in>") == 0 || 
        strcmp(root->token->instance, "<out>") == 0 ||
        strcmp(root->token->instance, "<iff>") == 0 ||
        strcmp(root->token->instance, "<iter>") == 0 ||
        strcmp(root->token->instance, "<assign>") == 0) {
        // Check for the use of vars
        // call to recursive function
        int u = checkundeclared(root, stack, local);
        return u;  
    }

    if(strcmp(root->token->id, "idTK") == 0) {
        // Check if its been added in this scope already
        if (isinstack(root->token, local+numborrowed) == -1) {
            // add this identifier to stack
            addtostack(root->token, local);
        } else {
            // already defined, replace
            fprintf(stderr, "\nvar %s already defined: line %d\n",
                                root->token->instance,
                                root->token->line_num);
            return 1; 
        }
    }

    int ud;
    int i;
    for (i = 0; i < root->num_children; i++) {
        // recursion
        ud = buildlocalstack(root->children[i], stack, local, 0, numborrowed);
        if (ud > 0)
            return ud;
    }
    return 0;
}

// Check if variable is in either stack
int
checkundeclared(node_t* root, stack_t* stack, stack_t* local)
{
    if (root == NULL)
        return 0;

    if(strcmp(root->token->id, "idTK") == 0) {
        if (isinstack(root->token, local) == -1 &&
            isinstack(root->token, stack) == -1) {
            // variable undefined
            fprintf(stderr, "\nvar %s undefined: line %d\n",
                                root->token->instance,
                                root->token->line_num);
            return 2;
        }
    }

    int ud;
    int i;
    for (i = 0; i < root->num_children; i++) {
        // recursion
        ud = checkundeclared(root->children[i], stack, local);
        if (ud > 0)
            return ud;
    }
    return 0;
}

// Add given token to given stack
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

// return 1 if tk is in stack
// return 0 otherwise
int
isinstack(token_t* tk, stack_t* stack)
{
    const char* varname = tk->instance;
    int i;
    for (i = 0; i < stack->nvars; i++) {
        const char* t = stack->varstack[i]->instance;
        if (strcmp(varname, t) == 0)
            return i;
    }
    return -1;
}