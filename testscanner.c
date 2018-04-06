#include <stdlib.h>
#include <stdio.h>
#include "testscanner.h"
#include "scanner.h"
#include "wordlist.h"
#include "token.h"

// Add tokens to list until EOF
// returns number of tokens
int
testscanner(token_t** tokenlist, wordlist_t* filter)
{
    printf("Watch the FSA work ... \n");
    printf("========================\n");
    int i = 0;
    while (1)
    {
        token_t* tk = scanner(filter);
        if (tk == (token_t*)NULL)
            return 1;
        printf("\t\t%s, %s, %d\n", tk->id, tk->instance, tk->line_num);
        tokenlist[i] = tk;
        i++;
        if (isEOFtoken(tk))
            break;
    }
    printf("========================\n");
    return i;
}