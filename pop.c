#include <stdlib.h>
#include "pop.h"

void*
pop(void** list) {
    int i = 0;;
    void* temp = list[i];
    for (i = 0; i < 2048; i++) {
        if (list[i+1] == (void*)NULL)
            break;
        list[i] = list[i+1];
    }
    list[i] = (void*)NULL;
    return temp;
}