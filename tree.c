#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "treehelper.h"
#include "readfile.h"

node_t*
buildtree (FILE* fp)
{
    char** wordlist = getfilestring(fp);
    int n = getnumwords();
    node_t* root = NULL;
    root = insert(root, wordlist[0], 0);
    int i;
    for (i = 1; i < n; i++)
        insert(root, wordlist[i], 0);
    return root;
}

node_t*
insert (node_t* root, char* word, int level)
{

    if (word == (char*) NULL)
        return (node_t*) NULL;

    int len = strlen(word);

    if (root == NULL) {
        // initialize a new node
        root = initializenode(root, len, level);
        addwordtonode(root, word);
    } else if (len == root->length) {
        // add this word to this node
        if (!isinnode(root, word)) {
            addwordtonode(root, word);
        }
    } else if (len < root->length) {
        root->left = insert(root->left, word, ++level);
    } else {
        root->right = insert(root->right, word, ++level);
    }
    return root;
}

node_t*
search (node_t* root, char* word)
{
    int len = strlen(word);
    if (root == NULL || root->length == len) {
        return root;
    }
    if (root->length < len) {
        return search(root->right, word);
    }
    return search(root->left, word);
}