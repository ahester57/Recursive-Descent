#ifndef STACK_H_
#define STACK_H_
#include "token.h"
#include "node.h"


typedef struct  {
    token_t** varstack;
    int nvars;
    int tos;
} stack_t;

int buildglobalstack(node_t* root, stack_t* stack);
int buildlocalstack(node_t* root, stack_t* stack, stack_t* local,
                    int first, int numborrowed);
int checkundeclared(node_t* root, stack_t* stack, stack_t* local);
void addtostack(token_t* tk, stack_t* stack);
int isinstack(token_t* tk, stack_t* stack);

#endif