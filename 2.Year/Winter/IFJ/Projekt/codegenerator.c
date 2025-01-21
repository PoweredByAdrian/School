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
#include "codegenerator.h"

void printHeaderCode()
{
	printf(".IFJcode20\nJUMP $$main\n\n");
}


void codeFromToken(Token *token, Token *prevToken)
{
	if (token == NULL || prevToken == NULL) return;
    
	static bool print = false;
	static bool definingFunction = false;

	switch (token->type)
	{
		case ID:
			if (token->value && strcmp(token->value, PRINT) == 0)
			{
				// v dalsom cykle volaj print
				print = true;
			}
			else if (prevToken->type == kwFUNCTION || prevToken->type == LBRACKET)
			{
				if (strcmp(token->value, MAIN) == 0)
				{
					printf("LABEL $main\nCREATEFRAME\n");
					printf("PUSHFRAME\n");

				}
				else
				{
					definingFunction = true;
					printf("JUMP $%s_end\nLABEL $%s_body\n", token->value, token->value);
				}
			}

			break;
		case EOFI:
			printf("EXIT int@0\n");
			break;
		case STRING:
			if (print)
			{
				printf("WRITE %s\n", token->value);
			}

			break;
		case LBRACKET:
			if (prevToken->type == ID && !print && !definingFunction && strcmp(prevToken->value, MAIN) != 0)
			{
				printf("CALL $%s_body\n", prevToken->value);
			}
			break;

		default:
			break;
	}

	return;
}