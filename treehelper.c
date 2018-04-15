#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treehelper.h"
#include "tree.h"
#include "token.h"

node_t*
initializenode (node_t* root, int len, int level)
{
    root = (node_t*) malloc(sizeof(node_t));
    root->token = (token_t*) malloc(sizeof(token_t));
    root->depth = level;
    root->children = (node_t**) malloc(sizeof(8*sizeof(node_t*)));
    int i;
    for (i = 0; i < 8; i++) {
        root->children[i] = (node_t*) malloc(sizeof(node_t));
        root->children[i] = NULL;
    }
    root->num_children = 0;
    return root;
}

void
addwordtonode (node_t* root, token_t* token)
{
    root->token = (token_t*) malloc(sizeof(token_t));
    copytoken(root->token, token);
}

void
traverseinorder (node_t* root)
{
    if (root == NULL)
        return;
    int i;
    for (i = 0; i < root->num_children; i++) {
        traverseinorder(root->children[i]);
        printnode(root);
    }
}

void
traversepreorder (node_t* root)
{
    if (root == NULL)
        return;
    printnode(root);

    int i;
    for (i = 0; i < root->num_children; i++) {
        traversepreorder(root->children[i]);
    }
}

void
traversepostorder (node_t* root)
{
    if (root == NULL)
        return;
    traversepostorder(root->left);
    traversepostorder(root->right);
    printnode(root);
}

void
printnode (node_t* node)
{
    if (node->depth > 0)
        printf("%*c", node->depth*2, ' ');
    printf("%s: ", node->token->id);
    printf("%s ", node->token->instance);
    printf(" - %d", node->num_children);
    printf("\n");
}