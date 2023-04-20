/**
 * htab_iterator_get_key.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci vracejici klic zaznamu odpovidajiciho iteratoru.
 */

#include "htab.h"

/**
 * Funkce vraci klic zaznamu daneho iteratoru.
 * Cil musi existovat.
 */
const char * htab_iterator_get_key(htab_iterator_t it)
{
    if(htab_iterator_valid(it))
    {
        return it.ptr->key;
    }

    return NULL;
}
