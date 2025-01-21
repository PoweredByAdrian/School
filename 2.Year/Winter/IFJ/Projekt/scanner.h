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
#ifndef IFJ20_SCANNER_H
#define IFJ20_SCANNER_H

#include "type.h"
#include "str.h"
#include "type.h"
#include "error.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BETWEEN(x, x_min, x_max) ((x) >= (x_min) && (x) <= (x_max))

typedef enum {
    STATE_START,
    STATE_DIV_NUM,
    STATE_DIV_INT,
    STATE_ASSIGN,
    STATE_EQ,
    STATE_LBRACKET,
    STATE_RBRACKET,
    STATE_STRING_START,
    STATE_STRING_ESCAPE,
    STATE_STRING_ESCAPE_1,
    STATE_STRING_ESCAPE_2,
    STATE_STRING_END,
    STATE_MINUS,
    STATE_COMMENT,
    STATE_MUL,
    STATE_PLUS,
    STATE_DOT,
    STATE_COMMA,
    STATE_DOUBLEDOT,
    STATE_TILDE,
    STATE_NEQ,
    STATE_NUMERIC,
    STATE_DECIMAL,
    STATE_DECIMAL_VALUE,
    STATE_EXP,
    STATE_EXP_SIGN,
    STATE_EXP_VALUE,
    STATE_HASH,
    STATE_LT,
    STATE_LTEQ,
    STATE_GT,
    STATE_GTEQ,
    STATE_KW_OR_ID,
    STATE_BLOCK_COMMENT_START,
    STATE_BLOCK_COMMENT,
    STATE_BLOCK_COMMENT_END,
    STATE_COLON


} ScannerState;

Token *getNextToken();

void freeTokenData(Token* token);

#endif