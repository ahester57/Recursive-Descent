#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treehelper.h"
#include "tree.h"
#include "token.h"

node_t*
initializenode (node_t* root, int level)
{
    root = (node_t*) malloc(sizeof(node_t));
    root->token = (token_t*) malloc(sizeof(token_t));
    root->depth = level;
    root->left = NULL;
    root->right = NULL;
    return root;
}

void
addtokentonode (node_t* root, token_t* token)
{
    //memcpy(root->token, token, sizeof(token_t));
    root->token = token;
}

void
traverseinorder (node_t* root)
{
    if (root == NULL)
        return;
    traverseinorder(root->left);
    printnode(root);
    traverseinorder(root->right);
}

void
traversepreorder (node_t* root)
{
    if (root == NULL) {
        perror("root is null");
        return;
    }
    printf("root not null\n");
    printnode(root);
    traversepreorder(root->left);
    traversepreorder(root->right);
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
    displaytoken(node->token);
    printf("\n");
}