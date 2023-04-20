/**
 * htab_bucket_count.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci vracejici pocet prvku pole rozptylove tabulky.
 */

#include "htab.h"

/**
 * Funkce vraci pocet prvku pole tabulky
 */
size_t htab_bucket_count(const htab_t * t)
{
    return t->arr_size;
}
