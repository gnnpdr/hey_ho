#pragma once

#include <stdio.h>

const size_t KEY_WORDS_AMT = 23;
const size_t MAX_WORD_LEN = 30;


#define KW(...) key_words[__VA_ARGS__].word

enum KeyWordType
{
    LINE_COMMENT,
    MULTI_LINE_COMMENT_START,
    MULTI_LINE_COMMENT_END,
    INT,
    VOID,
    EQUAL,
    RETURN,
    IF,
    ELSE_IF,
    ELSE,
    FOR,
    WHILE,
    PLUS,
    MINUS,
    MULT,
    DIV,
    END,
    OPEN_BRACE,
    CLOSE_BRACE,
    PRINT,
    SEP,
    UNKNOWN,
    OPEN_CURLY_BRACE,
    CLOSE_CURLY_BRACE
};

struct KeyWord
{
    KeyWordType type;
    size_t words_amt;
    const char* word;
};

inline KeyWord key_words[KEY_WORDS_AMT] = {{LINE_COMMENT, 1, "..."},    
                            {MULTI_LINE_COMMENT_START, 1, "(*"}, 
                            {MULTI_LINE_COMMENT_END, 1, "*)"},
                            {INT, 1, "yo"},
                            {VOID, 1, "bro"},
                            {EQUAL, 1, "is"},
                            {RETURN, 3, "later"},
                            {IF, 1, "check this"},
                            {ELSE_IF, 2, "or maybe"},
                            {ELSE, 1, "nah"}, 
                            {FOR, 1, "keep rolling"}, 
                            {WHILE, 2, "go thru"},
                            {PLUS, 2, "and"},
                            {MINUS, 1, "without"},
                            {MULT, 1, "times"},
                            {DIV, 2, "split"},
                            {END, 1, "donezo"},
                            {OPEN_BRACE, 1, "("},
                            {CLOSE_BRACE, 1, ")"},
                            {PRINT, 1, "say"},
                            {SEP, 1, ","},
                            {OPEN_CURLY_BRACE, 1,  "{"},
                            {CLOSE_CURLY_BRACE, 1, "}"}};

