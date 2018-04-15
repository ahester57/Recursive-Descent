#ifndef TOKEN_H_
#define TOKEN_H_
#include "states.h"

typedef struct {
    char* id;
    char* instance;
    int line_num;
} token_t;

char* gettoken(enum STATE state);
int isEOFtoken(const token_t* token);
void maketoken(token_t* token,
              const enum STATE state,
              char* string,
              const int line);
token_t* customtoken(char* id, char* string, int line);
void copytokenlist(token_t** dest, token_t** src, int n);
void copytoken(token_t* dest, token_t* src);
void displaytokens(token_t** tokenlist, const int numtokens);
void displaytoken(const token_t* token);

#endif