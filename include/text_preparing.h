#pragma once

//-----libs-----------------

#include <ctype.h>
#include <sys/stat.h>

#include "error_reporter.h"

//-----consts------------------

const size_t WORDS_AMT = 150;

//-----structs---------------

struct Word
{
    char* word;
    size_t line_num;
};

//-----funcs--------------------

Word* words_ctor(FILE* log_file);

void words_dtor(Word* words);

Word* text_preparing(const char const* file_name, FILE* log_file);