/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul pre inicializaciu tabulky
 */
#include <stdlib.h>
#include "htab.h"
#include "htab_struct.h"

htab_t *htab_init(size_t n)
{
    htab_t *hash_table = malloc(sizeof(htab_t) + sizeof(struct htab_item) * n);
    if (hash_table == NULL) 
        return NULL;

    //nastavenie velkosti tabulky
    hash_table->size = 0;
    hash_table->arr_size = n;

    for (size_t i = 0; i < n; i++) 
        hash_table->items[i] = NULL;

    return hash_table;
}