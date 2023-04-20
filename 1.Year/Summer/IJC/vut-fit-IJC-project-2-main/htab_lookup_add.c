/**
 * htab_lookup_add.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci, ktera v rozptylove tabulce vyhleda zaznam odpovidajiciho klice.
 */

#include <stdlib.h>
#include "htab.h"

/**
 * Funkce v tabulce hleda zaznam daneho klice.
 * Pokud ho nalezne, vrati na nej iterator.
 * V opacnem pripade vytvori zaznam a na ten vrati iterator.
 */
htab_iterator_t htab_lookup_add(htab_t * t, const char *key)
{
	// vypocet indexu
    unsigned int index = htab_hash_function(key) % htab_bucket_count(t);

    htab_iterator_t iterator;

    iterator.t = t;
    iterator.idx = index;
	
	// pruchod pres vsechny zaznamy na danem indexu
    for(struct htab_item *tmp = t->ptr[index]; tmp != NULL; tmp = tmp->next)
    {
        if(strcmp(key, tmp->key) == 0)
        {
            iterator.ptr = tmp;
            return iterator;
        }
    }
	
	// alokace pameti pro novy zaznam
    struct htab_item *new_item = malloc(sizeof(struct htab_item));
	
	// Pokud se alokace nezdari, vrati nevalidni iterator.
    if(new_item == NULL)
    {
        iterator.ptr = NULL;
        return iterator;
    }
	
	// alokace pameti pro klic zaznamu
    unsigned int length = strlen(key);

    new_item->key = malloc(length + 1);
	
    if(new_item->key == NULL)
    {
        iterator.ptr = NULL;
        return iterator;
    }
	
	// ulozeni klice
    strncpy(new_item->key, key, length);
    new_item->key[length] = '\0';
	
	// zarazeni zaznamu
    new_item->data = 0;
    new_item->next = t->ptr[index];
    t->ptr[index] = new_item;

    t->size++;

    iterator.ptr = new_item;

    return iterator;
}
