#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "metafile.h"
#include "parser.h"
#include "filter.h"
#include "wordlist.h"
#include "token.h"
#include "tree.h"
#include "treehelper.h"
#include "node.h"

int
main(int argc, char** argv)
{
    // deal with command line arguments or lack thereof
    int keyboardin = 1;
    char* fname = "out";
    FILE* fp = stdin;
    // Parse arguments
    if (argc > 1) {
        keyboardin = 0;
        fname = getbasefilename(argv[1]);
        if (fname == (char*)NULL) {
            perror("Input error");
            return 1;
        }
        fp = openinputfile(fname);
    }
    if (fp == (FILE*)NULL) {
        perror("Input error");
        return 1;
    }

    // filter the file to get a wordlist_t
    wordlist_t* filter = filtersource(fp);
    fclose(fp);
    if (filter == (wordlist_t*)NULL) {
        perror("Filter error");
        return 1;
    }

    // Display filtered source
    displayfilter(filter);

    // Initialize the token list
    token_t** tokenlist = (token_t**) malloc(2048*sizeof(token_t*));
    if (tokenlist == (token_t**)NULL) {
        perror("Memory error");
        return 1;
    }

    // Plant the roots
    node_t* root = NULL;

    // calls testscanner() function until EOF 
    int numtokens = parser(root, tokenlist, filter);

    printnode(root);
    traversepreorder(root);

    // free fname if it was generated.
    if (!keyboardin) {
        free(fname);
    }
    return 0;
}