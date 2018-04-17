#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "treetrim.h"
#include "token.h"
#include "keywords.h"

node_t*
treetrim(node_t* newroot, node_t* root)
{
    newroot = NULL;
    newroot = insert(newroot, root->token, root->depth);
    int i;
    for (i = 0; i < root->num_children; i++) {
        climbandtrim(newroot, root->children[i]);
    }
    
    return newroot;
}

node_t*
climbandtrim(node_t* newroot, node_t* root)
{
    if (root == NULL)
        return (node_t*)NULL;
    
    //printf("fuck\n");
    // if it is essential
    if (isessential(root)) { 
            insert(newroot, root->token, root->depth);
    }

    int i;
    for (i = 0; i < root->num_children; i++) {
        if (root->num_children != 0)
            climbandtrim(newroot->children[newroot->num_children-1], root->children[i]);
    }
    return newroot;
}

int
isessential(node_t* node)
{
    if (node->essential) 
        return 1;
    
    if (strcmp(node->token->id, ".TK") == 0) 
        return 0;

    if (strcmp(node->token->id, ":TK") == 0) 
        return 0;

    if (strcmp(node->token->id, "(TK") == 0) 
        return 0;
    
    if (strcmp(node->token->id, ")TK") == 0) 
        return 0;
    if (iskeyword(node->token->instance)) 
        return 0;
    
    if (strcmp(node->token->id, "EOFTK") == 0) 
        return 0;
    
    if (strcmp(node->token->id, "<>") == 0) {
        int i;
        for (i = 0; i < node->num_children; i++) {
            if (isessential(node->children[i])) {
                node->essential = 1;
                return 1;
            }
        }
        return 0;
    }
    node->essential = 1;
    return 1;
}