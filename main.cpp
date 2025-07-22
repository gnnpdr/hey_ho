#include "include/error_reporter.h"
#include "include/text_preparing.h"
#include "include/lex_analysis.h"
//#include "include/syn_analysis.h"

int main(int argc, char ** argv)
{
    FILE* log_file = err_reporter_ctor();
    if (!log_file)
    {
        printf("problem with oppening log file :((\n");
        return 1;
    }

    Word* words = text_preparing(argv[1], log_file);
    PTR_FUNC_CHECK_INT(words)
    
//--------------------------------          
    //определить смысл слов, лексер
    Lexer* lexer = lexer_ctor(words, log_file);
    lex_analysis(lexer, log_file);

    //for (int i = 0; i < 31; i++)
    //{
    //    printf("%s, %d, %d, %d, %d\n", lexer->tokens[i].word, lexer->tokens[i].line, lexer->tokens[i].scope, lexer->tokens[i].type, lexer->tokens[i].value);
    //}
    //printf("\n");
//------------------------------
    //связать смысловые блоки, рекурсивный спуск
    //syn_analysis(words, id_table);


    err_reporter_dtor(log_file);
}