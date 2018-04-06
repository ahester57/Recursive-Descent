#include <stdlib.h>
#include <stdio.h>
#include "writetree.h"
#include "tree.h"
#include "treehelper.h"

void
writeinorder (node_t* root, const char* fname)
{
    if (root != NULL && root->count > 0) {
        FILE* fp;
        char temp[64];
        sprintf(temp, "%s.inorder", fname);
        fp = fopen(temp, "w");
        traverseinorder(root, fp);
        fclose(fp);
    }
}

void
writepreorder (node_t* root, const char* fname)
{
    if (root != NULL && root->count > 0) {
        FILE* fp;
        char temp[64];
        sprintf(temp, "%s.preorder", fname);
        fp = fopen(temp, "w");
        traversepreorder(root, fp);
        fclose(fp);
    }
}

void
writepostorder (node_t* root, const char* fname)
{
    if (root != NULL && root->count > 0) {
        FILE* fp;
        char temp[64];
        sprintf(temp, "%s.postorder", fname);
        fp = fopen(temp, "w");
        traversepostorder(root, fp);
        fclose(fp);
    }
}

void
writenode (FILE* fp, node_t* node)
{
    if (node->depth > 0)
        fprintf(fp, "%*c", node->depth*2, ' ');
    fprintf(fp, "%d ", node->length);
    int i;
    for (i = node->count-1; i >= 0 ; i--) {
        fprintf(fp, "%s ", node->words[i]);
    }
    fprintf(fp, "\n");
}
