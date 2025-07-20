#include "../include/lex_analysis.h"

#include "../include/lex_analysis.h"

static size_t count_words(Word* words);
static size_t add_id(Lexer* lexer, IDType type);
static size_t check_id(Lexer* lexer);
static void create_scope(Lexer* lexer);
static void destroy_scope(Lexer* lexer);
static size_t find_keyword(KeyWordType type);
static KeyWordType is_keyword(char* word, KeyWordType* types, size_t len);
static void set_token(TokenType type, int value, Lexer* lexer);
static void process_current_token(Lexer* lexer);
static bool process_num_token(Lexer* lexer);
static bool process_declaration_token(Lexer* lexer);
static KeyWordType process_keyword_token(Lexer* lexer);
static int process_id_token(Lexer* lexer);


//-------------------ctors-------------------------------

Lexer lexer_ctor(Word* words)
{
    size_t words_amt = count_words(words);
    Token* tokens = tokens_ctor(words, words_amt);

    Lexer lexer = {};
    lexer.words_amt = words_amt;
    lexer.tokens = tokens;

    Scope* scopes = scopes_ctor();

    lexer.scopes = scopes;

    return lexer;
}

//scope - область видимости. массив областей + первая таблица, состоящая из идентификаторов. 
IDTable id_table_ctor() 
{
    IDTable table = {};
    table.table = (ID*)calloc(MAX_ID_TABLE_SIZE, sizeof(ID));

    for (size_t id_num = 0; id_num < MAX_ID_TABLE_SIZE; id_num++) 
        table.table[id_num].word = (char*)calloc(MAX_WORD_LEN, sizeof(char));

    return table;
}

Scope* scopes_ctor()
{
    Scope* scopes = (Scope*)calloc(1, sizeof(Scope));

    scopes[0].ID_table = id_table_ctor();

    return scopes;
}


Token* tokens_ctor(Word* words, size_t words_amt)
{
    Token* tokens = (Token*)calloc(words_amt, sizeof(Token));

    for (size_t token_cnt = 0; token_cnt < words_amt; token_cnt++)
    {
        char* word = words[token_cnt].word;

        tokens[token_cnt].word = strdup(word);
        tokens[token_cnt].line = words[token_cnt].line_num;
    }

    words_dtor(words);

    return tokens;
}

//--------------------dtors---------------------------------------

void lexer_dtor(Lexer* lexer)
{
    tokens_dtor(lexer->tokens, lexer->words_amt);

    scopes_dtor(lexer->scopes, lexer->cur_scope);
}

void tokens_dtor(Token* tokens, size_t words_amt)
{
    for (size_t words_cnt = 0; words_cnt < words_amt; words_cnt++)
        free(tokens[words_cnt].word);

    free(tokens);
}

//void id_table_dtor(IDTable table)
//{
//    ID* id_table = table.table;
//    for (size_t id_num = 0; id_num < MAX_ID_TABLE_SIZE; id_num++)
//    free(id_table[id_num].word);
//
//    free(id_table);
//}

void id_table_dtor(IDTable* table) {  // Принимает указатель
    if (!table || !table->table) return;
    
    for (size_t id_num = 0; id_num < MAX_ID_TABLE_SIZE; id_num++) {
        free(table->table[id_num].word);
    }
    free(table->table);
    table->table = NULL;  // Явно обнуляем указатель
}

void scopes_dtor(Scope* scopes, size_t scopes_amt)
{
    for (size_t scopes_cnt = 0; scopes_cnt < scopes_amt; scopes_cnt++)
    {
        IDTable id_table = scopes[scopes_cnt].ID_table;
        id_table_dtor(&id_table);
    }

    free(scopes);
}

//----------------------tools-----------------------------------------

size_t count_words(Word* words)
{
    size_t word_cnt = 0;
    while(words[word_cnt].word[0] != '\0')  word_cnt++;

    return word_cnt;
}


//size_t add_id(Lexer* lexer, IDType type)
//{
//    Token* tokens = lexer->tokens;
//    char* word = tokens[lexer->word_cnt].word;
//
//    Scope* scopes = lexer->scopes;
//    size_t cur_scope_num = lexer->cur_scope;
//
//    Scope cur_scope = scopes[cur_scope_num];
//    IDTable cur_table = cur_scope.ID_table;
//
//    strcpy(cur_table.table[cur_table.len].word, word);
//    cur_table.table[cur_table.len].type = type;
//
//    cur_table.len++;
//
//    return cur_table.len - 1;
//}

