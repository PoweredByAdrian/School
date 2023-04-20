/**
 * htab_hash_function.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s rozptylovacimi funkcemi.
 */


#include <stdint.h>

#ifndef HASH_TEST

unsigned int htab_hash_function(const char *str)
{
    uint32_t h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
    {
        h = 65599*h + *p;
    }

    return h;
}

#else

unsigned int htab_hash_function(unsigned char *str)
{
	uint32_t hash = 5381;
	int c;

	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

#endif
