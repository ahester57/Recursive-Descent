#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "readfile.h"

int static numwords = 0;



// Get a list of strings in the file,
// separated by whitespace
char**
getfilestring (FILE* fp)
{
    // words limited to 64 chars
    char word[64];
    char** list = (char**) malloc(64 * sizeof(char*));
    //char** list;
    
    int c = 0;
    while (fscanf(fp, "%s", word) != EOF)
    {
        list[c] = (char*) malloc(sizeof(word));
        //fprintf(stdout, "%s\n", word);
        strcpy(list[c], word);
        c++;
    }

    numwords = c;

    return list;
}

// Should be called after getfilestring,
// or else you'll just get 0
int
getnumwords ()
{
    return numwords;
}

// Get the size of the file, in bytes
size_t
getfilesize (FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    return size;
}