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

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdbool.h>
#include "type.h"

/**
 * A simple token
 */
struct token_t {
    TTokenType type;        // Identifier (0 is for an invalid token)
    char *value;        // Token's value
    bool nonterm;
};

typedef struct token_t Token;
Token* token_new(TTokenType type, char *value, const bool nonterm);
void token_delete(Token *token);
Token* token_copy(const Token *token);
unsigned token_get_type(const Token *token);
char* token_get_value(const Token *token);
bool token_get_nonterm(const Token *token);
bool token_set_type(Token *token, TTokenType type);
bool token_set_value(Token *token, char *value);
bool token_set_nonterm(Token *token, bool nonterm);
void token_print(const Token *token);

#endif
