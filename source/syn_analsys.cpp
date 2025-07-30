#include "../include/syn_analsys.h"

Node* get_sum (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{

}


Node* get_mul (const Token *const tokens, size_t *const pointer, FILE *const log_file) 
{
    assert(tokens);
    assert(pointer);
    assert(log_file);

    DATA_CHECK_PTR(tokens + *pointer)

    Node* left = get_brace(tokens, pointer, log_file);
    PTR_FUNC_CHECK_PTR(left)

    

}

//BR {'*''/' BR}*

Node* get_brace (const Token *const tokens, size_t *const pointer, FILE *const log_file)
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

/*
mul
Node* get_first_pr(Token *const tokens, Id *const ids, size_t *const pointer, ErrList *const list)
{
    REC_ASSERT
    
    Node* val = nullptr;

    bool is_end = false;

    val = get_brace(tokens, ids, pointer, list);
    RETURN_PTR

    bool is_first_pr = false;

    for (size_t op = 0; op < FIRST_PR_AMT; op++)
    {
        if (tokens[*pointer].value == first_pr[op].num && tokens[*pointer].type == OP)
            is_first_pr = true;
    }
    while (is_first_pr)
    {
        size_t the_op = tokens[*pointer].value;
        (*pointer)++;
        Node* val2 = get_brace(tokens, ids, pointer, list);
        RETURN_PTR
        val = make_node(OP, the_op, val, val2, list);
        RETURN_PTR

        is_first_pr = false;

        for (size_t op = 0; op < FIRST_PR_AMT; op++)
        {
            if (tokens[*pointer].value == first_pr[op].num && tokens[*pointer].type == OP)
                is_first_pr = true;
        }
    }

    return val;
}


sum
Node* get_sec_pr(Token *const tokens, Id *const ids, size_t *const pointer, ErrList *const list)
{
    REC_ASSERT
    
    Node* val = nullptr;

    val = get_first_pr(tokens, ids, pointer, list);
    RETURN_PTR

    bool is_sec_pr = false;
    for (size_t op = 0; op < SEC_PR_AMT; op++)
    {
        if (tokens[*pointer].value == sec_pr[op].num && tokens[*pointer].type == OP)
            is_sec_pr = true;
    }
    while (is_sec_pr)
    {
        size_t the_op = tokens[*pointer].value;
        (*pointer)++;
        Node* val2 = get_first_pr(tokens, ids, pointer, list);
        RETURN_PTR
        val = make_node(OP, the_op, val, val2, list);
        RETURN_PTR

        is_sec_pr = false;
        for (size_t op = 0; op < SEC_PR_AMT; op++)
        {
            if (tokens[*pointer].value == sec_pr[op].num && tokens[*pointer].type == OP)
                is_sec_pr = true;
        }
    }

    return val;
}*/