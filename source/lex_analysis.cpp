#include "../include/lex_analysis.h"

#include "../include/lex_analysis.h"

static Scope* scopes_ctor(FILE* log_file);
static void id_table_ctor(IDTable* table, FILE* log_file);
static Token* tokens_ctor(Word* words, size_t words_amt, FILE* log_file);

static void tokens_dtor(Token* tokens, size_t words_amt);
static void id_table_dtor(IDTable* table);
static void scopes_dtor(Scope* scopes, size_t scopes_amt);

static int count_words(const Word *const words, FILE* log_file);
static int add_id(Lexer *const lexer, IDType type, FILE* log_file);
static int check_id(const Lexer *const lexer, FILE* log_file);
static int create_scope(Lexer *const lexer, FILE* log_file);
static int destroy_scope(Lexer* lexer, FILE* log_file);
static size_t find_keyword(KeyWordType type);
static KeyWordType is_keyword(char* word, KeyWordType* types, size_t len);
static int set_token(TokenType type, int value, size_t scope, Lexer* lexer, FILE* log_file);

static int process_current_token(Lexer* lexer, FILE* log_file);
static int process_num_token(Lexer* lexer, FILE* log_file);
static int process_declaration_token(Lexer* lexer, FILE* log_file);
static int process_keyword_token(Lexer* lexer, FILE* log_file);
static int process_id_token(Lexer* lexer, FILE* log_file);

//-------------------ctors-------------------------------

Lexer* lexer_ctor(Word* words, FILE* log_file)
{
    DATA_CHECK_PTR(words)

    int words_amt = count_words(words, log_file);
    INT_FUNC_CHECK_PTR(words_amt)

    Token* tokens = tokens_ctor(words, words_amt, log_file);
    PTR_FUNC_CHECK_PTR(tokens)

    Lexer *lexer = (Lexer*)calloc(1, sizeof(Lexer));
    ALLOCATION_CHECK_PTR(lexer)

    lexer->words_amt = words_amt;
    lexer->tokens = tokens;

    Scope* scopes = scopes_ctor(log_file);
    PTR_FUNC_CHECK_PTR(scopes)

    lexer->scopes = scopes;

    return lexer;
}

Scope* scopes_ctor(FILE* log_file) 
{
    Scope* scopes = (Scope*)calloc(1, sizeof(Scope));
    ALLOCATION_CHECK_PTR(scopes)

    id_table_ctor(&scopes[0].ID_table, log_file);

    return scopes;
}

void id_table_ctor(IDTable* table, FILE* log_file)    
{
    assert(table);

    table->table = (ID*)calloc(MAX_ID_TABLE_SIZE, sizeof(ID));
    ALLOCATION_CHECK_VOID(table->table)

    for (size_t id_num = 0; id_num < MAX_ID_TABLE_SIZE; id_num++) 
    {
        table->table[id_num].word = (char*)calloc(MAX_WORD_LEN, sizeof(char));
        ALLOCATION_CHECK_VOID(table->table[id_num].word)
    }

    table->len = 0;
}

