/**
 * htab_begin.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci, ktera nastavi pocet vyskytu daneno zaznamu.
 */

#include "htab.h"

/**
 * Funkce je predan iterator, a pokud je validni, pocet vyskytu daneho zaznamu,
 * na ktery odkazuje se nastavi na hodnotu val, pricemy tuto hodnotu take vrati.
 */
int htab_iterator_set_value(htab_iterator_t it, int val)
{
    if(htab_iterator_valid(it))
    {
        it.ptr->data = val;
        return val;
    }

    return -1;
}
