/**
 * htab.h
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Knihovna fukci rozptylove tabulky.
 */

// Licence: zadna (Public domain)

// nasledujici radky zabrani nasobnemu vlozeni:
#ifndef __HTABLE_H__
#define __HTABLE_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include "private.h"

// tabulka:
struct htab;    // neuplna deklarace struktury - uzivatel nevidi obsah
typedef struct htab htab_t;     // typedef podle zadani
// iterator do tabulky:
struct htab_item;               // neuplna deklarace struktury

// iterator:
typedef struct htab_iterator {
    struct htab_item *ptr;      // ukazatel na polozku
    const htab_t *t;            // ve ktere tabulce
    int idx;                    // ve kterem seznamu tabulky
} htab_iterator_t;              // typedef podle zadani

// rozptylovaci (hash) funkce
unsigned int htab_hash_function(const char *str);

// funkce pro praci s tabulkou:
htab_t *htab_init(size_t n);
htab_t *htab_move(size_t n, htab_t *from);
size_t htab_size(const htab_t * t);             // pocet zaznamu v tabulce
size_t htab_bucket_count(const htab_t * t);     // velikost pole

htab_iterator_t htab_lookup_add(htab_t * t, const char *key);

htab_iterator_t htab_begin(const htab_t * t);
htab_iterator_t htab_end(const htab_t * t);
htab_iterator_t htab_iterator_next(htab_iterator_t it);
inline bool htab_iterator_valid(htab_iterator_t it) { return it.ptr!=NULL; }
inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2) {
  return it1.ptr==it2.ptr && it1.t == it2.t;
}
const char * htab_iterator_get_key(htab_iterator_t it);
int htab_iterator_get_value(htab_iterator_t it);
int htab_iterator_set_value(htab_iterator_t it, int val);

void htab_clear(htab_t * t);
void htab_free(htab_t * t);

#endif // __HTABLE_H__
