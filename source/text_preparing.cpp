#include "../include/text_preparing.h"

//----------------------------

static char* file_2_buffer(char* file_name);
char* remove_comments(char* buffer);
static Word* split_text_into_words(char* text);

void count_file_size(char* file_name, size_t* size);

//---------------------------------------

Word* text_preparing(char* file_name)
{
    char* buffer = file_2_buffer(file_name);
    char* text = remove_comments(buffer);
    Word* words = split_text_into_words(text); 

    return words;
}

char* file_2_buffer(char* file_name)
{
    FILE* file = fopen(file_name, "r");
    size_t size = 0;

    count_file_size(file_name, &size);

    char* text = (char*)calloc(size, sizeof(char));

    fread(text, sizeof(char), size, file);

    fclose(file);
    return text;
}

void count_file_size(char* file_name, size_t* size) 
{
    struct stat file_info;
    
    stat(file_name, &file_info);
    
    *size = file_info.st_size;
}

char* remove_comments(char* buffer)
{
    size_t size = strlen(buffer);
    size_t line_len = strlen(KW(LINE_COMMENT));
    size_t multi_start_len = strlen(KW(MULTI_LINE_COMMENT_START));
    size_t multi_end_len = strlen(KW(MULTI_LINE_COMMENT_END));

    char* text = (char*)calloc(size + 1, sizeof(char));

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

    free(buffer);

    return text;
}

Word* split_text_into_words(char* text)
{
    Word* words = (Word*)calloc(WORDS_AMT, sizeof(Word));

    size_t pointer = 0;
    size_t line_cnt = 0;
    size_t word_cnt = 0;
    size_t word_len = 0;

    while(pointer < strlen(text))
    {
        if (isspace(text[pointer]))
        {
            if (text[pointer] == '\n')
                line_cnt++;

            pointer++;
            continue;
        }
       
        words[word_cnt].line_num = line_cnt;
        words[word_cnt].word_start = text + pointer;
        word_len = 0;

        while (!isspace(text[pointer]))
        {
            word_len++;
            pointer++;
        }

        words[word_cnt].word_len = word_len;
        word_cnt++;
    }

    return words;
}