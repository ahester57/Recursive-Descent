#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "alphabet.h"
#include "find.h"

static const char ops[] = "=><:+-*/%";
static const char delims[] = ".(),{};[]&\"";

// fairly self-explainable
int
isinalphabet(const char c)
{
    if (isalnum(c) || findchar(c, ops) ||
        findchar(c, delims) || c == ' ' || c == '\n') { 
        return 1;
    }
    return 0;
}

int
isletter(const char c)
{
    if (isalpha(c))
        return 1;
    return 0;
}

int
isnumber(const char c)
{
    if (isdigit(c))
        return 1;
    return 0;
}

int
isoperator(const char c)
{
    if (findchar(c, ops))
        return 1;
    return 0;
}

int
isdelim(const char c)
{
    if (findchar(c, delims))
        return 1;
    return 0;
}

int
iswhitespace(const char c)
{
    if (c == ' ' || c == '\0')
        return 1;
    return 0;
}

int
isendofline(const char c)
{
    if (c == '\0')
        return 1;
    return 0;
}