size_t add_id(Lexer* lexer, IDType type) 
{
    // Работаем через указатели, а не копии!
    Scope* current_scope = &lexer->scopes[lexer->cur_scope];
    IDTable* table = &current_scope->ID_table;
    
    // Проверка на переполнение таблицы идентификаторов
    if (table->len >= MAX_ID_TABLE_SIZE) {
        return ERR_VAL_SIZE_T;
    }

    // Копируем слово
    strncpy(table->table[table->len].word, 
            lexer->tokens[lexer->word_cnt].word,
            MAX_WORD_LEN - 1);
    table->table[table->len].word[MAX_WORD_LEN - 1] = '\0';  // Гарантируем завершение строки
    
    // Устанавливаем тип
    table->table[table->len].type = type;
    
    // Возвращаем индекс ДО увеличения счётчика
    size_t added_index = table->len;
    table->len++;
    
    return added_index;
}

size_t check_id(Lexer* lexer)
{
    Token* tokens = lexer->tokens;
    char* word = tokens[lexer->word_cnt].word;

    Scope* scopes = lexer->scopes;
    size_t cur_scope_num = lexer->cur_scope;

    for (int scope_cnt = cur_scope_num; scope_cnt >= 0; scope_cnt--)
    {
        printf("scope_cnt %d\n", scope_cnt);

        IDTable table = scopes[scope_cnt].ID_table;
        size_t cur_id_amt = table.len;
        ID* id_table = table.table;

        for (int id_num = cur_id_amt; id_num >= 0; id_num--)
        {
            printf("id_num %d, word %s\n", id_num, id_table[id_num].word);
            if (!strcmp(word, id_table[id_num].word))
                return id_num + scope_cnt * 10;              //id_num <= 10 --> можно однозначно определить обе цифры из того, что получилось 
        }
    }

    return ERR_VAL_SIZE_T;
}



//void create_scope(Lexer* lexer)
//{
//    size_t scopes_amt = lexer->cur_scope + 1;
//    lexer->cur_scope++;
//
//    lexer->scopes = (Scope*)realloc(lexer->scopes, scopes_amt * sizeof(Scope));
//
//    lexer->scopes[scopes_amt].ID_table = id_table_ctor();
//}

void create_scope(Lexer* lexer) 
{
    if (!lexer) return;

    // Увеличиваем количество областей
    size_t new_scope_count = lexer->cur_scope + 1;
    
    // Выделяем память под новый массив (на 1 элемент больше)
    Scope* new_scopes = (Scope*)realloc(lexer->scopes, (new_scope_count + 1) * sizeof(Scope));
    if (!new_scopes) {
        fprintf(stderr, "Failed to allocate memory for new scope\n");
        return;
    }
    
    // Инициализируем новую область видимости
    new_scopes[new_scope_count].ID_table = id_table_ctor();
    new_scopes[new_scope_count].scope_ID = new_scope_count;  // Устанавливаем ID
    
    // Обновляем указатель и счётчик
    lexer->scopes = new_scopes;
    lexer->cur_scope = new_scope_count;
}


void destroy_scope(Lexer* lexer) 
{
    printf("here!!\n");
    // Проверка на NULL и минимальный scope
    if (!lexer || !lexer->scopes || lexer->cur_scope == 0) {
        printf("problem 1\n");
        return;
    }

    // Освобождаем ресурсы текущей области видимости
    size_t scope_to_remove = lexer->cur_scope;
    id_table_dtor(&(lexer->scopes[scope_to_remove].ID_table));

    // Уменьшаем количество областей
    lexer->cur_scope--;

    // Безопасное изменение размера массива
    size_t new_size = lexer->cur_scope + 1;  // +1 потому что индексы с 0
    Scope* new_scopes = (Scope*)realloc(lexer->scopes, new_size * sizeof(Scope));
    
    if (new_scopes || new_size == 0) {  // Принимаем NULL только если new_size == 0
        lexer->scopes = new_scopes;
    } else {
        // Ошибка realloc, но старый указатель остаётся валидным
        fprintf(stderr, "Warning: realloc failed in destroy_scope\n");
    }
}

size_t find_keyword(KeyWordType type)
{
    for (size_t word_cnt = 0; word_cnt < KEY_WORDS_AMT; word_cnt++)
    {
        if (key_words[word_cnt].type == type)
            return word_cnt;
    }

    return ERR_VAL_SIZE_T;
}


KeyWordType is_keyword(char* word, KeyWordType* types, size_t len)
{
    for (size_t word_cnt = 0; word_cnt < len; word_cnt++)
    {
        size_t word_num = find_keyword(types[word_cnt]);
        ///                                                                                ПРОВЕРИТЬ ВСЕ СТРКМП И ВСТАВИТЬ В ДЛИНУ МИНИМУМ
        if (!strncmp(word, key_words[word_num].word, strlen(key_words[word_num].word)))
            return types[word_cnt];
            
    }
    return UNKNOWN;
}

