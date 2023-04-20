/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na presunutie obsahu tabulky
 */
#include <stdlib.h>
#include <stdio.h>
#include "htab.h"
#include "htab_struct.h"


htab_t *htab_move(size_t n, htab_t *from)
{
    //inicializacia novej tabulky
    htab_t *tab = htab_init(n);

    //presunutie ukazatelov
    for (size_t i=0;i<from->arr_size;i++)
        tab->items[i] = from->items[i];

    //stara tabulka musi byt prazdna ale allocovana 
    htab_clear(from);

    return tab;
}