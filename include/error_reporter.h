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
    WORDS_OVERFLOW,
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

inline const char* system_err_output[] = {"Calloc error", 
                                          "can't open file",
                                          "can't close file",
                                          "invalid data",
                                          "stat failed",
                                          "EOF error",
                                          "fread error",
                                          "fclose error",
                                          "what have you done?"};

inline const char* lang_err_output[] = {"too much id in the scope\n"};

#define SYS_ERR_REP_PTR(sys_err_type)       do                                                                                      \
                                            {                                                                                       \
                                                int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file);    \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                    printf("fprintf error while printing error message\n");                         \
                                                return nullptr;                                                                     \
                                            }while(0);      
                                            
#define SYS_ERR_REP_VOID(sys_err_type)      do                                                                                      \
                                            {                                                                                       \
                                                int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file);    \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                    printf("fprintf error while printing error message\n");                         \
                                                return;                                                                             \
                                            }while(0);   

#define SYS_ERR_REP_INT(sys_err_type)       do                                                                                      \
                                            {                                                                                       \
                                                int output_res = output_sys_err_msg(__LINE__, __FILE__, sys_err_type, log_file);    \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                    printf("fprintf error while printing error message\n");                         \
                                                return ERR_VAL_INT;                                                                 \
                                            }while(0);                                                                           

#define LANG_ERR_REP_PTR(lang_err_type)    do                                                                                           \
                                                {                                                                                       \
                                                    int output_res = output_lang_err_msg(__LINE__, __FILE__, lang_err_type, log_file);  \
                                                    if (output_res == ERR_VAL_INT)                                                      \
                                                    {                                                                                   \
                                                        printf("fprintf error while printing error message\n");                         \
                                                        return nullptr;                                                                 \
                                                    }                                                                                   \
                                                }while(0);   
                                            
#define LANG_ERR_REP_INT(lang_err_type)    do                                                                                       \
                                            {                                                                                       \
                                                int output_res = output_lang_err_msg(__LINE__, __FILE__, lang_err_type, log_file);  \
                                                if (output_res == ERR_VAL_INT)                                                      \
                                                {                                                                                   \
                                                    printf("fprintf error while printing error message\n");                         \
                                                    return ERR_VAL_INT;                                                             \
                                                }                                                                                   \
                                            }while(0);     

#define ALLOCATION_CHECK_PTR(ptr)           do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_PTR(ALLOCATION_ERROR)                                               \
                                            }while(0);   
                        
#define ALLOCATION_CHECK_VOID(ptr)          do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_VOID(ALLOCATION_ERROR)                                              \
                                            }while(0);  

#define ALLOCATION_CHECK_INT(ptr)          do                                                                                       \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_INT(ALLOCATION_ERROR)                                               \
                                            }while(0);  

#define DATA_CHECK_PTR(ptr)                 do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_PTR(INPUT_DATA_ERROR)                                               \
                                            }while(0);        

#define DATA_CHECK_INT(ptr)                 do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_INT(INPUT_DATA_ERROR)                                               \
                                            }while(0);    

#define FILE_OPEN_CHECK(ptr)                do                                                                                      \
                                            {                                                                                       \
                                                if (!ptr)                                                                           \
                                                    SYS_ERR_REP_PTR(FILE_OPENING_ERROR)                                             \
                                            }while(0);  

#define STAT_CHECK(stat_res)                do                                                                                      \
                                            {                                                                                       \
                                                if (stat_res == -1)                                                                 \
                                                    SYS_ERR_REP_INT(STAT_ERROR)                                                     \
                                            }while(0);  

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
                                            }while(0);  

#define FILE_CLOSE_CHECK(fclose_res)        do                                                                                      \
                                            {                                                                                       \
                                                if (fclose_res == EOF)                                                              \
                                                    SYS_ERR_REP_PTR(FCLOSE_ERROR)                                                   \
                                            }while(0);  

#define PTR_FUNC_CHECK_PTR(func_output)     do                                                                                      \
                                            {                                                                                       \
                                                if (!func_output)                                                                   \
                                                    SYS_ERR_REP_PTR(RETURN_MSG)                                                     \
                                           }while(0);  

#define INT_FUNC_CHECK_PTR(func_output)     do                                                                                      \
                                            {                                                                                       \
                                                if (func_output == ERR_VAL_INT)                                                     \
                                                    SYS_ERR_REP_PTR(RETURN_MSG)                                                     \
                                            }while(0);  

#define PTR_FUNC_CHECK_INT(func_output)     do                                                                                      \
                                            {                                                                                       \
                                                if (!func_output)                                                                   \
                                                    SYS_ERR_REP_INT(RETURN_MSG)                                                     \
                                            }while(0);  

#define INT_FUNC_CHECK_INT(func_output)     do                                                                                      \
                                            {                                                                                       \
                                                if (func_output == ERR_VAL_INT)                                                     \
                                                    SYS_ERR_REP_INT(RETURN_MSG)                                                     \
                                            }while(0);  

#define ID_OVERFLOW_CHECK(table_len)        do                                                                                      \
                                            {                                                                                       \
                                                if (table_len >= MAX_ID_TABLE_SIZE)                                                 \
                                                    LANG_ERR_REP_INT(ID_TABLE_OVERFLOW)                                             \
                                            }while(0);  
        

FILE* err_reporter_ctor();
int err_reporter_dtor(FILE* log_file);
int output_sys_err_msg(size_t line, const char *const file_name, SystemErrorType type, FILE* log_file);
int output_lang_err_msg_help(size_t line, const char *const file_name, LangErrorType type, char* expected_answer, FILE* log_file);
int output_lang_err_msg(size_t line, const char *const file_name, LangErrorType type, FILE* log_file);
size_t safe_strncpy(char *dest, const char *src, size_t size, FILE* log_file);
char* safe_strdup(const char *src, FILE* log_file);