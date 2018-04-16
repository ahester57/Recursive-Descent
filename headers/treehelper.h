#ifndef TREEHELPER_H_
#define TREEHELPER_H_
#include "tree.h"
#include "token.h"

node_t* initializenode(node_t* root, int len, int level);
void addwordtonode(node_t* root, token_t* token);
void traversepreorder(node_t* root);
void printnode(node_t* node);

#endif