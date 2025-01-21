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
#ifndef ERROR_H
#define ERROR_H

#ifndef NDEBUG
#define ERROR(...) fprintf(stderr, __VA_ARGS__)
#else
#define ERROR(...)
#endif

#define SUCCESS 0 // No Error Found

// Errors
#define ERR_LEXICAL 1 // Lexical Error
#define ERR_SYNTAX 2 // Syntax Error
#define ERR_SEM_DEFINE 3 // Undefined func/var, defining an already defined func/var...
#define ERR_SEM_TYPE 4 // Type incompability in assigment
#define ERR_SEM_PARAM 5 // Wrong number of parameters or return values in functions
#define ERR_SEM_TYPE_EXPR 6 // Type incompability in expressions
#define ERR_SEM_OTHER 7 // Other semantic errors
#define ERR_NIL 8 // Attempt to call nil
#define ERR_SEM_DIVZ 9 // Division by zero
#define ERR_INTERNAL 99 // Internal Error

#endif
