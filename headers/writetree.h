#ifndef WRITETREE_H_
#define WRITETREE_H_
#include <stdio.h>
#include "tree.h"

void writeinorder (node_t* root, const char* fname);
void writepreorder (node_t* root, const char* fname);
void writepostorder (node_t* root, const char* fname);
void writenode(FILE* fp, node_t* node);

#endif