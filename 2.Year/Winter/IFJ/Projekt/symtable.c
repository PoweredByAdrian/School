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

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

void symtable_init(symtable_bst_node *tree)
{
  if (tree == NULL) return;
  (*tree) = NULL;
}

void symtable_insert(symtable_bst_node *tree, char *key, symtable_data data)
{
  if (tree == NULL) return;

  symtable_bst_node newTree = malloc(sizeof(struct bst_node));

	newTree->right = NULL;
	newTree->left = NULL;
	newTree->data = data;
	newTree->key = key;

	if (*tree == NULL)
	{
		*tree = newTree;
		return;
	}

	symtable_bst_node node = *tree;
	int side = 0;

	while (node != NULL)
  {
		int compare = strcmp(key, node->key);

		if (compare > 0)
		{
			if (node->right != NULL)
      {
				node = node->right;
			} 
      else
      {
				side = 1;
				break;
			}
			
		}
		else if (compare < 0)
		{
			if (node->left != NULL)
      {
				node = node->left;
			}
      else
      {
				break;
			}
			
		}
		else
		{
			free(node->data);
			node->data = data;
			return;
		}
	}

	if (side)
  {
		node->right = newTree;
	}
  else
  {
		node->left = newTree;
	}

}

symtable_data symtable_search(symtable_bst_node *tree, char *key)
{
  if (tree == NULL) return NULL;

  symtable_bst_node node = *tree;

	while (node != NULL)
  {
		int compare = strcmp(key, node->key);

		if (compare > 0)
		{
			node = node->right;
		}
		else if (compare < 0)
		{
			node = node->left;
		}
    else
    {
			return node->data;
		}
	}
	
	return NULL;

}

void symtable_init_left_nodes(symtable_bst_node tree, symtable_stack stack)
{
  if (tree == NULL) return;

  do
  {
		stack_push(stack, tree);
		tree = tree->left;
	} while (tree != NULL);

}

void symtable_init_all_nodes(symtable_bst_node tree)
{
  symtable_stack stack;
	stack_init(&stack);
	symtable_init_left_nodes(tree, stack);

	while (stack->top >= 0)
	{
		tree = stack_pop(stack);
		symtable_init_left_nodes(tree->right, stack);
	}

	stack_clear(&stack);

}

void symtable_dispose(symtable_bst_node *tree)
{
  if (tree == NULL || (*tree) == NULL) return;

  if ((*tree)->left != NULL)
  {
    symtable_dispose(&((*tree)->left));
  }
  if ((*tree)->right != NULL)
  {
    symtable_dispose(&((*tree)->right));
  }

  if ((*tree)->data->localFrame != NULL)
	{
		symtable_dispose(&(*tree)->data->localFrame);
	}
	if ((*tree)->data->globalFrame != NULL)
	{
		symtable_dispose(&(*tree)->data->globalFrame);
	}

	free((*tree)->data);


  free((*tree));
  (*tree) = NULL;
}

void stack_init(symtable_stack *stack)
{
  if (stack == NULL) return;

  (*stack) = malloc(sizeof(struct SymtableStack));
	(*stack)->stack = NULL;
	(*stack)->size = 0;
	(*stack)->top = -1;

}

void stack_push(symtable_stack stack, symtable_bst_node tree)
{
  if (stack == NULL) return;

  stack->top++;

	if (stack->size <= stack->top)
	{
		stack->size += 200;
		stack->stack = realloc(stack->stack, sizeof(symtable_bst_node) *stack->size);
	}

	stack->stack[stack->top] = tree;
}

symtable_bst_node stack_pop(symtable_stack stack)
{
  if (stack->top <= -1 || stack == NULL) return NULL;
	stack->top--;

	return stack->stack[stack->top + 1];

}

void stack_clear(symtable_stack *stack)
{
  if (stack == NULL) return;

  free((*stack)->stack);
	free(*stack);
	*stack = NULL;
}