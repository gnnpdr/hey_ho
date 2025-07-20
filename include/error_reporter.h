#pragma once

#include "structure.h"

enum SystemErrorType
{
    ALLOCATION_ERROR,
    FILE_OPENING_ERROR,
    FILE_CLOSING_ERROR, 
    RETURN_MSG,
    UNKNOWN_ERROR
};

enum LangErrorType
{
    UNKNOWN_VARIABLE,
    UNKNOWN_FUNC,
    MATH_ERROR
};


inline char* system_err_output[] = {"Calloc error\n", 
                                    "can't open file\n"
                                    "can't close file\n",
                                    "what have you done?\n"};

#define ERR_REP(...) 