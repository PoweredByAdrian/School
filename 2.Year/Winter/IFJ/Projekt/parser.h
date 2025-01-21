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
#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "error.h"
#include "symtable.h"
#include "type.h"

#define MAIN "main"
// #define IFJ21 "ifj21"

typedef struct {                         
    TTokenType* data;
    int size;
    int top;
} Stack;

void stInit(int size);
void stPush(TTokenType T);
TTokenType stPop();
TTokenType stTop();

typedef struct {
    Token** data;
    int size;
    int count;
} List;

void listInit(int size);
void listAdd(Token *T);
void listClear();

typedef struct {
    symtable_bst_node* symTable;
    int size;
    int top;
} SymStack;

void symStInit(int size);
void symStPush(symtable_bst_node table);
symtable_bst_node symStPop();
symtable_data symTableSearchAll(symtable_bst_node* tree, char* key);

bool isTerm(TTokenType token);
bool isExpr(TTokenType token);

int addFn(symtable_bst_node* symTable, char* name);
int addVar(symtable_bst_node* symTable, char* name);

int expand(TTokenType nextToken);
int parse();

#endif /*!PARSER_H */