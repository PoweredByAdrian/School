/**
 * htab_iterator_next.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci, ktera vraci iterator na nasledujici zaznam.
 */

#include "htab.h"

/**
 * Funkce prijima iterator na zaznam a vraci iterator na zaznam nasledujici.
 */
htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
	htab_iterator_t end_iterator = htab_end(it.t);
	
	// Pokud dostane iterator za koncem zaznamu, vraci iterator na prvni polozku za poslednim zaznamem.
	if(it.idx >= end_iterator.idx)
	{
		return end_iterator;
	} 
	
	// posun na dalsi polozku
    it.ptr = it.ptr->next;
    
	// test a popr. posun na nasledujici zaznam
	while(it.ptr == NULL)
	{	
		it.idx++;

		if(it.idx == end_iterator.idx)
		{
			return end_iterator;
		}
	
		it.ptr = it.t->ptr[it.idx];
	}
    
    return it;
}