Token* tokens_ctor(Word* words, size_t words_amt, FILE* log_file)
{
    DATA_CHECK_PTR(words)

    Token* tokens = (Token*)calloc(words_amt, sizeof(Token));
    ALLOCATION_CHECK_PTR(tokens)

    for (size_t token_cnt = 0; token_cnt < words_amt; token_cnt++)
    {
        DATA_CHECK_PTR(words[token_cnt].word)
        char* word = words[token_cnt].word;

        tokens[token_cnt].word = safe_strdup(word, log_file);
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
        safe_free((void**)&tokens[words_cnt].word);

    safe_free((void**)&tokens);
}

void id_table_dtor(IDTable* table) 
{
    for (size_t id_num = 0; id_num < MAX_ID_TABLE_SIZE; id_num++)
        safe_free((void**)&table->table[id_num].word);

    safe_free((void**)&table->table);
}

void scopes_dtor(Scope* scopes, size_t scopes_amt) 
{
    for (size_t scopes_cnt = 0; scopes_cnt < scopes_amt; scopes_cnt++)
    {
        IDTable id_table = scopes[scopes_cnt].ID_table;
        id_table_dtor(&id_table);
    }

    safe_free((void**)&scopes);
}

//----------------------tools-----------------------------------------

int count_words(const Word *const words, FILE* log_file) 
{
    DATA_CHECK_INT(words)

    size_t word_cnt = 0;
    DATA_CHECK_INT(words[word_cnt].word)
    while(words[word_cnt].word[0] != '\0')
    {
        word_cnt++;
        DATA_CHECK_INT(words[word_cnt].word)
    }  

    return word_cnt;
}

int add_id(Lexer *const lexer, IDType type, FILE* log_file)
{
    DATA_CHECK_INT(lexer)

    Scope* current_scope = &lexer->scopes[lexer->cur_scope];
    DATA_CHECK_INT(current_scope)
    IDTable* table = &current_scope->ID_table;
    DATA_CHECK_INT(table)
    
    ID_OVERFLOW_CHECK(table->len)

    DATA_CHECK_INT(table->table[table->len].word)
    safe_strncpy(table->table[table->len].word, lexer->tokens[lexer->word_cnt].word, MAX_WORD_LEN - 1, log_file);
    table->table[table->len].type = type;
    
    size_t added_index = table->len;
    table->len++;
    
    return added_index;
}

int check_id(const Lexer *const lexer, FILE* log_file) 
{
    DATA_CHECK_INT(lexer)

    Token* tokens = lexer->tokens;
    DATA_CHECK_INT(tokens)
    char* word = tokens[lexer->word_cnt].word;
    DATA_CHECK_INT(word)

    Scope* scopes = lexer->scopes;
    DATA_CHECK_INT(scopes)
    size_t cur_scope_num = lexer->cur_scope;

    for (int scope_cnt = cur_scope_num; scope_cnt >= 0; scope_cnt--)
    {
        IDTable table = scopes[scope_cnt].ID_table;
        size_t cur_id_amt = table.len;
        ID* id_table = table.table;
        DATA_CHECK_INT(id_table)

        for (int id_num = cur_id_amt; id_num >= 0; id_num--)
        {
            DATA_CHECK_INT(id_table[id_num].word)
            if (!strcmp(word, id_table[id_num].word))
                return id_num + scope_cnt * 10;              //id_num <= 10 --> можно однозначно определить обе цифры из того, что получилось 
        }
    }

    return ERR_VAL_SIZE_T;
}

int create_scope(Lexer *const lexer, FILE* log_file) 
{
    DATA_CHECK_INT(lexer)

    size_t new_scope_count = lexer->cur_scope + 1;
    
    Scope* new_scopes = (Scope*)realloc(lexer->scopes, (new_scope_count + 1) * sizeof(Scope));
    ALLOCATION_CHECK_INT(new_scopes)
    
    id_table_ctor(&new_scopes[new_scope_count].ID_table, log_file);
    new_scopes[new_scope_count].scope_ID = new_scope_count;
    
    lexer->scopes = new_scopes;
    lexer->cur_scope = new_scope_count;

    return 0;
}


int destroy_scope(Lexer* lexer, FILE* log_file)  
{
    DATA_CHECK_INT(lexer)
    DATA_CHECK_INT(lexer->scopes)
    DATA_CHECK_INT(lexer->cur_scope)

    size_t scope_to_remove = lexer->cur_scope;
    id_table_dtor(&(lexer->scopes[scope_to_remove].ID_table));

    lexer->cur_scope--;

    size_t new_size = lexer->cur_scope + 1;
    Scope* new_scopes = (Scope*)realloc(lexer->scopes, new_size * sizeof(Scope));
    if (new_scopes || new_size == 0)
        lexer->scopes = new_scopes;
    else
        ALLOCATION_CHECK_INT(new_scopes)

    return 0;
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
        if (!strcmp(word, key_words[word_num].word))
            return types[word_cnt];
            
    }
    return UNKNOWN;
}

int set_token(TokenType type, int value, size_t scope, Lexer* lexer, FILE* log_file)
{
    DATA_CHECK_INT(lexer)
    DATA_CHECK_INT(lexer->tokens)

    Token* token = &lexer->tokens[lexer->word_cnt];
    token->type  = type;
    token->value = value;
    token->scope = scope;

    return 0;
}

//----------------main funcs-------------------------------

int lex_analysis(Lexer* lexer, FILE* log_file)
{
    DATA_CHECK_INT(lexer)
    DATA_CHECK_INT(lexer->tokens)

    size_t words_amt = lexer->words_amt;

    for (size_t word_cnt = 0; word_cnt < words_amt; word_cnt++)
    {
        DATA_CHECK_INT(lexer->tokens[word_cnt].word)
        lexer->word_cnt = word_cnt;
        int func_output = process_current_token(lexer, log_file);
        INT_FUNC_CHECK_INT(func_output)

        word_cnt = lexer->word_cnt;
    } 

    return 0;
}

int process_current_token(Lexer* lexer, FILE* log_file)
{
    if (!process_num_token(lexer, log_file)) return 0;

    if (!process_declaration_token(lexer, log_file)) return 0;

    if (!process_keyword_token(lexer, log_file)) return 0;

    if (!process_id_token(lexer, log_file)) return 0;

    return ERR_VAL_INT;
}

