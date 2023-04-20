/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul s rozptylovacou funkciou
 */
#include "htab.h"
#include <stdint.h>

size_t htab_hash_function(htab_key_t str)
{
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;

    return h;
}