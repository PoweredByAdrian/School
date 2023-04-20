/**
 * htab_size.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci vracejici pocet zaznamu v rozptylove tabulce..
 */

#include "htab.h"

/**
 * Funkce vraci pocet zaznamu v tabulce.
 */
size_t htab_size(const htab_t * t)
{
    return t->size;
}
