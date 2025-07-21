#pragma once

#include "structure.h"

#include <stdlib.h>


const size_t ERR_VAL_SIZE_T = 155;
const int ERR_VAL_INT = -8;

enum SystemErrorType
{
    ALLOCATION_ERROR,
    FILE_OPENING_ERROR,
    FILE_CLOSING_ERROR, 
    RETURN_MSG,
    INPUT_DATA_ERROR,
    STAT_ERROR,
    UNKNOWN_ERROR
};

enum LangErrorType
{
    UNKNOWN_VARIABLE,
    UNKNOWN_FUNC,
    MATH_ERROR
};


void safe_free(void **ptr);

inline char* system_err_output[] = {"Calloc error\n", 
                                    "can't open file\n"
                                    "can't close file\n",
                                    "invalid data\n",
                                    "stat failed\n",
                                    "what have you done?\n"};

#define SYS_ERR_REP_PTR(sys_err_type) do                 \
                        {               \
                            int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file); \
                            if (output_res == 1)                                                            \
                            {                                                                                  \
                                printf("fprintf error\n");                                      \
                                return nullptr;                                             \
                            }                                                                               \
                        }while(true);   \

#define SYS_ERR_REP_INT(sys_err_type) do                 \
                        {               \
                            int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file); \
                            if (output_res == 1)                                                            \
                            {                                                                                  \
                                printf("fprintf error\n");                                      \
                                return 1;                                             \
                            }                                                                               \
                        }while(true);   \

#define LANG_ERR_REP(lang_err_type, ...) do                 \
                        {               \
                            int output_lang_err_msg(__LINE__, __FILE__, sys_err_type, ##__VA_ARGS__, log_file);\
                            if (output_res == 1)                                                            \
                            {                                                                                  \
                                printf("fprintf error\n");                                      \
                                return nullptr;                                             \
                            } \
                        }while(true);   \


#define ALLOCATION_CHECK(ptr)   do                 \
                        {               \
                            if (!ptr)   \
                            {   \
                                SYS_ERR_REP_PTR(ALLOCATION_ERROR)   \
                                return nullptr; \
                            } \
                        }while(true);   \

#define DATA_CHECK_PTR(ptr)   do                 \
                        {               \
                            if (!ptr)   \
                            {   \
                                SYS_ERR_REP_PTR(INPUT_DATA_ERROR)   \
                                return nullptr; \
                            } \
                        }while(true);   \

#define DATA_CHECK_INT(ptr)   do                 \
                        {               \
                            if (!ptr)   \
                            {   \
                                SYS_ERR_REP_INT(INPUT_DATA_ERROR)   \
                                return 1; \
                            } \
                        }while(true);   \

#define FILE_OPEN_CHECK(ptr)   do                 \
                        {               \
                            if (!ptr)   \
                            {   \
                                SYS_ERR_REP_PTR(FILE_OPENING_ERROR)   \
                                return nullptr; \
                            } \
                        }while(true);   \

#define STAT_CHECK(stat_res)   do                 \
                        {               \
                            if (stat_res == -1)   \
                            {   \
                                SYS_ERR_REP_INT(STAT_ERROR)   \
                                return 1; \
                            } \
                        }while(true);   \


#define PTR_FUNC_CHECK_PTR(func_output)   do                 \
                        {               \
                            if (!func_output)   \
                            {   \
                                SYS_ERR_REP_PTR(RETURN_MSG)   \
                                return nullptr; \
                            } \
                        }while(true);   \

#define INT_FUNC_CHECK_PTR(func_output)   do                 \
                        {               \
                            if (func_output == ERR_VAL_INT)   \
                            {   \
                                SYS_ERR_REP_PTR(RETURN_MSG)   \
                                return nullptr; \
                            } \
                        }while(true);   \

FILE* err_reporter_ctor();
int err_reporter_dtor(FILE* log_file);
int output_sys_err_msg(size_t line, char* file_name, SystemErrorType type, FILE* log_file);
int output_lang_err_msg(size_t line, char* file_name, LangErrorType type, char* expected_answer, FILE* log_file);