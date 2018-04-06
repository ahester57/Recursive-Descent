#ifndef PARSER_H_
#define PARSER_H_
#include "token.h"
#include "wordlist.h"

int parser(token_t** tokenlist, wordlist_t* filter);
void program(token_t**);
void block(token_t**);
void vars(token_t**);
void mvars(token_t**);
void expr(token_t**);
void xhelp(token_t**);
void M(token_t**);
void R(token_t**);
void stats(token_t**);
void mstat(token_t**);
void stat(token_t**);
void in(token_t**);
void out(token_t**);
void iff(token_t**);
void loop(token_t**);
void assign(token_t**);
void RO(token_t**);
void lesshelp(token_t**);
void greathelp(token_t**);
void equalhelp(token_t**);
void printerror();

#endif