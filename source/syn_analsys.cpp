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

        //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != SEP)
        //    ERROR
        val = make_node(KW_NODE, SEP, val, new_val, log_file);
        //if (!(tokens + *pointer))
        //    ERROR
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
    if (is_serv)
    {
        val = get_serv(tokens, pointer, log_file);
        PTR_FUNC_CHECK_PTR(val)
    }

    //ERROR
}

//SERV    ::= VARSET | EQAL | FUNC
bool check_serv(const Token *const tokens, size_t *const pointer, FILE *const log_file)    //TODO сюда надо проход по массиву типов токенов -> нужно сделать функцию проверки на void или типа того
{             
    bool is_correct_type = is_in_massive((void*)serv_pr, sizeof(TokenType), FORMING_SERV_AMT, (void*)tokens[*pointer].type);                                                                      
    
    if (is_correct_type)
    {
        //if (tokens[*pointer].type == VAR_SET && tokens[*pointer].value == VOID)         //достаточно обработать varset, остальные не пересекаются
        //    ERROR 
        return true;
    }

    return false;
}

Node* get_serv(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = nullptr;
    if (tokens[*pointer].type == VAR_SET)
        val = get_varset(tokens, pointer, log_file);
    else if (tokens[*pointer].type == VAR)
        val = get_equal(tokens, pointer, log_file);
    else
        val = get_func(tokens, pointer, log_file);

    return val;
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

//LAYER   ::= CYCLE | COND | FUNCSET
Node* get_layer(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* val = nullptr;
    if (tokens[*pointer].value == FOR || tokens[*pointer].value == WHILE)
        val = get_cycle(tokens, pointer, log_file);
    else if (tokens[*pointer].value == IF || tokens[*pointer].value == ELSE IF || tokens[*pointer].value == ELSE)
        val = get_cond(tokens, pointer, log_file);
    else 
        val = get_funcset(tokens, pointer, log_file);

    PTR_FUNC_CHECK_PTR(val)

    return val;
}


//VARSET  ::= TYPE + VARID [+ '=' + VARID | SUM | FUNC]                                             //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

//Node* get_func_params(const Token *const tokens, size_t *const pointer, FILE *const log_file)           //TODO сделать меньше
//{
//    assert(tokens);
//    assert(pointer);
//    assert(log_file);
//    DATA_CHECK_PTR(tokens + *pointer)
//
//    //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_BRACE)
//    //    ERROR 
//    (*pointer)++;
//    //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != SEP)
//    //    ERROR 
//    (*pointer)++;
//
//    Node* param = get_id(tokens, pointer, log_file);                 //! надо ссылаться на айди из таблицы. в таблице должны быть указаны слой, имя и ТИП
//        
//    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_BRACE)
//    {    
//        Node* temp_node = param->right;
//        if (temp_node)
//        {
//            while (temp_node)
//                temp_node = temp_node->right;
//            param = temp_node;
//        }
//
//        temp_node = get_id(tokens, pointer, log_file);;
//        //if (!(tokens + *pointer))
//        //    ERROR
//    }
//    (*pointer)++;
//
//    return param;
//}

Node* get_func_params(const Token* tokens, size_t* pointer, FILE* log_file) 
{
    assert(tokens && pointer && log_file);
    DATA_CHECK_PTR(tokens + *pointer);

    (*pointer)++; 
    (*pointer)++;

    Node* head = nullptr;
    Node* tail = nullptr;

    while (tokens[*pointer].type != KEY_WORD || tokens[*pointer].value != CLOSE_BRACE) 
    {
        Node* new_param = get_id(tokens, pointer, log_file);

        if (!head) 
        {
            head = new_param;
            tail = head;
        } else 
        {
            tail->right = new_param;
            tail = tail->right;
        }

        if (tokens[*pointer].type == SEP)
            (*pointer)++;
    }

    (*pointer)++; // Пропускаем CLOSE_BRACE
    return head;
}

Node* get_funcset(const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);
    DATA_CHECK_PTR(tokens + *pointer)

    Node* type = get_type(tokens, pointer, log_file);
    //if (tokens[*pointer].type != FUNC)
    //    ERROR
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

Node* get_varsets(const Token *const tokens, size_t *const pointer, FILE *const log_file)  
{
    //if (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != OPEN_BRACE)
    //    ERROR 
    (*pointer)++;
    Node* param = nullptr;                                                                    //TODO TODO TODO!!! поправить по примеру func_params
    Node* new_param = nullptr;
    while (tokens[*pointer].type != KEY_WORD && tokens[*pointer].value != CLOSE_BRACE)
    {    
        //if (tokens[*pointer].type != VAR_SET)
        //    ERROR 
        //if (tokens[*pointer].value == VOID)
        //    ERROR 
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

        //if (!(tokens + *pointer))
        //    ERROR
    }
    (*pointer)++;

    return param;
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

//bool is_in_massive(void* array, size_t type_size, size_t types_amt, void* the_word)
//{
//    char* base_ptr = (char*)array;
//    char* word = (char*)the_word;
//
//    for (size_t ind = 0; ind < types_amt; ind++)
//    {
//        size_t coinc_bits = 0;
//        for (size_t bit = 0; bit < type_size; bit++)
//        {
//            if (base_ptr[ind * type_size + bit] == word[bit])
//                coinc_bits++;
//        }
//        
//        if (coinc_bits == type_size)
//            return true;
//    }
//    return false;
//}


//memcmp эффективнее
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