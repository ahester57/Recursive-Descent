#ifndef PARSER_H_
#define PARSER_H_
#include "token.h"
#include "node.h"
#include "wordlist.h"

node_t* parser(node_t* root, token_t** tk, token_t** tc, wordlist_t* fltr, int* n);
void program(node_t* root, token_t**);
void block(node_t* root, token_t**);
void vars(node_t* root, token_t**);
void mvars(node_t* root, token_t**);
void expr(node_t* root, token_t**);
void xhelp(node_t* root, token_t**);
void M(node_t* root, token_t**);
void R(node_t* root, token_t**);
void stats(node_t* root, token_t**);
void mstat(node_t* root, token_t**);
void stat(node_t* root, token_t**);
void in(node_t* root, token_t**);
void out(node_t* root, token_t**);
void assign(node_t* root, token_t**);
void iffandloop(node_t* root, token_t**);
void evaluate(node_t* root, token_t** tklist);
void RO(node_t* root, token_t**);
void lesshelp(node_t* root, token_t**);
void greathelp(node_t* root, token_t**);
void equalhelp(node_t* root, token_t**);
void printerror(const char* FUNC);

#endif