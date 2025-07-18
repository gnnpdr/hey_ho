#pragma once

#include <stdio.h>

//const size_t KEY_WORDS_AMT = 3;

#define KW(...) key_words[__VA_ARGS__].word

enum KeyWordType
{
    LINE_COMMENT,
    MULTI_LINE_COMMENT_START,
    MULTI_LINE_COMMENT_END
};

struct KeyWord
{
    size_t words_amt;
    const char* word;
};

inline KeyWord key_words[] = {{1, "censor"},
                        {1, "ban"},
                        {1, "unban"}};