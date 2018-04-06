#ifndef FIND_H_
#define FIND_H_

// return 1 if c is in charset,
// return 0 otherwise
int findchar(char c, const char* charset);
int findword(char* word, const char** wordset);

#endif