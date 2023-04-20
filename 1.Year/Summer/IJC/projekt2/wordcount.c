/**
 * wordcount.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Program pocita pocet slov v stupnom texte, slovo je cokolvek oddelene "White-space" znakom
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htab.h"

/*Velkost tabulky bola zvolena na zaklade pasivnej slovnej zasoby cloveka co je cca 50 000 slov + 1.2 nasobok 
pre istotu a velkost tabulky by mala byt prvocislo */

#define MAX_HTAB_SIZE 59999 
#define MAX_WORD_LENGTH 128

//funkcia na vypis tabulky  volana htba_for_each
void print_item(htab_pair_t *data)
{
    printf("%s\t%d\n", data->key, data->value);
}


int main()
{
    //inicializacia tabulky
    htab_t *table = htab_init(MAX_HTAB_SIZE);
    char word[MAX_WORD_LENGTH]; //pomocna premenna pre fscanf
    htab_pair_t *item;
    
    //nacitavanie slov
    while (fscanf(stdin, "%s", word) != EOF)
    {   
        item = htab_lookup_add(table, word);
        //zvisenie poctu slov
        item->value += 1;

    }

    htab_for_each(table, print_item);
    htab_free(table);

    return 0;
}