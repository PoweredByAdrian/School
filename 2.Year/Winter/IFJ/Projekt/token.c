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
#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type.h"

Token* token_new(const TTokenType type, char *value, const bool nonterm) {
        Token *token = malloc(sizeof (*token));
        if (token != NULL) {
            if (value) {
              size_t len = strlen(value);
              token->value = malloc(sizeof (*token->value) * (len + 1));
              strcpy(token->value, value);
              //token->value = value;
            }
            else{
                return NULL;
            }
            //size_t len = strlen(value);
            //token->value = malloc(sizeof (*token->value) * (len + 1));
            //strcpy(token->value, value);
            token->type = type;
            token->nonterm = nonterm;
        }
        return token;
}


void token_delete(Token *token) {
    if (token != 0) {
        free (token->value);
        free(token);
    }
}


Token* token_copy(const Token *token) {
    if (token != 0) {
        Token *copy = malloc(sizeof (*copy));
        if (copy != 0) {
            size_t len = strlen(token->value);
            copy->value = malloc(sizeof (*copy->value) * (len + 1));
            if (copy->value != 0) {
                strcpy(copy->value, token->value);
                copy->type = token->type;
                copy->nonterm = token->nonterm;
            } else {
                free(copy);
                return 0;
            }
            return copy;
        }
    }
    return 0;
}


TTokenType token_get_type(const Token *token) {
    return token != 0 ? token->type : EMPTY;
}


char* token_get_value(const Token *token) {
    return token != 0 ? token->value : 0;
}



bool token_get_nonterm(const Token *token) {
    return token != 0 ? token->nonterm : 0;
}


bool token_set_type(Token *token, TTokenType type) {
    if (token != 0) {
        token->type = type;
        return true;
    }
    return false;
}


bool token_set_value(Token *token, char *value) {
    if (token != 0 && value != 0) {
        free(token->value);
        size_t len = strlen(value);
        token->value = malloc(sizeof(*token->value) * (len + 1));
        if (token->value) {
            strcpy(token->value, value);
            return true;
        }
    }
    return false;
}

bool token_set_nonterm(Token *token, bool nonterm) {
    if (token != 0) {
        token->nonterm = nonterm;
        return true;
    }
    return false;
}


void token_print(const Token *token) {
    if (token != 0) {
        printf("\nToken\nTYPE\t%d\nValue\t%s\nNonterm\t%d\n",
                token->type, token->value, token->nonterm);
    } else {
        printf("[ NULL ]\n");
    }
}
