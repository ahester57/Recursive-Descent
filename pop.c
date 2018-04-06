#include <stdlib.h>
#include "pop.h"

void*
pop(void** list) {
    int i;
    void* temp = list[0];
    for (i = 0; i < 256; i++) {
        list[i] = list[i+1];
    }
    list[i] = NULL;
    return temp;
}