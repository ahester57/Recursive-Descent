#ifndef TREEHELPER_H_
#define TREEHELPER_H_
#include "tree.h"

node_t* initializenode(node_t* root, int len, int level);
void addwordtonode(node_t* root, char* word);
int isinnode(node_t* node, char* word);
void traverseinorder(node_t* root, FILE* fp);
void traversepreorder(node_t* root, FILE* fp);
void traversepostorder(node_t* root, FILE* fp);
void printnode(node_t* node);

#endif