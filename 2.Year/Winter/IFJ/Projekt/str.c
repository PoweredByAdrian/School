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
#include <string.h>
#include <malloc.h>
#include "str.h"

#define STR_INC_LEN 8 // Size of initialized string

bool strInit(string* str) {
  if (str == NULL) {
    return false;
  }
  str->str = (char*) malloc(STR_INC_LEN);
  if (str->str == NULL) {
   return false;
  }
  str->length = 0;
  str->str[0] = '\0';
  str->size = STR_INC_LEN;
  return true;
}

void strFree(string* str) {
  if (str == NULL || str->str == NULL) {
    return;
  }
  free(str->str);
  str->str = NULL;
}

void strClear(string* str) {
  if (str == NULL) {
    return;
  }
  str->str[0] = '\0';
  str->length = 0;
}

bool strAddChar(string* str, char c) {
  if (str == NULL) {
    return false;
  }
  if ((str->length + 1) >= str->size) {
    int newsize = str->length + STR_INC_LEN;
    str->str = (char*) realloc(str->str, newsize);
    if (str->str == NULL) {
      return false;
    } else str->size = newsize;
  }

  str->str[str->length] = c;
  str->length++;
  str->str[str->length] = '\0';
  return true;
}

bool strAddConst(string* str, char* str_const) {
  if (str == NULL || str_const == NULL) {
    return false;
  }
  int constLen = strlen(str_const);
  if (str->length + constLen + 1 >= str->size) {
    int newsize = str->length + constLen + 1;
    str->str = (char*) realloc(str->str, newsize);
    if (str->str == NULL) {
      return false;
    }
    str->size = newsize;
  }

  str->length = str->length + constLen;
  strcat(str->str, str_const); // String concatenation
  str->str[str->length] = '\0'; // Not sure if strcat does this so I'll just do it manually
  return true;
}

bool strCopy(string* src, string* dest) {
  if (src == NULL || dest == NULL) {
    return false;
  }
  int newlen = src->length;
  if (dest->size <= src->length) {
    newlen++;
    dest->str = (char*) realloc(dest->str, newlen);
    if (dest->str == NULL) {
      return false;
    }
    dest->size = newlen;
  }

  strcpy(dest->str, src->str);
  dest->length = newlen;
  return true;
}

int strCmp(string* str1, string* str2) {
  if (str1 == NULL || str2 == NULL) {
    return 1;
  }
  return strcmp(str1->str, str2->str);
}

int strCmpConst(string* str, char* str_const) {
  if (str == NULL || str_const == NULL) {
    return 1;
  }
  return strcmp(str->str, str_const);
}

char* strToConst(string* str) {
  if (str == NULL) {
    return NULL;
  }
  return str->str;
}
