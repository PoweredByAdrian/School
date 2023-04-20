/**
 * inline.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s prototypy inline funkci.
 */

#include "htab.h"

/**
 * Funkce testuje, zda je iterator validni.
 */
extern inline bool htab_iterator_valid(htab_iterator_t it);

/**
 * Funkce porovna dva iteratory.
 */
extern inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2);
