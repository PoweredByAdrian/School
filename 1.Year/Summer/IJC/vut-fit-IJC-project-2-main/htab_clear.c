/**
 * htab_clear.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci pro vycisteni rozptylove tabulky.
 */


#include <stdlib.h>
#include "htab.h"

/**
 * Funkce zrusi vsechny polozky tabulky, kterou zanecha prazdnou.
 */
void htab_clear(htab_t * t)
{
    t->size = 0;
	unsigned int bucket_count = htab_bucket_count(t);

    struct htab_item *tmp;
	
	// pruchod pres vsechny polozky
    for(unsigned int i = 0; i < bucket_count; i++)
    {
        while(t->ptr[i] != NULL)
        {
			// mazani polozky
            tmp = t->ptr[i];
            t->ptr[i] = tmp->next;
            free(tmp->key);
            free(tmp);
        }
    }
}
