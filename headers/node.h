#ifndef NODE_H_
#define NODE_H_
#include "token.h"

typedef struct node node_t;

struct node
{
    token_t* token;
    int  depth;
    node_t* left;
    node_t* right;
    node_t** children;
    int num_children;
};

#endif