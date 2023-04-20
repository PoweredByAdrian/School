/**
 * htab_iterator_get_value.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci vracejici pocet vyskytu daneho zaznamu.
 */

#include "htab.h"

/**
 * Funkci je predan interator na zaznam, a pokud je validni, funkce vrati pocet vyskytu daneho zaznamu.
 */
int htab_iterator_get_value(htab_iterator_t it)
{
    if(htab_iterator_valid(it))
    {
        return it.ptr->data;
    }

    return -1;
}
