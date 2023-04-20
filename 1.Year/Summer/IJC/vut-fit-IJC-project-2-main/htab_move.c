/**
 * htab_begin.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci pro presun zaznamu z jedne rozptylove tabulky do druhe.
 */

#include <stdlib.h>
#include "htab.h"

/**
 * Funkce vytvori a inicializuje novou tabulku presunem dat z tabulky from.
 * Tabulka from nakonec zustane prazdna a alokovana.
 */
htab_t *htab_move(size_t n, htab_t *from)
{
    htab_t *new_table = htab_init(n);

	unsigned int from_bucket_count = htab_bucket_count(from);	    //pocet prvku pole tabulky from
	unsigned int new_bucket_count = htab_bucket_count(new_table);	//pocet prvku pole nove tabulky
	
	// Pokud alokace selze, vraci NULL.
	if(new_table == NULL)
	{
		return NULL;
	}

    struct htab_item *tmp;
    unsigned int index;
	
	// pruchod pres vsechny zaznamy
    for(unsigned int i = 0; i < from_bucket_count; i++)
    {
        if(from->ptr[i] != NULL)
        {
            while(from->ptr[i] != NULL)
            {
				// prepocet noveho indexu
                index = htab_hash_function(from->ptr[i]->key) % new_bucket_count;

                tmp = new_table->ptr[index];
                new_table->ptr[index] = from->ptr[i];
                from->ptr[i] = from->ptr[i]->next;
                new_table->ptr[index]->next = tmp;
            }
        }
    }

	new_table->size = from->size;
    from->size = 0;

    return new_table;
}
