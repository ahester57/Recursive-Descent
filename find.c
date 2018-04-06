#include <stdlib.h>
#include <string.h>
#include "find.h"

int
findchar(char c, const char* charset)
{
    int i = 0;
    while (charset[i] != '\0')
    {
        if (c == charset[i])
            return i;
        i++; 
    }
    return 0;
}

int
findword(char* word, const char** wordset)
{
    int i = 0;
    while (wordset[i] != NULL)
    {
        if (strcmp(word, wordset[i]) == 0)
            return i;
        i++; 
    }
    return -1;
}