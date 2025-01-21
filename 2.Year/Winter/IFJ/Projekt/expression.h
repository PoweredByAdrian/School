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
#include "type.h"
#include "error.h"
#include "tokenstack.h"



/** Checks if expression is syntactically correct
 * @param input_stack pointer to stack with tokens
 * @return SUCESS on success, ERR_INTERNAL or ERR_SYNTAX on error
 */
int expre(tStack* input_stack);