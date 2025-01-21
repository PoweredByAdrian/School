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
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"
#include "type.h"
#include "str.h"
#include "parser.h"
#include "codegenerator.h"

int main(){
    // Token tok;
    // Token *token = &tok;
    // do{
    //     token = getNextToken();
    //     //put parse here
    //     #ifdef DEBUG
    //     ERROR("Type:  %d\tValue: %s\n",token_get_type(token), token_get_value(token));
    //     #endif
    // }
    // while(token_get_type(token)!=EOFI);

    int result = parse();
    if(result == 0)     printHeaderCode();
    ERROR("%d\n", result);
    return result;
}