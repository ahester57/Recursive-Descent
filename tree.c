#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "treehelper.h"
#include "token.h"

node_t*
insert (node_t* root, token_t* token, int left, int level)
{
    if (token == (token_t*) NULL)
        return (node_t*) NULL;

    if (root == NULL) {
        // initialize a new node
        root = initializenode(root, level);
        addtokentonode(root, token);
        displaytoken(root->token);
        printf("inserted\n");
    } else if (left) {
        root->left = insert(root->left, token, left, ++level);
    } else {
        root->right = insert(root->right, token, left, ++level);
    }
    return root;
}
