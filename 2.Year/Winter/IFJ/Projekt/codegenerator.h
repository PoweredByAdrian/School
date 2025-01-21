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
#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "symtable.h"
#include "type.h"

#define MAIN "main"
#define PRINT "print"

void codeFromToken(Token *token, Token *prevToken);
void printHeaderCode();

char *floatToVariable(double rawFloat);
char *stringToVariable(char *rawString);
char *integerToVariable(int rawInt);
char *booleanToVariable(int isTrue);
char *nilToVariable();
char *variableToFrame(char *id);

#endif