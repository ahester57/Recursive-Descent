#ifndef TREETRIM_H_
#define TREETRIM_H_
#include "node.h"

node_t* treetrim(node_t* newroot, node_t* root);
node_t* climbandtrim(node_t* newroot, node_t* root);
int isessential(node_t* node);

#endif