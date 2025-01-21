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

#ifndef IFJ20_STR_H
#define IFJ20_STR_H

#include <stdbool.h>

typedef struct {
  char* str; // The actual string data
  int length; // Length of the string
  int size; // Allocated Memory Size | How many chars allocated
} string;

bool strInit(string* str);
void strFree(string* str);
void strClear(string* str);
bool strAddChar(string* str, char c);
bool strAddConst(string* str, char* str_const);
bool strCopy(string* src, string* dest);
int strCmp(string* str1, string* str2);
int strCmpConst(string* str, char* str_const);
char* strToConst(string* str);

#endif