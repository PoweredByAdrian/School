/**
 * htab_begin.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci, ktera vraci iterator na prvni zaznam.
 */

#include "htab.h"

/**
 * Funkce prochazi rozptylovou tabulkou a hleda prvni zaznam,
 * pokud ho nalezne vraci na nej iterator.
 * V opacnem pripade vraci nevalidni iterator.
 */
htab_iterator_t htab_begin(const htab_t * t)
{
    htab_iterator_t iterator;
	unsigned int bucket_count = htab_bucket_count(t);
	
	// inicializace iteratoru
    iterator.ptr = NULL;
    iterator.t = t;
    iterator.idx = -1;
	
	// pruchod pres vsechny polozky
    for(unsigned int i = 0; i < bucket_count; i++)
    {
        if(t->ptr[i] != NULL)
        {
			// prvni zaznam
            iterator.ptr = t->ptr[i];
            iterator.idx = i;
            break;
        }
    }

    return iterator;
}
