#include "../include/syn_analsys.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//CODE    ::= {EXPR}* + 'end'
Node* syn_analysis(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    Node* root = get_expr(tokens, pointer, log_file);

    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != END)
    {
        Node* new_expr = get_expr(tokens, pointer, log_file);

        root = make_node(KW_NODE, SEP, root, new_expr, log_file);              //в тексте не будет разделителя, это просто нода такая
        //if (!(tokens + *pointer))
        //    ERROR -> no END
    }

    return root;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//EXPR    ::= {LAYER}* | SERV {',' SERV}*
Node* get_expr(const Token *const tokens, size_t *const pointer, FILE *const log_file)          //TODO: можно сделать меньше, если сделать функцию и передавать в нее тип функции анализа для списка
{

    ListNode* expr_list = list_ctor();

    bool is_layer = check_layer(tokens, pointer, log_file);
    while (is_layer)
    {
        Node* layer = get_layer(tokens, pointer, log_file);
        list_push(expr_list, layer);
        is_layer = check_layer(tokens, pointer, log_file);
    }
    if (expr_list->list_size) 
    {
        Node* expr = make_list_node(LAYER_LIST_NODE, expr_list, 0, log_file);
        return expr;
    }   

    bool is_serv = check_serv(tokens, pointer, log_file);
    while (is_serv)
    {
        Node* serv = get_serv(tokens, pointer, log_file);
        list_push(expr_list, serv);
        if (tokens[*pointer].type == KEY_WORD && tokens[*pointer].value == SEP)
            (*pointer)++;
        //else
        //    ERROR -> lost SEP
        is_serv = check_serv(tokens, pointer, log_file);
    }
    if (expr_list->list_size) 
    {
        Node* expr = make_list_node(LAYER_LIST_NODE, expr_list, 0, log_file);
        return expr;
    }   

    // ERROR what did you write?
    return nullptr;
}

bool check_serv(const Token *const tokens, size_t *const pointer, FILE *const log_file)
{             
    bool is_correct_type = is_in_massive((void*)serv_pr, sizeof(TokenType), FORMING_SERV_AMT, (void*)tokens[*pointer].type);                                                                      
    return is_correct_type;
}

bool check_layer(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    bool is_correct_type = is_in_massive((void*)layer_pr, sizeof(KeyWordType), FORMING_LAYER_AMT, (void*)tokens[*pointer].value);
    if (is_correct_type)
    {
        if ((tokens[*pointer].value == VOID || tokens[*pointer].value == INT) && tokens[*pointer].type == FUNC_SET)
            return true;
        else if (tokens[*pointer].type == KEY_WORD)
            return true;
    }

    return false;
}

//-----LIST THINGS-------
ListNode* list_ctor()
{
    ListNode* list = (ListNode*)calloc(1, sizeof(ListNode));

    return list;
}

void list_push(ListNode* list, Node* new_node)
{
    list->list = (Node**)realloc(list->list, (list->list_size + 1) * sizeof(Node*));
    list->list[list->list_size++] = new_node;
}

Node* make_list_node(NodeType type, ListNode* list, int val, FILE* log_file)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    new_node->val = val;
    new_node->type = type;
    new_node->list = list;

    return new_node;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//SERV ::= VARSET | EQAL | FUNC

Node* get_serv(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    Node* val = nullptr;
    switch(tokens[*pointer].type)
    {
        case VAR_SET:
            val = get_varset(tokens, pointer, log_file); break;
        case VAR:
            val = get_equal(tokens, pointer, log_file); break;
        default:
            val = get_func(tokens, pointer, log_file);
    }
    return val;
}

