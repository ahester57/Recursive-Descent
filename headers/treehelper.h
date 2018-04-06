#ifndef TREEHELPER_H_
#define TREEHELPER_H_
#include "tree.h"

node_t* initializenode(node_t* root, int level);
void addtokentonode (node_t* root, token_t* token);
void traverseinorder(node_t* root);
void traversepreorder(node_t* root);
void traversepostorder(node_t* root);
void printnode(node_t* node);

#endif