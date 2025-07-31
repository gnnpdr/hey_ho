#include "../include/syn_analsys.h"

Node* syn_analysis(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* root = get_code(tokens, pointer, log_file);                               //? слишком маленькая функция тогда
    //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != END)
    //    ERROR 

    return root;
}

Node* get_code(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = get_expr(tokens, pointer, log_file);                                //TODO проверить пустой код

    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != END)
    {
        Node* new_val = get_expr(tokens, pointer, log_file);

        if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != SEP)
            ERROR
        val = make_node(KW_NODE, SEP, val, new_val, log_file);
        if (!(tokens + *pointer))
            ERROR
    }

    return val;
}

//EXPR    ::= LAYER | SERV {',' SERV}*
Node* get_expr(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = nullptr;
    //check layer
    bool is_layer = check_layer(tokens, pointer, log_file);
    if (is_layer)
    {
        val = get_layer(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(val)
    }
    //check serv
    bool is_sert = check_serv(tokens, pointer, log_file);
    if (is_layer)
    {
        val = get_layer(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(val)
    }

    ERROR
}

//LAYER   ::= CYCLE | COND | FUNCSET
bool is_layer(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    if (is_the_op(layer_pr, FORMING_LAYER_AMT, tokens[*pointer].value))
    {
        if (tokens[*pointer].value == VOID && tokens[*pointer].type == FUNC_SET)                //TODO сюда еще инт
            return true;
        else if (tokens[*pointer].type == KEY_WORD)
            return true;
    }

    return false;
}

//SERV    ::= VARSET | EQAL | FUNC
bool is_serv(const Token *const tokens, size_t *const pointer, FILE *const log_file)    //TODO сюда надо проход по массиву типов токенов -> нужно сделать функцию проверки на void или типа того
{                                                                                   
    if (tokens[*pointer].type == VAR_SET )
        return true;
    if (tokens[*pointer].type == VAR)
}

Node* get_func(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    int func_id = tokens[*pointer].value;
    (*pointer)++;

    Node* parameters = get_func_params(tokens, pointer, log_file);

    Node* func = make_node(FUNCSET_NODE, func_id, NULL, parameters);

    return func;
}

Node* get_func_params(const Token *const tokens, size_t *const pointer, FILE *const log_file)           //TODO сделать меньше
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_BRACE)
        ERROR 
    (*pointer)++;
    if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != SEP)
        ERROR 
    (*pointer)++;

    Node* param = nullptr;
    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_BRACE)
    {    
        Node* new_param = get_id(tokens, pointer, log_file);                 //! надо ссылаться на айди из таблицы. в таблице должны быть указаны слой, имя и ТИП
        if (!param)
            param = new_param;
        else
        {
            Node* temp_node = param->right;
            while (temp_node)
                temp_node = temp_node->right;
            param = temp_node;
        }

        if (!(tokens + *pointer))
            ERROR
    }
    (*pointer)++;

    return param;
}

Node* get_funcset(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* type = get_type(tokens, pointer, log_file);
    if (tokens[*pointer].type != FUNC)
        ERROR
    int func_id = tokens[*pointer].value;
    (*pointer)++;

    Node* varset = get_varsets(tokens, pointer, log_file);
    Node* body = get_func_body(tokens, pointer, log_file);
    Node* parameters = make_node(PARAM_NODE, 0, type, varset, log_file);

    Node* func = make_node(FUNCSET_NODE, func_id, parameters, body);

    return func;
}

Node* get_func_body(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_CURLY_BRACE)
        ERROR 
    (*pointer)++;
    Node* expr = nullptr;
    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_CURLY_BRACE)
    {
        //GET EXPRESSIONS

        if (!(tokens + *pointer))
            ERROR
    }
    (*pointer)++;

    return expr;
}

