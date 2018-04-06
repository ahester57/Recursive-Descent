#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filter.h"
#include "trimline.h"
#include "wordlist.h"

// create a 2-d array representation of the source file
// without spaces/ comments and include line numbers
// this does not split source into tokens yet
wordlist_t*
filtersource(FILE* fp)
{
    // declare our wordlist_t* for use throughout the program
    wordlist_t* list = (wordlist_t*) malloc(sizeof(wordlist_t));
    if (list == (wordlist_t*)NULL)
        return list;

    char* buf;
    size_t len = 0;
    ssize_t read;

    // declare our char** to set to list->list
    char** lines = (char**) malloc(256*sizeof(char*));
    if (lines == (char**)NULL)
        return (wordlist_t*)NULL;

    // Go thru the file line-by-line
    int i = 0;
    read = getline(&buf, &len, fp);
    while (read != -1)
    {
        if (read > 0) {
            lines[i] = (char*) malloc(256*sizeof(char));
            // trim all characters not in the alphabet
            trimline(lines[i], buf);
            if (strlen(lines[i]) > 0)
                i++;
            else
                free(lines[i]);
        }
        free(buf);
        buf = (char*) malloc(256*sizeof(char));
        read = getline(&buf, &len, fp);
        // at the eof errno is set by getline and read is set to -1
    }
    lines[i] = (char*)NULL;
    list->list = lines; 
    list->length = i+1;
    return list;
}

// Display the filtered source
void
displayfilter(wordlist_t* filter)
{
    int i;
    fprintf(stderr, "\nSource after filter and trim:\n");
    fprintf(stderr, "NUM_LINES = %d\n", filter->length-1);
    fprintf(stderr, "========================\n");
    for (i = 0; i < filter->length; i++) {
        if (filter->list[i] == NULL) {
            fprintf(stderr, "%s\n", "EOF");
            break;
        }
        fprintf(stderr, "%s", filter->list[i]);
    }
    fprintf(stderr, "========================\n\n");
}