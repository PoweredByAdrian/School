/*
 * ----------------------------------------------------------
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
#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef enum
{
	TYPE_ID,
	TYPE_INT,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_VARIABLE,
	TYPE_FUNCTION,

} identifier_type;


typedef struct bst_node {
  char *key;               
  struct SymtableData *data;              
  struct bst_node *left;
  struct bst_node *right;

} *symtable_bst_node;

typedef struct SymtableData
{
	identifier_type type;
	struct bst_node * globalFrame;
	struct bst_node * localFrame;
	bool defined_in_function; 
	bool defined_in_block;
	int numberOfParams;
	identifier_type returnTypes[];

} *symtable_data;


typedef struct SymtableStack
{
	int size;
	int top;
	symtable_bst_node * stack;

}*symtable_stack;


void symtable_init(symtable_bst_node *tree);

void symtable_insert(symtable_bst_node *tree, char *key, symtable_data data);

symtable_data symtable_search(symtable_bst_node *tree, char *key);

void symtable_dispose(symtable_bst_node *tree);

void stack_init(symtable_stack *stack);

void symtable_init_all_nodes(symtable_bst_node tree);

void symtable_init_left_nodes(symtable_bst_node tree, symtable_stack stack);

symtable_bst_node stack_pop(symtable_stack stack);

void stack_push(symtable_stack stack, symtable_bst_node tree);

void stack_clear(symtable_stack *stack);

#endif