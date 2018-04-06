#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "metafile.h"

char*
getbasefilename (const char* arg)
{
    int len = strlen(arg);
    char* fname = (char*) malloc((len+1)*sizeof(char));

    if (fname == NULL)
        return (char*) NULL;
    
    // Copy argument to fname
    strcpy(fname, arg);

    if (len > 4) {
        // Check if extension '.sp18' already provided
        // advocate for TAB completion 
        const char* ext = &fname[len-5];
        if (!strcmp(ext, ".sp18")) {
            // If .sp18 appended, take off.
            fname[len-5] = '\0';
        }
    }
    return fname;
}

FILE*
openinputfile (const char* fname)
{
    char temp[64];
    
    // append .sp18 to base file name
    sprintf(temp, "%s%s", fname, ".sp18");
    if (temp == NULL)
        return (FILE*) -1;
    return fopen(temp, "r");
}