/**
 * htab_hash_function.c
 * Riesenie IJC-DU2, priklad 2), 26.4.2021
 * Autor: Adrian Horvath, FIT
 * Prelozeno: gcc 10.2.1
 * Modul s funkciou ktora zavola danu funkciu na vsetky prvky pola
 */
#include "htab.h"
#include "htab_struct.h"

void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data))
{
    //prechadzanie ukazatelov
    for (int i=0;i<t->arr_size;i++)
    {
        //ak je ukazatel NULL tak ho preskoci
        if (t->items[i] != NULL)
        {
            struct htab_item *tItem = t->items[i];
            //prechadzanie cez zoznam
            while (tItem != NULL)
            {
                htab_pair_t *data = tItem->data;
                
                f(data);

                tItem = tItem->next;
            }
        }
    }
}