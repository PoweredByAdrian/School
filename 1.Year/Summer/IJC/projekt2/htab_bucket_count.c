/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na zistenie velkosti tabulky
 */
#include "htab.h"
#include "htab_struct.h"

size_t htab_bucket_count(const htab_t * t)
{
    return t->arr_size;
}