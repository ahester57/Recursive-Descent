#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "first.h"

int
first_stat(char* tkid)
{
    if (strcmp(tkid, "readTK") == 0 || strcmp(tkid, "printTK") == 0 ||
        strcmp(tkid, "iffTK") == 0 || strcmp(tkid, "iterTK") == 0 ||
        strcmp(tkid, "letTK") == 0 || strcmp(tkid, "startTK") == 0) {
        return 1;
    }
    return 0;
}