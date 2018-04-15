#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsadriver.h"
#include "fsatable.h"
#include "wordlist.h"
#include "token.h"
#include "states.h"
#include "keywords.h"

// for tracking place in the filter
static int line = 0;
static int column = 0;

// I apologize for how ridiculous this function is.
// I'll split it up in the future
token_t*
fsadriver(const wordlist_t* filter)
{
    int i = 0;
    enum STATE state = INITIAL;
    enum STATE nextstate = ERROR;
    token_t* token = (token_t*) malloc(sizeof(token_t));
    char nextchar;
    char string[256] = "";

    int numlines = filter->length;
    char buf[256];

    for (line = line; line < numlines; line++) {
        // first for loop for each line
        //fprintf(stderr, "line = %s\n", filter->list[line]);
        if (filter->list[line] == (char*)NULL) {
            // If next line is NULL,
            // \t is a special char here, signifies 
            // EOF to FSA
            nextchar = 26;            
        } else {
            // copy new line to buffer
            strcpy(buf, filter->list[line]);
            nextchar = buf[column];
        }

        // loop until FINAL state, breaks on end of line
        // which allows for multi-line tokens
        while (state < IDENTIFIER)
        {
            nextstate = fsatable(state, nextchar);
            fprintf(stderr, "state\t = %d\n", state);
            fprintf(stderr, "string\t = .%s.\n", string);
            fprintf(stderr, "next\t = %c,\n", nextchar);

            // If there is an error
            if (nextstate == ERROR) {
                fprintf(stderr, "Error while parsing @ line ");
                fprintf(stderr, "%d\n", line);
                fprintf(stderr, "Check last state and next for info.\n");
                return (token_t*)NULL;
            }
            // If we have reached a final state
            if (nextstate >= START) {
                state = nextstate;
                // If we have a single character token,
                // add the current char to string
                if (i == 0) {
                    if (nextchar != '\n' && nextchar != ' ') {
                        string[i] = nextchar;
                    }
                }
                if (nextstate == EQUALSEQUALS ||
                    nextstate == GREATEREQUALS ||
                    nextstate == LESSEQUALS) {
                    string[i] = nextchar;
                    column++;
                }



                // the following corrects error of line number
                // when the lookahead requires going to the
                // next line
                int lastline = line;
                if (column == 0 && i != 0)
                    lastline--;

                // Check if keyword
                if (iskeyword(string)) {
                    state = whichkeyword(string);
                }

                maketoken(token, state, string, lastline);

                // this is needed for avoiding infinite loops in table
                if (i < 1)
                    column++;

                // is the next char the end?
                // if so let the position vars know
                nextchar = buf[column];
                if (nextchar == '\0' || nextchar == '\n') {
                    column = 0;
                    line++;
                } 
                // Return that token
                return token;
            // If we're still not done
            } else {
                state = nextstate;
                if (nextchar != '\n' && nextchar != ' ') {
                    string[i] = nextchar;
                    i++;
                }
                column++;
                nextchar = buf[column];
                if (nextchar == '\0') {
                    column = 0;
                    break;
                } 
            }
            // inside while
        }
        // outside of while
        // token is still processing.
        // new lines do not end scanning
    }
    // we did not encounter an error, but no token was made
    // must be END OF FILE
    maketoken(token, EOFILE, "EOF", line);
    return token;
}

void
resetfsadriver() {
    line = 0;
    column = 0;
}
