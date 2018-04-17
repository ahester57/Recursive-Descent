#include <stdlib.h>
#include <string.h>
#include "first.h"

// return the first of <stats> and <stat>
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

int
first_mvars(char* tkid)
{
    if (strcmp(tkid, ".TK") == 0 || strcmp(tkid, ":TK") == 0) {
        return 1;
    }
    return 0;
}

// return the first of <expr>
int
first_expr(char* tkid)
{
    if (first_R(tkid) || first_M(tkid)) {
        return 1;
    }
    return 0;
}

int
first_xhelp(char* tkid)
{
    if (strcmp(tkid, "+TK") == 0 || strcmp(tkid, "-TK") == 0 ||
        strcmp(tkid, "/TK") == 0 || strcmp(tkid, "*TK") == 0) {
        return 1;
    }
    return 0;
}

int
first_M(char* tkid)
{
    if (strcmp(tkid, "%TK") == 0) {
        return 1;
    }
    return 0;
}

int
first_R(char* tkid)
{
    if (strcmp(tkid, "(TK") == 0 || strcmp(tkid, "idTK") == 0 ||
        strcmp(tkid, "intTK") == 0) {
        return 1;
    }
    return 0;
}