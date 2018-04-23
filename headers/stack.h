#ifndef STACK_H_
#define STACK_H_
#include "token.h"
#include "node.h"


typedef struct  {
    token_t** varstack;
    int nvars;
    int tos;
} stack_t;

void buildglobalstack(node_t* root, stack_t* stack);

#endif