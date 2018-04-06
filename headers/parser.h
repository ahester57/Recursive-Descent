#ifndef PARSER_H_
#define PARSER_H_
#include "token.h"
#include "wordlist.h"

int parser(token_t** tokenlist, wordlist_t* filter);
void program();
void block();
void vars();
void mvars();
void expr();
void xhelp();
void M();
void R();
void stats();
void mstat();
void stat();
void in();
void out();
void iff();
void loop();
void assign();
void RO();
void lesshelp();
void greathelp();
void equalhelp();
#endif