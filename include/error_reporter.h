#pragma once

#include <stdlib.h>
#include <string.h>

#include "structure.h"

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
    EOF_ERROR,
    FREAD_ERROR,
    FCLOSE_ERROR,
    UNKNOWN_ERROR
};

enum LangErrorType
{
    ID_TABLE_OVERFLOW,
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
                                    "EOF error\n",
                                    "fread error\n",
                                    "fclose error\n",
                                    "what have you done?\n"};

inline char* lang_err_output[] = {"too much id in the scope\n"};

#define SYS_ERR_REP_PTR(sys_err_type)       do                                                                                      \
                                            {                                                                                       \
                                                int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file);    \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                {                                                                                   \
                                                    printf("fprintf error while printing error message\n");                         \
                                                    return nullptr;                                                                 \
                                                }                                                                                   \
                                            }while(true);      
                                            
#define SYS_ERR_REP_VOID(sys_err_type)      do                                                                                      \
                                            {                                                                                       \
                                                int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file);    \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                {                                                                                   \
                                                    printf("fprintf error while printing error message\n");                         \
                                                    return;                                                                         \
                                                }                                                                                   \
                                            }while(true);   

#define SYS_ERR_REP_INT(sys_err_type)       do                                                                                      \
                                            {                                                                                       \
                                                int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file);    \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                {                                                                                   \
                                                    printf("fprintf error while printing error message\n");                         \
                                                    return ERR_VAL_INT;                                                             \
                                                }                                                                                   \
                                            }while(true);                                                                           

#define LANG_ERR_REP_PTR(lang_err_type, ...)    do                                                                                  \
                                            {                                                                                       \
                                                int output_lang_err_msg(__LINE__, __FILE__, lang_err_type, ##__VA_ARGS__, log_file); \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                {                                                                                   \
                                                    printf("fprintf error while printing error message\n");                         \
                                                    return nullptr;                                                                 \
                                                }                                                                                   \
                                            }while(true);   
                                            
#define LANG_ERR_REP_INT(lang_err_type, ...)    do                                                                                  \
                                            {                                                                                       \
                                                int output_lang_err_msg(__LINE__, __FILE__, lang_err_type, ##__VA_ARGS__, log_file); \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                {                                                                                   \
                                                    printf("fprintf error while printing error message\n");                         \
                                                    return ERR_VAL_INT;                                                             \
                                                }                                                                                   \
                                            }while(true);     

#define ALLOCATION_CHECK_PTR(ptr)           do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_PTR(ALLOCATION_ERROR)                                               \
                                            }while(true);   
                        
#define ALLOCATION_CHECK_VOID(ptr)          do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_VOID(ALLOCATION_ERROR)                                              \
                                            }while(true);  

#define DATA_CHECK_PTR(ptr)                 do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_PTR(INPUT_DATA_ERROR)                                               \
                                            }while(true);       

#define DATA_CHECK_INT(ptr)                 do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_INT(INPUT_DATA_ERROR)                                               \
                                            }while(true);   

#define FILE_OPEN_CHECK(ptr)                do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_PTR(FILE_OPENING_ERROR)                                             \
                                            }while(true); 

#define STAT_CHECK(stat_res)                do                                                                                      \
                                            {                                                                                       \
                                                if (stat_res == -1)                                                                 \
                                                    SYS_ERR_REP_INT(STAT_ERROR)                                                     \
                                            }while(true);

#define FREAD_CHECK(file)                   do                                                                                      \
                                            {                                                                                       \
                                                if (read_symbs < size)                                                              \
                                                {                                                                                   \
                                                    if (feof(file))                                                                 \
                                                    {                                                                               \
                                                        printf("EOF, readen %zu from %zu bytes ", read_symbs, size);                \
                                                        SYS_ERR_REP_PTR(EOF_ERROR)                                                  \
                                                    }                                                                               \
                                                    else if (ferror(file))                                                          \
                                                        SYS_ERR_REP_PTR(FREAD_ERROR)                                                \
                                                }                                                                                   \
                                            }while(true);

#define FILE_CLOSE_CHECK(fclose_res)        do                                                                                      \
                                            {                                                                                       \
                                                if (fclose_res == EOF)                                                              \
                                                    SYS_ERR_REP_PTR(FCLOSE_ERROR)                                                   \   
                                            }while(true);

#define PTR_FUNC_CHECK_PTR(func_output)     do                                                                                      \
                                            {                                                                                       \
                                                if (!func_output)                                                                   \
                                                    SYS_ERR_REP_PTR(RETURN_MSG)                                                     \
                                            }while(true);

#define INT_FUNC_CHECK_PTR(func_output)     do                                                                                      \
                                            {                                                                                       \
                                                if (func_output == ERR_VAL_INT)                                                     \
                                                    SYS_ERR_REP_PTR(RETURN_MSG)                                                     \
                                            }while(true);

#define ID_OVERFLOW_CHECK(table_len)        do                                                                                      \
                                            {                                                                                       \
                                                if (table_len >= MAX_ID_TABLE_SIZE)                                                 \
                                                    LANG_ERR_REP_INT(ID_TABLE_OVERFLOW)                                             \
                                            }while(true);
        

FILE* err_reporter_ctor();
int err_reporter_dtor(FILE* log_file);
int output_sys_err_msg(size_t line, char* file_name, SystemErrorType type, FILE* log_file);
int output_lang_err_msg_help(size_t line, char* file_name, LangErrorType type, char* expected_answer, FILE* log_file);
int output_lang_err_msg(size_t line, char* file_name, LangErrorType type, char* expected_answer, FILE* log_file);
size_t safe_strncpy(char *dest, const char *src, size_t size, FILE* log_file);
char* safe_strdup(const char *src, FILE* log_file);