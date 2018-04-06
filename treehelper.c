#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treehelper.h"
#include "tree.h"
#include "writetree.h"

node_t*
initializenode (node_t* root, int len, int level)
{
    root = (node_t*) malloc(sizeof(node_t));
    root->words = (char**) malloc(64*sizeof(char*));
    root->length = len;
    root->depth = level;
    root->left = NULL;
    root->right = NULL;
    return root;
}

void
addwordtonode (node_t* root, char* word)
{
    root->count++;
    root->words[root->count-1] = (char*) malloc(64*sizeof(char));
    strcpy(root->words[root->count-1], word);
}

// 1 - yes
// 0 - no
int
isinnode (node_t* node, char* word)
{
    int i;
    for (i = 0; i < node->count; i++) {
        if (!strcmp(node->words[i], word)) 
            return 1;
    } 
    return 0;
}

void
traverseinorder (node_t* root, FILE* fp)
{
    if (root == NULL)
        return;
    traverseinorder(root->left, fp);
    printnode(root);
    writenode(fp, root);
    traverseinorder(root->right, fp);
}

void
traversepreorder (node_t* root, FILE* fp)
{
    if (root == NULL)
        return;
    printnode(root);
    writenode(fp, root);
    traversepreorder(root->left, fp);
    traversepreorder(root->right, fp);
}

void
traversepostorder (node_t* root, FILE* fp)
{
    if (root == NULL)
        return;
    traversepostorder(root->left, fp);
    traversepostorder(root->right, fp);
    printnode(root);
    writenode(fp, root);
}

void
printnode (node_t* node)
{
    if (node->depth > 0)
        printf("%*c", node->depth*2, ' ');
    printf("%d ", node->length);
    int i;
    for (i = node->count-1; i >= 0 ; i--) {
        printf("%s ", node->words[i]);
    }
    printf("\n");
}