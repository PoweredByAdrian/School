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

#include <stdlib.h>
#include <stdbool.h>
#include "str.h"
#include "tokenstack.h"

#define CHECK_ALLOC(var) if (var == NULL) {return false;};

void tStackInit(tStack* stack) {
  if (stack == NULL) {
    return;
  }
  stack->top = NULL;
}
bool tStackPush(tStack* stack, Token* token) {
  if (stack == NULL) {
    return false;
  }
  if (stack->top == NULL) {
    stack->top = malloc(sizeof(struct tokenStackItem));
    CHECK_ALLOC(stack->top);
    stack->top->token = token;
    stack->top->next = NULL;
    return true;
  }
  tStackItem* item = malloc(sizeof(struct tokenStackItem));
  CHECK_ALLOC(item);
  item->token = token;
  item->next = stack->top;
  stack->top = item;
  return true;
}

Token* tStackPop(tStack* stack) {
  if (stack == NULL) {
    return NULL;
  }
  if (stack->top == NULL) {
    return NULL;
  }
  else if (stack->top->next == NULL) {
    Token* temp = stack->top->token;
    free(stack->top);
    stack->top = NULL;
    return temp;
  }
  else {
    Token* temp = stack->top->token;
    tStackItem* item = stack->top;
    stack->top = stack->top->next;
    free(item);
    return temp;
  }
}

Token* tStackTop(tStack* stack) {
  if (stack == NULL) {
    return NULL;
  }
  if (stack->top == NULL) {
    return NULL;
  }
  else {
    return stack->top->token;
  }
}

void tStackDestroy(tStack* stack) {
  if (stack == NULL) {
    return;
  }
  tStackItem* item = stack->top;
  if (item == NULL) {
    return;
  }
  if (item->next == NULL) {
    free(item->token->value);
    free(item->token);
    free(item);
    return;
  }
  tStackItem* itemnxt = stack->top->next;
  while (itemnxt != NULL) {
    free(item->token->value);
    free(item->token);
    free(item);
    item = itemnxt;
    itemnxt = itemnxt->next;
  }
  free(item->token->value);
  free(item->token);
  free(item);
}