Node* get_varsets(const Token *const tokens, size_t *const pointer, FILE *const log_file)    //TODO сделать меньше
{
    if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_BRACE)
        ERROR 
    (*pointer)++;
    Node* varset = nullptr;
    Node* new_varset = nullptr;
    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_BRACE)
    {    
        if (tokens[*pointer].type != VAR_SET)
            ERROR 
        if (tokens[*pointer].value == VOID)
            ERROR 
        (*pointer)++;

        Node* new_varset = get_id(tokens, pointer, log_file);                 //! надо ссылаться на айди из таблицы. в таблице должны быть указаны слой, имя и !!!!ТИП!!!!
        if (!param)
            param = new_param;
        else
        {
            Node* temp_node = param->right;
            while (temp_node)
                temp_node = temp_node->right;
            param = temp_node;
        }

        if (!(tokens + *pointer))
            ERROR
    }
    (*pointer)++;

    return varset;
}

Node* get_sum (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = get_nul(tokens, pointer, log_file);
    PTR_FUNC_CHECK_PTR(val)

    if (tokens[*pointer].type != KEY_WORD)  return val;
    bool is_sec_pr = is_the_op(sec_pr, SECOND_PR_AMT, tokens[*pointer].value);

    while (is_sec_pr)
    {
        int op = tokens[*pointer].value;
        (*pointer)++;
        Node* right = get_mul(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(right)
        val = make_node(KW_NODE, op, val, right, log_file);
        PTR_FUNC_CHECK_PTR(val)

        if (tokens[*pointer].type != KEY_WORD)  return val;
        is_sec_pr = is_the_op(sec_pr, SECOND_PR_AMT, tokens[*pointer].value);
    }

    return val;
}

Node* get_mul (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = get_brace(tokens, pointer, log_file);
    PTR_FUNC_CHECK_PTR(val)

    if (tokens[*pointer].type != KEY_WORD)  return val;
    bool is_first_pr = is_the_op(first_pr, FIRST_PR_AMT, tokens[*pointer].value);

    while (is_first_pr)
    {
        int op = tokens[*pointer].value;
        (*pointer)++;
        Node* right = get_brace(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(right)
        val = make_node(KW_NODE, op, val, right, log_file);
        PTR_FUNC_CHECK_PTR(val)

        if (tokens[*pointer].type != KEY_WORD)  return val;
        bool is_first_pr = is_the_op(first_pr, FIRST_PR_AMT, tokens[*pointer].value);
    }

    return val;
}

bool is_the_op(KeyWordType* types, size_t types_amt, int the_key_word)
{
    for (size_t kw = 0; kw < types_amt; kw++)
    {
        if (types[kw] == the_key_word)
            return true;
    }
    return false;
}

Node* get_brace (const Token *const tokens, size_t *const pointer, FILE *const log_file)     //TODO сделать меньше
{
    assert(tokens);
    assert(pointer);
    assert(log_file);

    Node* val = nullptr;

    DATA_CHECK_PTR(tokens + *pointer)
    if (tokens[*pointer].type == KEY_WORD && tokens[*pointer].value == OPEN_BRACE)
    {
        (*pointer)++;
        val = get_sum(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(val)

        DATA_CHECK_PTR(tokens + *pointer)
        if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_BRACE)
            ERROR
        else 
            (*pointer++);
    }
    else if (tokens[*pointer].type == NUM)
    {
        val = get_num(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(val)
    }
    else if (tokens[*pointer].type == VAR)
    {
        val = get_id(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(val)
    }
    else
        ERROR

    return val;
}

Node* get_type (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);

    DATA_CHECK_PTR(tokens + *pointer)
    int value = tokens[*pointer].value;
    (*pointer)++;

    Node* val = make_node(TYPE_NODE, value, nullptr, nullptr, log_file);
    PTR_FUNC_CHECK_PTR(val)

    return val;
}

Node* get_id (const Token *const tokens, size_t *const pointer, FILE *const log_file)
{
    assert(tokens);
    assert(pointer);
    assert(log_file);

    DATA_CHECK_PTR(tokens + *pointer)
    Node* val = make_node(ID_NODE, tokens[*pointer].value, nullptr, nullptr, log_file);
    PTR_FUNC_CHECK_PTR(val)

    (*pointer)++;

    return val;
}

Node* get_num (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);

    DATA_CHECK_PTR(tokens + *pointer)
    int value = tokens[*pointer].value;
    (*pointer)++;

    Node* val = make_node(NUM_NODE, value, nullptr, nullptr, log_file);
    PTR_FUNC_CHECK_PTR(val)

    return val;
}