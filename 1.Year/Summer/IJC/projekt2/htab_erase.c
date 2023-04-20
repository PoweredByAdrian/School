/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na zmazanie zaznamu
 */
#include <stdbool.h>
#include <string.h>
#include "htab.h"
#include "htab_struct.h"

bool htab_erase(htab_t *t, htab_key_t key)
{
    //vypocitanie indexu ukazatela
    size_t index = htab_hash_function(key) % t->arr_size;
    struct htab_item *current_item = t->items[index];
    struct htab_item *previous_item = t->items[index];

    bool nested = false; //false= prva polozka | true = vnoreny item

    //prechadzanie itemov z zozname
    while(current_item!=NULL)
    {
        //kontrolovanie itemu
        if (strcmp(current_item->data->key, key) == 0)
        {
            //ak je item vnoreny prepisu sa ukazatele next na dalsi prvok
            if (nested)
            {
                previous_item->next = current_item->next;
                //uvolnenie polozky ukazatela na strukturu
                free(current_item->data);
                free(current_item);
                current_item = NULL;
                return true;
            }
            
            //ak je polozka prva nastavime ukazatel pola na NULL
            t->items[index] = NULL;
            return true;
        }
        
        
        previous_item = current_item;
        current_item = current_item->next;
        nested = true;
    }

    return false;
}