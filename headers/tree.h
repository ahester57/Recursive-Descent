#ifndef TREE_H_
#define TREE_H_
#include "node.h"
#include "wordlist.h"
#include "token.h"

node_t* buildtree(token_t** word, int n);
node_t* insert(node_t* root, token_t* token, int level);

#endif