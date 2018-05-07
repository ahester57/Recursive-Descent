
/* Austin Hester
CS 4280 sp18
C.Z. Janikow */
#include <stdlib.h>
#include "pop.h"

void*
pop(void** list) {
    int i;
    void* temp = list[0];
    for (i = 0; i < 2048; i++) {
        if (list[i+1] == (void*)NULL)
            break;
        list[i] = list[i+1];
    }
    list[i] = (void*)NULL;
    return temp;
}
