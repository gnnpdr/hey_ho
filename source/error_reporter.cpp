#include "../include/error_reporter.h"

FILE* err_reporter_ctor()
{
    FILE* log_file = fopen("log_file.txt", "a");
    //if (!log_file)
    //{
    //    printf("problem with oppening log file :((\n");
    //    return 1;
    //}

    return log_file;
}

void err_reporter_dtor(FILE* log_file)
{
    
}

int output_sys_err_msg(size_t line, char* file_name, SystemErrorType type, FILE* log_file)
{
    int written_bytes = -1; 

    if (type != RETURN_MSG)
        written_bytes = fprintf(log_file, "[ERROR] %s, FILE %s, LINE %d\n", system_err_output[type], file_name, line);
    else
        written_bytes = fprintf(log_file, "[RETURN], FILE %s, LINE %d\n", file_name, line);

    if (written_bytes < 0)
        return 1;

    return 0;
}

int output_lang_err_msg(size_t line, char* file_name, LangErrorType type, char* expected_answer, FILE* log_file)
{
    int written_bytes = -1; 

    if (expected_answer)
        written_bytes = fprintf(log_file, "[ERROR] %s, maybe you meant '%s'??, FILE %s, LINE %d\n", system_err_output[type], expected_answer, file_name, line);
    else
        written_bytes = fprintf(log_file, "[ERROR] %s, FILE %s, LINE %d\n", system_err_output[type], file_name, line);

    if (written_bytes < 0)
        return 1;

    return 0;
}