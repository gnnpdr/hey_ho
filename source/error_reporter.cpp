#include "../include/error_reporter.h"

FILE* err_reporter_ctor()
{
    FILE* log_file = fopen("log_file.txt", "a");

    return log_file;
}

int err_reporter_dtor(FILE* log_file)
{
    int fclose_res = fclose(log_file);
    if (fclose_res == EOF)
    {
        printf("problem with losing log file\n");
        return ERR_VAL_INT;
    }

    log_file = NULL;

    return 0;
}

int output_sys_err_msg(size_t line, const char *const file_name, SystemErrorType type, FILE* log_file)
{
    int written_bytes = -1; 

    if (type != RETURN_MSG)
        written_bytes = fprintf(log_file, "[ERROR] %s, FILE %s, LINE %ld\n", system_err_output[type], file_name, line);
    else
        written_bytes = fprintf(log_file, "[RETURN], FILE %s, LINE %ld\n", file_name, line);

    if (written_bytes < 0)
        return ERR_VAL_INT;

    return 0;
}

int output_lang_err_msg_help(size_t line, const char *const  file_name, LangErrorType type, char* expected_answer, FILE* log_file)
{
    int written_bytes = -1; 

    if (expected_answer)
        written_bytes = fprintf(log_file, "[ERROR] %s, maybe you meant '%s'??, FILE %s, LINE %ld\n", lang_err_output[type], expected_answer, file_name, line);
    else
        written_bytes = fprintf(log_file, "[ERROR] %s, FILE %s, LINE %ld\n",lang_err_output[type], file_name, line);

    if (written_bytes < 0)
        return ERR_VAL_INT;

    return 0;
}

int output_lang_err_msg(size_t line, const char *const file_name, LangErrorType type, FILE* log_file)
{
    int written_bytes = fprintf(log_file, "[ERROR] %s, FILE %s, LINE %ld\n", lang_err_output[type], file_name, line);
    if (written_bytes < 0)
        return ERR_VAL_INT;

    return 0;
}

void safe_free(void **ptr) 
{
    if (ptr && *ptr) 
    {
        free(*ptr);
        *ptr = NULL;
    }
}


size_t safe_strncpy(char *dest, const char *src, size_t size, FILE* log_file) 
{
    DATA_CHECK_INT(dest)
    DATA_CHECK_INT(src)
    DATA_CHECK_INT(size)

    size_t cnt = 0;
    for (cnt = 0; cnt < size - 1 && src[cnt] != '\0'; cnt++)
        dest[cnt] = src[cnt];

    dest[cnt] = '\0';
    return cnt;
}

char* safe_strdup(const char *src, FILE* log_file) 
{
    DATA_CHECK_PTR(src)

    char* copy = strdup(src);
    ALLOCATION_CHECK_PTR(copy)

    return copy;
}