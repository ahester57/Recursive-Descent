
/* Austin Hester
CS 4280 sp18
C.Z. Janikow */
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
#include "treetrim.h"
#include "node.h"
#include "staticsem.h"

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
    // filter the file line by line to get a wordlist_t
    wordlist_t* filter = filtersource(fp);
    fclose(fp);
    if (filter == (wordlist_t*)NULL) {
        free(filter);
        perror("Filter error");
        return 1;
    }
    // Display filtered wordlist
    displayfilter(filter);
    // Initialize the token list
    token_t** tokenlist = (token_t**) malloc(256*sizeof(token_t*));
    if (tokenlist == (token_t**)NULL ) {
        free(tokenlist);
        perror("Memory error");
        return 1;
    }
    int n = 0;  // num_tokens
    node_t* root = NULL;
    // calls parser() function until EOF 
    root = parser(root, tokenlist, filter, &n);
    //Print the complete parse tree
    printf("\nFull Tree:");
    traversepreorder(root);
    // Trim the tree and print
    node_t* newroot = NULL;
    newroot = treetrim(newroot, root);
    printf("\nTrimmed Tree:");
    traversepreorder(newroot);

    // Static semantic analysis
    int pass_static = analyzevars(newroot);

    if (pass_static == 0)
        printf("\nPass static semantic check. OK\n");
    else if (pass_static > 0) {
        printf("********************************\n");
        printf("Fail static semantic check. FAIL\n");
        switch (pass_static)
        {
            case 1: 
                printf("Variable already defined\n");
                break;
            case 2: 
                printf("Variable not yet defined\n");
                break;
            default:
                printf("Check yourself. No idea.\n");
        }
        printf("********************************\n");
    } else
        fprintf(stderr, "Error while checking static semantics.");

    // Free them
    free(filter);
    free(tokenlist);
    free(root);
    free(newroot);
    // free fname if it was generated.
    if (!keyboardin)
        free(fname);
    return 0;
}