//LAYER   ::= CYCLE | COND | FUNCSET
Node* get_layer(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    Node* val = nullptr;
    switch(tokens[*pointer].value)
    {
        case FOR:
        case WHILE:
            val = get_cycle(tokens, pointer, log_file); break;
        case IF:
        case ELSE_IF:
        case ELSE:
            val = get_cond(tokens, pointer, log_file); break;
        default:
            val = get_funcset(tokens, pointer, log_file);
    }
    return val;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------

//VARSET  ::= TYPE + VARID [+ '=' + VARID | SUM | FUNC]
Node* get_varset(const Token *const tokens, size_t *const pointer, FILE *const log_file)  
{
    (*pointer)++;                                                                           //тип хранится в таблице
    int var_id = tokens[*pointer].value;
    Node* varset_node = make_node(VARSET_NODE, var_id, NULL, NULL, log_file);
    (*pointer)++;
    if (tokens[*pointer].type == KEY_WORD && tokens[*pointer].value == EQUAL)
    {
        (*pointer)++;
        Node* right = get_right_equal_part(tokens, pointer, log_file);
        return make_node(KW_NODE, EQUAL, varset_node, right, log_file);
    }

    return varset_node;
}

//EQUAL   ::= VARID + '=' + VARID | SUM | FUNC
Node* get_equal(const Token *const tokens, size_t *const pointer, FILE *const log_file)  
{
    Node* left = get_id(tokens, pointer, log_file);
    (*pointer)++;
    if (tokens[*pointer].type == KEY_WORD && tokens[*pointer].value == EQUAL)
    {
        (*pointer)++;
        Node* right = get_right_equal_part(tokens, pointer, log_file);
        return make_node(KW_NODE, EQUAL, left, right, log_file);
    }
    //else
    //    ERROR

    return nullptr;
}

Node* get_right_equal_part(const Token *const tokens, size_t *const pointer, FILE *const log_file)  
{
    Node* right = nullptr;
    (*pointer)++;
    switch(tokens[*pointer].type)
    {
        case VAR:
            right = get_id(tokens, pointer, log_file); break;
        case FUNC:
            right = get_func(tokens, pointer, log_file); break;
        case KEY_WORD:                                                                              //!!!проверка
            right = get_sum(tokens, pointer, log_file); break;
        default:
            //ERROR
    }
    return right;
}

//FUNC ::= ID + '(' + (VARID | SUM) {',' (VARID | SUM)}* + ')'
Node* get_func(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    int func_id = tokens[*pointer].value;
    (*pointer)++;
    ListNode* param_list = get_func_params(tokens, pointer, log_file);
    Node* func = make_list_node(FUNC_NODE, param_list, func_id, log_file);

    return func;
}

ListNode* get_func_params(const Token* tokens, size_t* pointer, FILE* log_file) 
{
    assert(tokens && pointer && log_file);
    DATA_CHECK_PTR(tokens + *pointer);

    //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_BRACE)
        //ERROR -> brace
    //else
        (*pointer)++;       //пропускаем первую скобку

    ListNode* param_list = list_ctor();

    while (tokens[*pointer].type != KEY_WORD || tokens[*pointer].value != CLOSE_BRACE) 
    {
        Node* new_param = nullptr;
        switch (tokens[*pointer].type)
        {
            case VAR:
                new_param = get_id(tokens, pointer, log_file);  break;           //! надо ссылаться на айди из таблицы. в таблице должны быть указаны слой, имя и ТИП
            default:
                new_param = get_sum(tokens, pointer, log_file);
        }
        list_push(param_list, new_param);

        if (tokens[*pointer].type == SEP)
            (*pointer)++;
        //else
        //    ERROR -> no SEP
    }

    (*pointer)++; // Пропускаем CLOSE_BRACE
    return param_list;
}

//FUNCSET ::= TYPE + ID + '(' + {VARSET}+ + ')' + '{' + [EXPR]+ + '}'
Node* get_funcset(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    (*pointer)++;
    //if (tokens[*pointer].type != FUNC)
    //    ERROR
    int func_id = tokens[*pointer].value;
    (*pointer)++;


    //TODO: нужно сделать два листа!! один с varsets а второй с expessions
    Node* varset = get_varset(tokens, pointer, log_file);
    Node* body = get_func_body(tokens, pointer, log_file);
    Node* parameters = make_node(PARAM_NODE, 0, type, varset, log_file);

    Node* func = make_node(FUNCSET_NODE, func_id, parameters, body);

    return func;
}

Node* get_func_body(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_CURLY_BRACE)
    //    ERROR 
    (*pointer)++;
    Node* expr = nullptr;
    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_CURLY_BRACE)
    {
        //GET EXPRESSIONS

        //if (!(tokens + *pointer))
        //    ERROR
    }
    (*pointer)++;

    return expr;
}


Node* get_sum (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = get_num(tokens, pointer, log_file);
    PTR_FUNC_CHECK_PTR(val)

    if (tokens[*pointer].type != KEY_WORD)  return val;
    bool is_sec_pr = is_in_massive((void*)sec_pr, sizeof(KeyWordType), SECOND_PR_AMT, (void*)tokens[*pointer].value);

    while (is_sec_pr)
    {
        int op = tokens[*pointer].value;
        (*pointer)++;
        Node* right = get_mul(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(right)
        val = make_node(KW_NODE, op, val, right, log_file);
        PTR_FUNC_CHECK_PTR(val)

        if (tokens[*pointer].type != KEY_WORD)  return val;
        is_sec_pr = is_in_massive((void*)sec_pr, sizeof(KeyWordType), SECOND_PR_AMT, (void*)tokens[*pointer].value);
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
    bool is_first_pr = is_in_massive((void*)first_pr, sizeof(KeyWordType),  FIRST_PR_AMT, (void*)tokens[*pointer].value);

    while (is_first_pr)
    {
        int op = tokens[*pointer].value;
        (*pointer)++;
        Node* right = get_brace(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(right)
        val = make_node(KW_NODE, op, val, right, log_file);
        PTR_FUNC_CHECK_PTR(val)

        if (tokens[*pointer].type != KEY_WORD)  return val;
        is_first_pr = is_in_massive((void*)first_pr, sizeof(KeyWordType), FIRST_PR_AMT, (void*)tokens[*pointer].value);
    }

    return val;
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
        //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_BRACE)
        //    ERROR
        //else 
            (*pointer)++;
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
    //else
    //    ERROR

    return val;
}

//TODO надо сделать проверку, что переменная была объявлена раньше, те не ошибочный ли номер в таблице -> в токене
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

Node* make_node(NodeType node_type, int val, Node* left, Node* right, FILE* log_file)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->type = node_type;
    new_node->val = val;
    new_node->left = left;
    new_node->right = right;

    return new_node;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool is_in_massive(const void* arr, size_t elem_size, size_t arr_size, const void* elem) 
{
    assert(arr);
    assert(elem);
    assert(elem_size);
    
    const char* byte_arr = (const char*)arr;
    for (size_t i = 0; i < arr_size; i++) 
    {
        if (memcmp(byte_arr + i * elem_size, elem, elem_size) == 0)
            return true;
    }
    return false;
}