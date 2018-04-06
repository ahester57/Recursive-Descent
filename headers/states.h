#ifndef STATE_H_
#define STATE_H_

enum STATE {
    ERROR,
    INITIAL,
    ONE,
    TWO,
    OPERATOR,
    DELIM,
    // Begin FINAL states
    START,
    STOP,
    ITER,
    VOID,
    VAR,
    RETURN,
    READ,
    PRINT,
    PROGRAM,
    IFF,
    THEN,
    LET,
    IDENTIFIER,
    INTEGER,
    EQUALS,
    GREATER,
    LESS,
    COLON,
    PLUS,
    MINUS,
    STAR,
    FWDSLASH,
    PERCENT,
    PERIOD,
    LEFTPAREN,
    RIGHTPAREN,
    COMMA,
    LEFTCURLY,
    RIGHTCURLY,
    SEMICOLON,
    LEFTBRACKET,
    RIGHTBRACKET,
    AMPERSAND,
    QUOTE,
    EOFILE
};

#endif