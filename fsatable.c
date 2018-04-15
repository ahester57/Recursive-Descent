
/* Austin Hester
CS 4280 sp18
C.Z. Janikow */
#include <stdlib.h>
#include <string.h>
#include "fsatable.h"
#include "alphabet.h"
#include "states.h"

enum STATE
fsatable(const enum STATE state, const char nextchar)
{
    enum STATE nextstate = ERROR;
    // sorry for this 
    // i should have a function that nextstate =s an int 
    // that i switch upon, but in reality that is the
    // same amount of calculations
    switch (state)
    {
        // case: error
        case ERROR:
            nextstate = ERROR;
            break;
        // case: initial state
        case INITIAL:
            if (isoperator(nextchar)) {
                nextstate = OPERATOR;
                break;
            }
            if (isdelim(nextchar)) {
                nextstate = DELIM;
                break;
            }
            if (isletter(nextchar)) {
                nextstate = ONE;
                break;
            }
            if (isnumber(nextchar)) {
                nextstate = TWO;
                break;
            }
            if (iswhitespace(nextchar)) {
                nextstate = INITIAL;
                break;
            }
            if (nextchar == '\n') {
                nextstate = INITIAL;
                break;
            }
            if (nextchar == 26)
                nextstate = EOFILE;
            break;
        // case: the beginnings of an identifier
        case ONE:
            if (isoperator(nextchar)) {
                nextstate = IDENTIFIER;
                break;
            }
            if (isdelim(nextchar)) {
                nextstate = IDENTIFIER;
                break;
            }
            if (isletter(nextchar)) {
                nextstate = ONE;
                break;
            }
            if (isnumber(nextchar)) {
                nextstate = ONE;
                break;
            }
            if (iswhitespace(nextchar)) {
                nextstate = IDENTIFIER;
                break;
            }
            if (nextchar == '\n') {
                nextstate = IDENTIFIER;
                break;
            }
            if (nextchar == 26)
                nextstate = IDENTIFIER;
            break;
        // case: the beginnings of an integer
        case TWO:
            if (isoperator(nextchar)) {
                nextstate = INTEGER;
                break;
            }
            if (isdelim(nextchar)) {
                nextstate = INTEGER;
                break;
            }
            if (isletter(nextchar)) {
                nextstate = INTEGER;
                break;
            }
            if (isnumber(nextchar)) {
                nextstate = TWO;
                break;
            }
            if (iswhitespace(nextchar)) {
                nextstate = INTEGER;
                break;
            }
            if (nextchar == '\n') {
                nextstate = INTEGER;
                break;
            }
            if (nextchar == 26)
                nextstate = INTEGER;
            break;
        case THREE:
            nextstate = operatortable(state, nextchar);
            break;
        case FOUR:
            nextstate = operatortable(state, nextchar);
            break;
        case FIVE:
            nextstate = operatortable(state, nextchar);
            break;
        default:
            nextstate = ERROR;
    }
    if (nextstate == OPERATOR) {
        nextstate = operatortable(state, nextchar);
    }
    if (nextstate == DELIM) {
        nextstate = delimtable(state, nextchar);
    }
    return nextstate;
}

// operator token stuff
enum STATE
operatortable(const int state, const char nextchar)
{

    enum STATE nextstate = ERROR;
    switch (state)
    {
        case INITIAL:
            if (nextchar == '=') {
                nextstate = THREE;
                break;
            }
            if (nextchar == '>') {
                nextstate = FOUR;
                break;
            }
            if (nextchar == '<') {
                nextstate = FIVE;
                break;
            }
            if (nextchar == ':') {
                nextstate = COLON;
                break;
            }
            if (nextchar == '+') {
                nextstate = PLUS;
                break;
            }
            if (nextchar == '-') {
                nextstate = MINUS;
                break;
            }
            if (nextchar == '*') {
                nextstate = STAR;
                break;
            }
            if (nextchar == '/') {
                nextstate = FWDSLASH;
                break;
            }
            if (nextchar == '%') {
                nextstate = PERCENT;
                break;
            }
            break;
        // hmmm
        case THREE:
            // i handle the following tokens 
            // using backtracking -> cut spaces
            // in the original trim line
            // no need for a look ahead
            // or extra helper functions
            switch (nextchar)
            {
                case '=':
                    nextstate = EQUALSEQUALS;
                    break;
                default:
                    nextstate = EQUALS;
            }
            break;
        case FOUR:
            if (nextchar == '>') {
                nextstate = GREATEREQUALS;
                break;
            } else {
                nextstate = GREATER;
                break;
            }
            break;
        case FIVE:
            if (nextchar == '<') {
                nextstate = LESSEQUALS;
                break;
            } else {
                nextstate = LESS;
            }
            break;
        default:
            nextstate = ERROR;
    }
    return nextstate;
}

// delimiter token stuff,
// divide and conquer even if it's not for efficiency
enum STATE
delimtable(const int state, const char nextchar)
{

    enum STATE nextstate = ERROR;
    switch (state) 
    {
        // init. delims end any other token
        case INITIAL:
            if (nextchar == '.') {
                nextstate = PERIOD;
                break;
            }
            if (nextchar == '(') {
                nextstate = LEFTPAREN;
                break;
            }
            if (nextchar == ')') {
                nextstate = RIGHTPAREN;
                break;
            }
            if (nextchar == ',') {
                nextstate = COMMA;
                break;
            }
            if (nextchar == '{') {
                nextstate = LEFTCURLY;
                break;
            }
            if (nextchar == '}') {
                nextstate = RIGHTCURLY;
                break;
            }
            if (nextchar == ';') {
                nextstate = SEMICOLON;
                break;
            }
            if (nextchar == '[') {
                nextstate = LEFTBRACKET;
                break;
            }
            if (nextchar == ']') {
                nextstate = RIGHTBRACKET;
                break;
            }
            if (nextchar == '&') {
                nextstate = AMPERSAND;
                break;
            }
            if (nextchar == '"') {
                nextstate = QUOTE;
                break;
            }
            break;
        default:
            nextstate = ERROR;
    }
    return nextstate;
}