//----------------main funcs-------------------------------

void lex_analysis(Lexer* lexer)
{
    size_t words_amt = lexer->words_amt;
    //printf("word amt %d\n", words_amt);

    for (size_t word_cnt = 0; word_cnt < words_amt; word_cnt++)
    {
        printf("%s\n\n", lexer->tokens[word_cnt].word);
        lexer->word_cnt = word_cnt;
        process_current_token(lexer);

        //for (int i = 0; i < 10; i++)
        //{
        //    printf("%s, %d, %d, %d, %d\n", lexer->tokens[i].word, lexer->tokens[i].line, lexer->tokens[i].scope, lexer->tokens[i].type, lexer->tokens[i].value);
        //}
        //printf("\n");

        word_cnt = lexer->word_cnt;
    } 
}

void set_token(TokenType type, int value, size_t scope, Lexer* lexer) 
{
    Token* token = &lexer->tokens[lexer->word_cnt];  // Работаем через УКАЗАТЕЛЬ
    token->type = type;                               // Изменяем оригинал
    token->value = value;
    token->scope = scope;
}

void process_current_token(Lexer* lexer)
{
    if (process_num_token(lexer)) return;

    if (process_declaration_token(lexer)) return;

    if (process_keyword_token(lexer) != UNKNOWN) return;

    if (process_id_token(lexer)) return;
}

bool process_num_token(Lexer* lexer)
{
    printf("num\n");
    Token token = lexer->tokens[lexer->word_cnt];
    char* word = token.word;

    bool is_created = false;
    if (isdigit(word[0]))
    {
        int value = atoi(word);
        set_token(NUM, value, lexer->cur_scope, lexer);
        is_created = true;
    }

    return is_created;
}

bool process_declaration_token(Lexer* lexer)
{
    printf("declaration\n");

    bool is_created = false;
    Token token = lexer->tokens[lexer->word_cnt];
    char* word = token.word;

    KeyWordType type = is_keyword(word, forming_set, forming_set_amt);
    if (type == UNKNOWN)    return is_created;
    is_created = true;

    IDType id_type = VAR_ID;
    TokenType token_type= VAR;

    char* is_keyword_word = lexer->tokens[lexer->word_cnt + 2].word;        //слово для проверки, кокой тип устанавливает это слово - функция или переменную

    if (!strncmp(is_keyword_word, KW(OPEN_BRACE), strlen(KW(OPEN_BRACE))))
    {
        set_token(FUNC_SET, type, lexer->cur_scope, lexer);
        id_type = FUNC_ID;
        token_type = FUNC;
    }
    else
        set_token (VAR_SET, type, lexer->cur_scope, lexer);

    lexer->word_cnt++;

    word = lexer->tokens[lexer->word_cnt].word;

    size_t id_num = add_id(lexer, id_type);

    set_token(token_type, id_num, lexer->cur_scope, lexer);

    return is_created;
}

KeyWordType process_keyword_token(Lexer* lexer)
{   
    printf("keyword\n");

    Token token = lexer->tokens[lexer->word_cnt];
    char* word = token.word;
    //printf("scope %d, word !%s!\n", token.scope, word);

    KeyWordType keyword_type = is_keyword(word, forming_new_scope, forming_new_scope_amt);

    if (keyword_type != UNKNOWN)   
        create_scope(lexer);

    keyword_type = is_keyword(word, delete_scope, delete_scope_amt);

    if (keyword_type != UNKNOWN)
        destroy_scope(lexer);
    else        
        keyword_type = is_keyword(word, key_word_rest, word_rest_amt);

    //printf("kw type %d\n", keyword_type);

    if (keyword_type == ERR_VAL_SIZE_T)  return keyword_type;

    set_token(KEY_WORD, keyword_type, lexer->cur_scope, lexer);

    return keyword_type;
}

int process_id_token(Lexer* lexer)
{
    printf("id\n");
    size_t id_pos = check_id(lexer);

    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n%d\n", id_pos);

    if (id_pos == ERR_VAL_SIZE_T) return false;

    size_t id_num = id_pos % 10;
    size_t scope = (id_pos - id_num) / 10;

    TokenType type = VAR;

    char* is_keyword_word = lexer->tokens[lexer->word_cnt + 1].word;
    if (!strncmp(is_keyword_word, KW(OPEN_BRACE), strlen(KW(OPEN_BRACE))))
        type = FUNC;

    set_token(type, id_num, scope, lexer);

    return true;
}