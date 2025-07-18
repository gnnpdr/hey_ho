#include "include/text_preparing.h"
//#include "include/lex_analysis.h"
//#include "include/syn_analysis.h"

int main(int argc, char ** argv)
{
    //принять текст из файла в буфер, поделить на отдельные части
    //в этой функции выделяется место для текста из файла (буфер), для массива слов, содержащего данные по каждому слову
    //но не само слово, данные по нему есть в буфере
    Word* words = text_preparing(argv[1]);  //words = массив строк - слов текста
//--------------------------------          //токены же не сожержат само слово, а только сокращенную информацию
    
    //определить смысл слов, лексер
    //Token* tokens = words_ctor();
    //ID* ID_table = ID_table_ctor;
    //lex_analysis(words, tokens);
//------------------------------
    //связать смысловые блоки, рекурсивный спуск
    //syn_analysis(words, id_table);
}