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

#ifndef TOKENSTACK_H
#define TOKENSTACK_H

#include <stdbool.h>
#include "type.h"
#include "token.h"

typedef struct tokenStackItem{
  Token* token;
  struct tokenStackItem* next;
} tStackItem;

typedef struct tokenStack{
  tStackItem* top;
} tStack;

// Functions
void tStackInit(tStack* stack);
bool tStackPush(tStack* stack, Token* token);
Token* tStackTop(tStack* stack);
Token* tStackPop(tStack* stack);
void tStackDestroy(tStack* stack);

#endif
