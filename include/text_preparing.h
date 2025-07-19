#pragma once

#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

#include "structure.h"

const size_t WORDS_AMT = 150;

struct Word
{
    char* word;
    size_t line_num;
};

Word* text_preparing(char* file_name);

Word* words_ctor();

void words_dtor(Word* words);