int process_num_token(Lexer* lexer, FILE* log_file)
{
    DATA_CHECK_INT(lexer)
    DATA_CHECK_INT(lexer->tokens)
    
    Token token = lexer->tokens[lexer->word_cnt];
    char* word = token.word;
    DATA_CHECK_INT(word)

    if (isdigit(word[0]))
    {
        int value = atoi(word);
        int func_output = set_token(NUM, value, lexer->cur_scope, lexer, log_file);
        INT_FUNC_CHECK_INT(func_output)

        return 0;
    }

    return ERR_VAL_INT;
}

int process_declaration_token(Lexer* lexer, FILE* log_file)
{
    DATA_CHECK_INT(lexer)
    DATA_CHECK_INT(lexer->tokens)

    Token token = lexer->tokens[lexer->word_cnt];
    char* word = token.word;
    DATA_CHECK_INT(word)

    KeyWordType type = is_keyword(word, forming_set, FORMING_SET_AMT);
    if (type == UNKNOWN)    return ERR_VAL_INT;

    IDType id_type = VAR_ID;
    TokenType token_type= VAR;

    char* is_keyword_word = lexer->tokens[lexer->word_cnt + 2].word;
    DATA_CHECK_INT(is_keyword_word)

    int func_output = 0;
    if (!strcmp(is_keyword_word, KW(OPEN_BRACE)))
    {
        func_output = set_token(FUNC_SET, type, lexer->cur_scope, lexer, log_file);
        id_type = FUNC_ID;
        token_type = FUNC;
    }
    else
        func_output = set_token(VAR_SET, type, lexer->cur_scope, lexer, log_file);
    INT_FUNC_CHECK_INT(func_output)

    lexer->word_cnt++;

    word = lexer->tokens[lexer->word_cnt].word;
    DATA_CHECK_INT(word)

    size_t id_num = add_id(lexer, id_type, log_file);

    func_output = set_token(token_type, id_num, lexer->cur_scope, lexer, log_file);
    INT_FUNC_CHECK_INT(func_output)

    return 0;
}

int process_keyword_token(Lexer* lexer, FILE* log_file)
{   
    DATA_CHECK_INT(lexer)
    DATA_CHECK_INT(lexer->tokens)

    int func_output = 0;

    Token token = lexer->tokens[lexer->word_cnt];
    char* word = token.word;

    KeyWordType keyword_type = is_keyword(word, forming_new_scope, FORMING_NEW_SCOPE_AMT);

    if (keyword_type != UNKNOWN)   
    {
        func_output = create_scope(lexer, log_file);
        INT_FUNC_CHECK_INT(func_output)

        func_output = set_token(KEY_WORD, keyword_type, lexer->cur_scope, lexer, log_file);
        INT_FUNC_CHECK_INT(func_output)

        return 0;
    }

    keyword_type = is_keyword(word, delete_scope, DELETE_SCOPE_AMT);

    if (keyword_type != UNKNOWN)
    {
        func_output = destroy_scope(lexer, log_file);
        INT_FUNC_CHECK_INT(func_output);

        func_output = set_token(KEY_WORD, keyword_type, lexer->cur_scope, lexer, log_file);
        INT_FUNC_CHECK_INT(func_output)

        return 0;
    }
    else        
        keyword_type = is_keyword(word, key_word_rest, WORD_REST_AMT);

    if (keyword_type == UNKNOWN)  return ERR_VAL_INT;

    func_output = set_token(KEY_WORD, keyword_type, lexer->cur_scope, lexer, log_file);
    INT_FUNC_CHECK_INT(func_output)

    return 0;
}

int process_id_token(Lexer* lexer, FILE* log_file)
{
    DATA_CHECK_INT(lexer)
    int id_pos = check_id(lexer, log_file);
    INT_FUNC_CHECK_INT(id_pos)

    if (id_pos == ERR_VAL_SIZE_T) return false;

    size_t id_num = id_pos % 10;
    size_t scope = (id_pos - id_num) / 10;

    TokenType type = VAR;

    DATA_CHECK_INT(lexer->tokens)
    char* keyword_word = lexer->tokens[lexer->word_cnt + 1].word;
    DATA_CHECK_INT(keyword_word)
    if (!strcmp(keyword_word, KW(OPEN_BRACE)))
        type = FUNC;

    int func_output = set_token(type, id_num, scope, lexer, log_file);
    INT_FUNC_CHECK_INT(func_output)

    return 0;
}