/**
 * htab_init.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci, ktera inicializuje rozptylovou tabulku.
 */

#include <stdlib.h>
#include "htab.h"

/**
 * Funkce inicializuje alokuje pamet pro danu=y pocet prvku tabulky a tabulku inicializuje na prazdnou.
 */
htab_t *htab_init(size_t n)
{
    htab_t *init_table = malloc(sizeof(htab_t) + (n * sizeof(struct htab_item *)));

	// Pokud alokace selze, funkce vraci null.
    if(init_table == NULL)
    {
        return NULL;
    }

    init_table->size = 0;
    init_table->arr_size = n;

	// inicializace prvku pole tabulky na NULL
    for(unsigned int i = 0; i < n; i++)
    {
        init_table->ptr[i] = NULL;
    }

    return init_table;
}
