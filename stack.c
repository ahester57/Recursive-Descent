#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "token.h"
#include "node.h"
#include "pop.h"

void
buildglobalstack(node_t* root, stack_t* stack)
{
    if (root == NULL)
        return;
    
    if(strcmp(root->token->instance, "<block>") == 0)
        return;

    if(strcmp(root->token->id, "idTK") == 0) {
        // add this identifier to stack
        int n = stack->nvars;
        stack->varstack[n] = (token_t*) malloc(sizeof(token_t));
        copytoken(stack->varstack[n], root->token);
        stack->nvars++;
        stack->tos = stack->nvars - 1;
    } 

    int i;
    for (i = 0; i < root->num_children; i++) {
        buildglobalstack(root->children[i], stack);
    }
}