#pragma once

#include <assert.h>
#include "text_preparing.h"

enum TokenType
{
    NUM,
    VAR,
    VAR_SET,
    FUNC,
    FUNC_SET,
    KEY_WORD
};

struct Token
{
    int value;
    char* word;

    TokenType type;
    size_t line;

    size_t scope;
};

enum IDType
{
    FUNC_ID,
    VAR_ID,
};

struct ID
{
    char* word;
    IDType type;
};

struct IDTable          //так удобнее обращаться к ид, искать их, удобнее, чем если бы это был просто массив 
{
    ID* table;
    size_t len;         //сколько заполнено
};

struct Scope
{
    size_t scope_ID;
    IDTable ID_table;
};

const size_t MAX_ID_TABLE_SIZE = 10;

struct Lexer
{
    Token* tokens;
    size_t word_cnt;
    size_t words_amt;

    Scope* scopes;
    size_t cur_scope;
};

Lexer lexer_ctor(Word* words);

const size_t forming_new_scope_amt = 1;
inline KeyWordType forming_new_scope[forming_new_scope_amt] = {OPEN_CURLY_BRACE};

const size_t delete_scope_amt = 1;
inline KeyWordType delete_scope[] = {CLOSE_CURLY_BRACE};


const size_t forming_set_amt = 2;
inline KeyWordType forming_set[] = {INT, VOID};

const size_t word_rest_amt = 16;
inline KeyWordType key_word_rest[word_rest_amt] = {
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
    SEP
};


IDTable id_table_ctor();
Scope* scopes_ctor();
Token* tokens_ctor(Word* words, size_t words_amt);
void lexer_dtor(Lexer* lexer);
void tokens_dtor(Token* tokens, size_t words_amt);
void id_table_dtor(IDTable* table);
void scopes_dtor(Scope* scopes, size_t scopes_amt);

void lex_analysis(Lexer* lexer);