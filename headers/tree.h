#ifndef TREE_H_
#define TREE_H_
#include "node.h"
#include "token.h"

node_t* insert(node_t* root, token_t* token, int left, int level);

#endif