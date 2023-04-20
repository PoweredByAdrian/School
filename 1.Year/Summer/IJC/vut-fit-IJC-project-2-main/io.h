/**
 * io.h
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s prototypen funkce pro nacteni slova.
 */

#ifndef IO_H
#define IO_H

/**
 * Funkce nacte slovo ze souboru.
 */
int get_word(char *s, int max, FILE *f);

#endif
