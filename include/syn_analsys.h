#pragma once
#include "lex_analysis.h"

enum NodeType
{
    ID_NODE,
    KW_NODE,
    NUM_NODE,
    TYPE_NODE,
    FUNCSET_NODE,
    PARAM_NODE,         //слева вывод, справа ноды - входные параметры
    VARSET_NODE,

};

struct Node
{
    NodeType type;
    int val;
    Node* left;
    Node* right;
};

const size_t FIRST_PR_AMT = 2;
const size_t SECOND_PR_AMT = 2;
inline KeyWordType sec_pr[] = {PLUS, MINUS};
inline KeyWordType first_pr[] = {MULT, DIV};

const size_t FORMING_LAYER_AMT = 7;
inline KeyWordType layer_pr[] = {FOR, WHILE, IF, ELSE_IF, ELSE, VOID, INT};
const size_t FORMING_SERV_AMT = 3;
inline TokenType serv_pr[] = {VAR, VAR_SET, FUNC};

/*PROG    ::= CODE + 'end'
CODE    ::= {EXPR}*
EXPR    ::= LAYER | SERV {',' SERV}*
LAYER   ::= CYCLE | COND | FUNCSET
SERV    ::= VARSET | EQAL | FUNC
VARSET  ::= TYPE + VARID [+ '=' + VARID | SUM | FUNC]
EQUAL   ::= VARID + '=' + VARID | SUM | FUNC
INEQUAL ::= VARID + '<''>''<=''>=''==''!=' VARID | SUM | FUNC
CYCLE   ::= 'for' | 'while' + '(' + INEQUAL + ')' + '{' + [EXPR]+ + '}'
COND    ::= if + '(' + EQUAL | INEQUAL + ')' + '{' + [EXPR]+ + '}' + ['else if' + '(' + EQUAL | INEQUAL + ')' + '{' + [EXPR]+ + '}']* + ['else' + '{' + [EXPR]+ + '}']*
FUNC    ::= ID + '(' + (VARID | SUM) {',' (VARID | SUM)}* + ')'
FUNCSET ::= TYPE + ID + '(' + {VARSET}+ + ')' + '{' + [EXPR]+ + '}'
SUM     ::= MUL {'+''-' MUL}*
MUL     ::= BR {'*''/' BR}*
BR      ::= '(' + SUM + ')' | NUM | VARID
ID      ::= a-z [a-z | 0-9]*
NUM     ::= [0-9]+


программа = код + "конец"
код = {выражение}*
выражение = новые слои | [служебные выражения + ,]
новые слои = цикл | условие | объявление функции + тело функции}*
служебные выражения = {объявление переменной (с присваиванием или нет)| равенство | неравенство| функция
объявление переменной = тип + идентификатор [+ присваивание значения {переменная | число}]
равенство = переменная + равенство + другая переменная | число
неравенство = переменная + неравенство + другия переменная | число
цикл = если | пока + ( + равенство | неравенство + ) + { + [выражения]+ + }
условие = если + ( + равенство | неравенство + ) + { + [выражения]+ + } + [если иначе + ( + равенство | неравенство + ) + { + [выражения]+ + }] + [иначе + { + [выражения]+ + }]
функция = идентификатор + (переменные | числа)
объявление функции = тип + идентификатор + ( + тип идентификатор... + ) + { + [выражения]+ + }
сум = умн {+ - умн}*
умн = скоб {* / скоб}*
скоб = (+ сум + ) | число
ид = буква [буквы или цифры]
число = [цифры]*/