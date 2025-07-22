#pragma once

//-----libs-------------------

#include <assert.h>

#include "text_preparing.h"

//------consts--------------

const size_t FORMING_NEW_SCOPE_AMT  = 1;
const size_t DELETE_SCOPE_AMT       = 1;
const size_t FORMING_SET_AMT        = 2;
const size_t MAX_ID_TABLE_SIZE      = 10;
const size_t WORD_REST_AMT          = 16;

//------enums---------------

enum TokenType
{
    NUM,
    VAR,
    VAR_SET,
    FUNC,
    FUNC_SET,
    KEY_WORD
};

enum IDType
{
    FUNC_ID,
    VAR_ID,
};

//-------structs------------

struct Token
{
    int value;
    char* word;

    TokenType type;
    size_t line;

    size_t scope;
};

struct ID
{
    char* word;
    IDType type;
};

struct IDTable
{
    ID* table;
    size_t len;
};

struct Scope
{
    size_t scope_ID;
    IDTable ID_table;
};

struct Lexer
{
    Token* tokens;
    size_t word_cnt;
    size_t words_amt;

    Scope* scopes;
    size_t cur_scope;
};

//--------arrays------------

inline KeyWordType forming_new_scope[] = {OPEN_CURLY_BRACE};

inline KeyWordType delete_scope[] = {CLOSE_CURLY_BRACE};

inline KeyWordType forming_set[] = { INT, 
                                     VOID };

inline KeyWordType key_word_rest[]  = { EQUAL,
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
                                        SEP };

//-------funcs-------------

Lexer* lexer_ctor(Word* words, FILE* log_file);

void lexer_dtor(Lexer* lexer);

int lex_analysis(Lexer* lexer, FILE* log_file);