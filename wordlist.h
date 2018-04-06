#ifndef WORDLIST_H_
#define WORDLIST_H_

typedef struct {
    char** list;
    char* currentword;
    int length;
} wordlist_t;

#endif