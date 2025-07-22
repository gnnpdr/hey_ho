#include "../include/text_preparing.h"

//-------------------------------------------

static int count_file_size(const char *const file_name, size_t* size, FILE* log_file);          //? перенести в common.cpp?

static char* file_2_buffer(const char *const file_name, FILE* log_file);
static char* remove_comments(char* buffer, FILE* log_file);
static Word* split_text_into_words(char* text, FILE* log_file);

//-------------ctors--------------------------

Word* words_ctor(FILE* log_file)
{
    Word* words = (Word*)calloc(WORDS_AMT, sizeof(Word));
    ALLOCATION_CHECK_PTR(words)
    
    for (size_t word_cnt = 0; word_cnt < WORDS_AMT; word_cnt++)
    {
        words[word_cnt].word = (char*)calloc(MAX_WORD_LEN, sizeof(char));
        ALLOCATION_CHECK_PTR(words[word_cnt].word)
    }
        
    return words;
}

//-------------dtor----------------------

void words_dtor(Word* words)
{
    for (size_t word_cnt = 0; word_cnt < WORDS_AMT; word_cnt++) 
        safe_free((void**)&words[word_cnt].word);

    safe_free((void**)&words);
}

//-------------tools-------------------

int count_file_size(const char *const file_name, size_t* size, FILE* log_file) 
{
    DATA_CHECK_INT(file_name)
    DATA_CHECK_INT(size)

    struct stat file_info;
    
    int stat_res = stat(file_name, &file_info);
    STAT_CHECK(stat_res)
    
    *size = file_info.st_size;

    return 0;
}

//-----------main--------------------------------

Word* text_preparing(const char *const file_name, FILE* log_file)
{
    DATA_CHECK_PTR(file_name)

    char* buffer = file_2_buffer(file_name, log_file);
    PTR_FUNC_CHECK_PTR(buffer)
    
    char* text = remove_comments(buffer, log_file);

    PTR_FUNC_CHECK_PTR(text)
    Word* words = split_text_into_words(text, log_file); 
    PTR_FUNC_CHECK_PTR(words)


    return words;
}

char* file_2_buffer(const char *const file_name, FILE* log_file)
{
    DATA_CHECK_PTR(file_name)
    DATA_CHECK_PTR(log_file)
    
    FILE* file = fopen(file_name, "r");
    FILE_OPEN_CHECK(file)

    size_t size = 0;
    int func_output = count_file_size(file_name, &size, log_file);
    INT_FUNC_CHECK_PTR(func_output)
    
    char* buffer = (char*)calloc(size, sizeof(char));
    ALLOCATION_CHECK_PTR(buffer)

    size_t read_symbs = fread(buffer, sizeof(char), size, file);
    FREAD_CHECK(file)

    int fclose_res = fclose(file);
    FILE_CLOSE_CHECK(fclose_res)
    file = NULL;

    return buffer;
}

char* remove_comments(char* buffer, FILE* log_file)
{
    DATA_CHECK_PTR(buffer)
    DATA_CHECK_PTR(log_file)

    size_t size = strlen(buffer);
    size_t line_len = strlen(KW(LINE_COMMENT));
    size_t multi_start_len = strlen(KW(MULTI_LINE_COMMENT_START));
    size_t multi_end_len = strlen(KW(MULTI_LINE_COMMENT_END));

    char* text = (char*)calloc(size + 1, sizeof(char));
    ALLOCATION_CHECK_PTR(text)

    size_t buf_pointer = 0, text_pointer = 0;

    while (buf_pointer < size)
    {
        if (!strncmp(buffer + buf_pointer, KW(LINE_COMMENT), line_len))
        {
            buf_pointer += line_len;
            while (buf_pointer < size && buffer[buf_pointer] != '\n')
                buf_pointer++;

            text[text_pointer++] = '\n';
            buf_pointer++;

            continue;
        }
        else if (!strncmp(buffer + buf_pointer, KW(MULTI_LINE_COMMENT_START), multi_start_len))
        {
            buf_pointer += multi_start_len;
            while(buf_pointer < size && strncmp(buffer + buf_pointer, KW(MULTI_LINE_COMMENT_END), multi_end_len))
                buf_pointer++;
            buf_pointer += multi_end_len;

            continue;
        }

        text[text_pointer++] = buffer[buf_pointer++];
    }

    text[text_pointer] = '\0';

    safe_free((void**)&buffer);

    return text;
}

Word* split_text_into_words(char* text, FILE* log_file)
{
    DATA_CHECK_PTR(text)
    DATA_CHECK_PTR(log_file)
    
    Word* words = words_ctor(log_file);

    size_t pointer = 0, line_cnt = 1, word_cnt = 0, word_len = 0;

    while(text[pointer] != '\0')
    {
        if (isspace(text[pointer]))
        {
            if (text[pointer] == '\n')
                line_cnt++;

            pointer++;
            continue;
        }
       
        words[word_cnt].line_num = line_cnt;
        word_len = 0;

        char* word_start = text + pointer;

        if (isalnum(text[pointer]))
        {
            while (isalnum(text[pointer]))
            {
                word_len++;
                pointer++;
            }
        }
        else
        {
            while (ispunct(text[pointer]))
            {
                word_len++;
                pointer++;
            }
        }

        //safe_strncpy(words[word_cnt++].word, text + pointer - word_len, word_len, log_file);

        if (word_cnt < WORDS_AMT)
        {
            words[word_cnt].line_num = line_cnt;
            
            // Выделяем память под слово (+1 для '\0')
            //words[word_cnt].word = (char*)calloc(word_len + 1, sizeof(char));
            //if (!words[word_cnt].word)
            //{
            //    output_sys_err_msg(__LINE__, __FILE__, ERR_MEM_ALLOC, log_file);
            //    break;
            //}
            
            // Копируем именно word_len символов
            strncpy(words[word_cnt].word, word_start, word_len);
            words[word_cnt].word[word_len] = '\0'; // Явно добавляем терминатор
            
            word_cnt++;
        }
        else
        {
            output_sys_err_msg(__LINE__, __FILE__, WORDS_OVERFLOW, log_file);
            break;
        }
    }

    safe_free((void**)&text);

    return words;
}