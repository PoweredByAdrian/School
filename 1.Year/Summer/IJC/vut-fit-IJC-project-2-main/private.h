/**
 * private.h
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s definicemi struktur rozptylove tabulky.
 */

#ifndef PRIVATE_H
#define PRIVATE_H

/**
 * struktura tabulky
 */
struct htab {
    size_t size;	            //aktualni pocet zaznamu
    size_t arr_size;	        //velikost nasledujiciho pole ukazatelu
    struct htab_item *ptr[];	//pole ukazatelu na zaznamy
};

/**
 * struktura zaznamu
 */
struct htab_item {
    char *key;	                //klic - slovo
    unsigned int data;	        //pocet vyskytu
    struct htab_item *next;	    //ukazatel na dalsi zaznam
};

#endif
