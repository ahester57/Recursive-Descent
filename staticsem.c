
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "staticsem.h"
#include "tree.h"
#include "node.h"
#include "token.h"
#include "pop.h"
#include "stack.h"

int
analyzevars(node_t* root)
{
    //traversepreorder(root);
    stack_t* stack = (stack_t*) malloc(sizeof(stack_t));
    if (stack == (stack_t*)NULL) {
        return -1;
    }
    stack->varstack = (token_t**) malloc(256*sizeof(token_t*));
    stack->nvars = 0;
    stack->tos = -1;

    //buildglobalstack(root, stack);
    int stk = buildglobalstack(root, stack);

    if (stk > 0) {
        fprintf(stderr, "stk = %d \n", stk);
        return stk;
    }

    printf("Num Vars = %d \n", stack->nvars);
    int i;
    for (i = 0; i < stack->nvars; i++) {
        displaytoken(stack->varstack[i]);
    }
    return 0;
}
