/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na uvolnenie tabulky
 */
#include "htab.h"
#include "htab_struct.h"


void htab_free(htab_t * t)
{
    //uvolnenie zaznamov
    htab_clear(t);
    //uvolnenie ukazatela na tabulku
    free(t);
}