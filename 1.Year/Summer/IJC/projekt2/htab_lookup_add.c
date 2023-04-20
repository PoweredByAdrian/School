/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul na priradenie slova do tabulky
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "htab.h"
#include "htab_struct.h"


htab_pair_t *htab_lookup_add(htab_t *t, htab_key_t key)
{
    htab_pair_t *pair = NULL;

    //ak najde polozku tak vrati ukazatel
    if (htab_find(t, key) != NULL)
        return pair;

    else
    {   
        //zisti index na ktory treba ulozit polozku
        size_t index = htab_hash_function(key) % t->arr_size;

        //ak je prazdny ulozi ho na zaciatok pola 
        if (t->items[index] == NULL)
        {   
            //allocuje potrebne miesto na struktury
           /* t->items[index] = malloc(sizeof(struct htab_item));
            if (t->items[index] == NULL) 
                return NULL;

            t->items[index]->data = malloc(sizeof(htab_pair_t));
            if (t->items[index]->data == NULL) 
                return NULL;

            t->items[index]->data->key = malloc(sizeof(key));
            if (t->items[index]->data->key == NULL) 
                return NULL;*/
            
            struct htab_item *item = funkc(t->items[index], key);
                if (item == NULL)
                    return NULL;

            //skopiruje key do tabulky a zvisi hotnodu size
            strncpy((char*) item->data->key, key, strlen(key));
            item->data->value = 0;
            t->size++;
            pair = t->items[index]->data;
        }
        else
        {
            struct htab_item *current_item = t->items[index];
            struct htab_item *next_item;

            //iterovanie na koniec zoznamu
            while(current_item->next!=NULL) 
                current_item = current_item->next;

            //allocuje potrebne miesto na struktury
            /*next_item = malloc(sizeof(struct htab_item));
            if (next_item == NULL) 
                return NULL;

            next_item->data = malloc(sizeof(htab_pair_t));
            if (next_item->data == NULL) 
                return NULL;

            next_item->data->key = malloc(sizeof(key));
            if (next_item->data->key == NULL) 
                return NULL;*/

            next_item = funkc(next_item, key);
                if (next_item == NULL)
                    return NULL;
            //skopirovanie slova do zoznamu
            strncpy((char*) next_item->data->key, key, strlen(key));
            next_item->data->value = 0;
            t->size++;
            //zviazanie zoznamu
            current_item->next = next_item;
            pair = next_item->data;
        }
        return pair;
    }
}

struct htab_item* funkc(struct htab_item *item, htab_key_t key){

    item = malloc(sizeof(struct htab_item));
    if (item == NULL) 
        return NULL;

    item->data = malloc(sizeof(htab_pair_t));
    if (item->data == NULL) 
        return NULL;

    item->data->key = malloc(sizeof(key));
    if (item->data->key == NULL) 
        return NULL;

    return item;
    
}