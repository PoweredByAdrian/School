/**
 * -----------------------------------------------------------
 * Projekt: Implementace prekladace imperativniho jazyka IFJ21
 * Varianta: I (Binarni vyhledavaci strom)
 * Autori:		Gáfrik Patrik	xgafri00@stud.fit.vutbr.cz
	            Horváth Adrián	xhorva14@stud.fit.vutbr.cz
	            Kipila Maxim	xkipil00@stud.fit.vutbr.cz
	            Dohnal Ondřej	xdohna45@stud.fit.vutbr.cz

 * Datum: 20.11.2021
 *
 *
 * -----------------------------------------------------------
 */
#include "expression.h"



typedef enum
{ //<,>,=, , 
    S,R,E,N
}p_table_sign;

typedef enum
{
    i_CONC,
    i_PLUS_MINUS,
    i_DIV_MUL,
    i_HASHTAG,
    i_RELATION,
    i_LEFT_B,
    i_DATA,
    i_RIGHT_B,
    i_DOLLAR
}p_table_index;

typedef enum
{
    r_DOTS,
    r_PLUS,
    r_MINUS,
    r_MUL,
    r_DIV,
    r_DDIV,
    r_HASH,
    r_RELATION,
    r_BRACKET,
    r_ID
} p_rules;

typedef enum
{
    s_DOUBLEDOT,
    s_PLUS,
    s_MINUS,
    s_MUL,
    s_DIV,
    s_DDIV,
    s_HASHTAG,
    s_EQUAL,
    s_NEQUAL,
    s_LESS,
    s_GREAT,
    s_LESSEQ,
    s_GREATEQ,
    s_LEFT_BRACKET,
    s_RIGHT_BRACKET,
    s_DDOLLAR,
    s_ID,
    s_INT,
    s_STRING,
    s_EXPR,
    s_NUMBER
} p_table_symbol;

int p_table[9][9] = {
//   .. +- */ #  r  (  i  )  $
    {S, S, S, S, R, S, S, R, R}, //..
    {R, R, S, S, R, S, S, R, R}, //+-
    {R, R, R, S, R, S, S, R, R}, // */
    {R, R, R, N, R, S, S, R, R}, // #
    {S, S, S, S, S, S, S, R, R}, // r
    {S, S, S, S, S, S, S, E, N}, // (
    {R, R, R, N, R, N, N, R, R}, // i 
    {R, R, R, N, R, N, N, R, R}, // )
    {S, S, S, S, S, S, S, N, N}, // $
};

p_table_index get_p_table_i(p_table_symbol s)
{
    switch (s)
    {
    case s_DOUBLEDOT:
        return i_CONC;
    case s_MINUS:
    case s_PLUS:
        return i_PLUS_MINUS;
    case s_MUL:
    case s_DIV:
    case s_DDIV:
        return i_DIV_MUL;
    case s_HASHTAG:
        return i_HASHTAG;
    case s_EQUAL:
    case s_NEQUAL:
    case s_LESS:
    case s_GREAT:
    case s_LESSEQ:
    case s_GREATEQ:
        return i_RELATION;
    case s_LEFT_BRACKET:
        return i_LEFT_B;
    case s_ID:
    case s_INT:
    case s_STRING:
    case s_NUMBER:
        return i_DATA;
    case s_RIGHT_BRACKET:
        return i_RIGHT_B;
    
    default:
        return i_DOLLAR;
    }
}

p_table_symbol get_symbol(Token* token)
{
    switch (token->type)
    {
    case DOUBLEDOT:
        return s_DOUBLEDOT;
    case PLUS:
        return s_PLUS;
    case MINUS:
        return s_MINUS;
    case MUL:
        return s_MUL;
    case DIV_INT:
        return s_DIV;    
    case DIV_NUM:
        return s_DDIV;
    case HASH:
        return s_HASHTAG;
    case EQ:
        return s_EQUAL;
    case NEQ:
        return s_NEQUAL;
    case LT:
        return s_LESS;
    case GT:
        return s_GREAT;
    case LTEQ:
        return s_LESSEQ;
    case GTEQ:
        return s_GREATEQ;
    case LBRACKET:
        return s_LEFT_BRACKET;
    case ID:
        return s_ID;
    case INTEGER:
        return s_INT;
    case STRING:
        return s_STRING;
    case NUMBER:
        return s_NUMBER;
    case RBRACKET:
        return s_RIGHT_BRACKET;
    case EXPRESSION:
        return s_EXPR;

    default:
        return s_DDOLLAR;
    }
}

