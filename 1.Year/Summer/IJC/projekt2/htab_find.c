/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul s funkciou na najdenie slova v tabulke
 */
#include <stdio.h>
#include <string.h>
#include "htab.h"
#include "htab_struct.h"


htab_pair_t *htab_find(htab_t *t, htab_key_t key)
{
    //vypocitanie indexu ukazatela 
    size_t index = htab_hash_function(key) % t->arr_size;
    struct htab_item *tItem = t->items[index];

    while(tItem!=NULL)
    {
        //vratenie ukazatela na key
        if (strcmp(tItem->data->key, key) == 0)
            return tItem->data;
        tItem = tItem->next;
    }
    return NULL;
}