#ifndef FILTER_H_
#define FILTER_H_
#include "wordlist.h"
#include <stdio.h>

wordlist_t* filtersource(FILE* fp);
void displayfilter(wordlist_t* filter);

#endif