p_rules ruleCheck(tStack* stack, int count)
{
    switch (count)
    {
    case 1:
        switch (get_symbol(stack->top->token))
        {
        case s_ID:
        case s_INT:
        case s_NUMBER:
        case s_STRING:
            return r_ID;
        default:
            return ERR_SYNTAX;
        }
        break;
    case 2:
        if ((get_symbol(stack->top->next->token) == s_HASHTAG))
            return r_HASH;
        else
            return ERR_SYNTAX;
    case 3:
        switch ((get_symbol(stack->top->next->token)))
        {
        case s_PLUS:
            return r_PLUS;
        case s_MINUS:
            return r_MINUS;
        case s_MUL:
            return r_MUL;
        case s_DIV:
            return r_DIV;
        case s_DDIV:
            return r_DDIV;
        case s_DOUBLEDOT:
            return r_DOTS;
        case s_EXPR:
            return r_BRACKET;
        case s_EQUAL:
        case s_NEQUAL:
        case s_LESS:
        case s_LESSEQ:
        case s_GREAT:
        case s_GREATEQ:
            return r_RELATION;
        default:
            return ERR_SYNTAX;
        }
    default:
        return ERR_SYNTAX;
    }
}

void doRule(tStack* stack, p_rules rule, Token* E)
{
    switch (rule)
    {
    case r_DOTS:
    case r_PLUS:
    case r_MINUS:
    case r_MUL:
    case r_DIV:
    case r_DDIV:
    case r_RELATION:
    case r_BRACKET:
        for (int i = 0; i < 4; i++)
            (void)tStackPop(stack);
        tStackPush(stack, E);
        break;
    case r_HASH:
        for (int i = 0; i < 3; i++)
            (void)tStackPop(stack);
        tStackPush(stack, E);  
        break;
    case r_ID:
        (void)tStackPop(stack);
        (void)tStackPop(stack);
        tStackPush(stack, E);  
        break; 
    default:
        return;
    }
}
tStack* rotateStack(tStack* stack)
{
    tStack* rotated = malloc(sizeof(tStack));
    tStackInit(rotated);

    do
    {
        tStackPush(rotated, tStackPop(stack));
    }while(stack->top != NULL);
    return rotated;
    
}
int expre(tStack* input)
{
    p_rules rule;
    Token *dollar, *handle, *expr;
    expr = token_new(EXPRESSION, "E", true);
    dollar = token_new(DOLLAR, "$", true);
    handle = token_new(HANDLE, "<", true);

    tStack* exp_stack  = malloc(sizeof(tStack));
    tStackInit(exp_stack);
    tStackPush(exp_stack, dollar);
    tStackPush(input, dollar);

    tStack* input_stack = rotateStack(input);

    tStackItem* next_item = malloc(sizeof(struct tokenStackItem)); 
    if(next_item == NULL)
        return ERR_INTERNAL;

    do
    {

        p_table_index stack_index;

        int cnt = 0;
        next_item = exp_stack->top;

        if (exp_stack->top->token->type == EXPRESSION)
        {
            stack_index = get_p_table_i(get_symbol(exp_stack->top->next->token));
            cnt++;
        }
        else
            stack_index = get_p_table_i(get_symbol(exp_stack->top->token));
        p_table_index input_index = get_p_table_i(get_symbol(input_stack->top->token));

        switch (p_table[stack_index][input_index])
        { 
        case E:
            tStackPush(exp_stack, tStackPop(input_stack));
            break;
        case S:
            tStackPush(exp_stack, handle);
            tStackPush(exp_stack, tStackPop(input_stack));
            break;
        case R:

            do
            {
                next_item = next_item->next;
                cnt++;
                if(cnt > 3)
                {
                    free(next_item);
                    return ERR_SYNTAX;
                };
            }while(next_item->token->type != HANDLE);
            
            rule = ruleCheck(exp_stack, cnt);
            if (rule == ERR_SYNTAX)
            {
                free(next_item);
                return ERR_SYNTAX;
            }
            doRule(exp_stack, rule, expr);
            break;
        default:
            return ERR_SYNTAX; 
        }



        //ukoncenie while
        if((exp_stack->top->next->token->type == DOLLAR) && (input_stack->top->token->type == DOLLAR))
            break;
        
        
    }while (true);
    



    tStackPop(input_stack);
    tStackDestroy(exp_stack);
    free(expr);
    free(dollar);
    free(handle);


    free(next_item);
    return SUCCESS;
}