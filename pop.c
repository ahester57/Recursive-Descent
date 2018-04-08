#include <stdlib.h>
#include "pop.h"
#include "token.h"
#include "string.h"

void*
pop(void** list) {
    int i;
    void* temp = (void*) malloc(sizeof(token_t));
    memcpy(temp, list[0], sizeof(token_t));

    for (i = 0; i < 2048; i++) {
        if (list[i+1] == (void*)NULL)
            break;
        list[i] = list[i+1];
    }
    list[i] = (void*)NULL;
    return temp;
}