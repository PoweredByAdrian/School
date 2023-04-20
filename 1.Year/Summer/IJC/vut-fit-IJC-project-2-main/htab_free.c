/**
 * htab_free.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci, ktera rusi rozpltylovou tabulku.
 */


#include <stdlib.h>
#include "htab.h"

/**
 * Funkce zrusi tabulku.
 * Nejprve ji inicializuje na prazdnou a pote ji uvolni.
 */
void htab_free(htab_t * t)
{
    htab_clear(t);
    free(t);
}
