#ifndef NODE_H_
#define NODE_H_

typedef struct node node_t;

struct node
{
    char** words;
    int length;
    int count;
    int depth;
    node_t* left;
    node_t* right;
};

#endif