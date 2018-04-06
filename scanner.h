#ifndef SCANNER_H_
#define SCANNER_H_
#include "wordlist.h"
#include "token.h"

token_t* scanner(const wordlist_t* filter);
token_t settoken(const char* instance, const int linenum);

#endif