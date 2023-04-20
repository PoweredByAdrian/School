/**
 * htab_end.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci vracejici iterator na prvni polozku rozptylove tabulky za poslednim zaznamem.
 */

#include "htab.h"

/**
 * Funkce prochazi pole tabulky od konce, a kdyz nalezne zaznam, vraci iterator na polozku, ktera je za nim.
 * Pokud nenalezne zadny zaznam, vraci nevalidni iterator.
 */
htab_iterator_t htab_end(const htab_t * t)
{
    htab_iterator_t iterator;

    iterator.ptr = NULL;
    iterator.t = t;
    iterator.idx = -1;
	
	// pruchod pres polozky tabulky od konce
    for(int i = htab_bucket_count(t) - 1; i >= 0; i--)
    {
        if(t->ptr[i] != NULL)
        {
            iterator.idx = i + 1;
			break;
        }
    }

    return iterator;
}
