#include "include/text_preparing.h"
#include "include/lex_analysis.h"
//#include "include/syn_analysis.h"

int main(int argc, char ** argv)
{
    Word* words = text_preparing(argv[1]);
//--------------------------------          

    //for (int i = 0; i < 10; i++)
    //{
    //    printf("%s, %d\n", words[i].word, words[i].line_num);
    //}
    //printf("\n");

    //определить смысл слов, лексер
    Lexer lexer = lexer_ctor(words);
    lex_analysis(&lexer);

    for (int i = 0; i < 31; i++)
    {
        printf("%s, %d, %d, %d, %d\n", lexer.tokens[i].word, lexer.tokens[i].line, lexer.tokens[i].scope, lexer.tokens[i].type, lexer.tokens[i].value);
    }
    printf("\n");
//------------------------------
    //связать смысловые блоки, рекурсивный спуск
    //syn_analysis(words, id_table);
}