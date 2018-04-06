#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trimline.h"
#include "alphabet.h"

// for tracking multi-
// line comments
static int commentflag = 0;

// trim spaces and comments out
// also symbols not in the alphabet 
void
trimline(char* dest, const char* src)
{
    int i = 0;
    int j = 0;
    char buf[256];
    char c = src[i];

    while (c != '\0')
    {
        if (c == '&') {
            // toggle comment
            commentflag = (commentflag + 1) % 2;
            if (commentflag == 0) {
                // marks end of comment
                c = src[++i];
                continue;
            }
        }
        if (commentflag || !isinalphabet(c)) {
            // forget comments and symbols not in the alphabet
            c = src[++i];
            continue;
        }

        buf[j++] = c;
        c = src[++i];
    }

    buf[j] = '\0';
    strcpy(dest, buf);
    return;
}