/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na zistenie poctu slov v tabulke
 */
#include "htab.h"
#include "htab_struct.h"

size_t htab_size(const htab_t * t)
{
    return t->size;
}