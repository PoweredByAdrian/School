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

#ifndef IFJ20_TYPE_H
#define IFJ20_TYPE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "str.h"







typedef enum TokenType{
    EMPTY,
    kwDO,
    kwELSE,
    kwEND,
    kwFUNCTION,
    kwGLOBAL,
    kwIF,
    kwINTEGER,
    kwLOCAL,
    kwNIL,
    kwNUMBER,
    kwREQUIRE,
    kwRETURN,
    kwSTRING,
    kwTHEN,
    kwWHILE,
    kwWRITE,
    kwREADS,
    kwREADI,
    kwREADN,
    kwTOINTEGER,
    kwSUBSTR,
    kwORD,
    kwCHR,



    COLON,
    LT,
    GT,
    LTEQ,
    GTEQ,
    EQ,
    NEQ,
    DOUBLEDOT,
    ASSIGN,
    PLUS,
    MINUS,
    MUL,
    DIV_NUM,
    DIV_INT,
    COMMENT,
    HASH,
    LBRACKET,
    RBRACKET,
    COMMA,
    EOFI,
    DOLLAR,
    EXPRESSION,
    HANDLE,
    tTerm,
    tExpr,
    tExpr2,
    IFJ21,


    NIL,
    INTEGER,
    NUMBER,
    STRING,
    ID,



    PROG,
    PROLOG,
    RETURNTYPE,
    PARAMSTYPE,
    PARAMSTYPEN,
    STATEMENT,
    PARAMS,
    PARAMSN,
    DEFVALUE,
    FUNC,
    IDLIST,
    RETURNEXP,
    EXPLIST,
    BUILTIN,
    ARG,
    VALUE,
    ARGN,
    TYPE,
    EQL
} TTokenType;




#endif //IFJ_TYPE_H
