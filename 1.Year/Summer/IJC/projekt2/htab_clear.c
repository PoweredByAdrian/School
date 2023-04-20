/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na vycistenie tabulky
 */
#include <stdlib.h>
#include "htab.h"
#include "htab_struct.h"

void htab_clear(htab_t * t)
{
    //prechadzanie cez vsetky ukazatele pola
    for (int i=0;i<t->arr_size;i++)
    {
        if (t->items[i] != NULL)
        {
            struct htab_item *current_item = t->items[i];
            struct htab_item *next_item;
            //prechadzanie vsetkych poloziek v zozname 
            while (current_item != NULL)
            {   
                htab_pair_t *data = current_item->data;
                next_item = current_item->next;

                free(data);
                free(current_item);

                current_item = next_item;
            }
        }
    }
}