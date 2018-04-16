#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "token.h"
#include "treehelper.h"

node_t*
buildtree (token_t** tk, int n)
{
    node_t* root = NULL;
    root = insert(root, tk[0], 0);
    int i;
    for (i = 1; i < n; i++) {
        insert(root, tk[i], 0);
    }
    return root;
}

node_t*
insert (node_t* root, token_t* tk, int level)
{

    if (tk == (token_t*) NULL)
        return (node_t*) NULL;

    int len = strlen(tk->instance);

    if (root == NULL) {
        // initialize a new node
        root = initializenode(root, len, level);
        addwordtonode(root, tk);
    } else {
        // give birth
        int n = root->num_children;
        root->num_children++;
        root->children[n] = insert(root->children[n], tk, level);
    }

    return root;
}
