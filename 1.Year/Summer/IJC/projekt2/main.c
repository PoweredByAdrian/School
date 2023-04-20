/*#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "htab.h"
#include "htab_struct.h"
#include "hash_function.c"
#include "htab_bucket_count.c"
#include "htab_move.c"
#include "htab_clear.c"
#include "htab_find.c"
#include "htab_lookup_add.c"
#include "htab_erase.c"
#include "htab_for_each.c"
#include "htab_free.c"
#include "htab_init.c"


void print_table(htab_pair_t *data)
{
    printf("%s : %d\n", data->key, data->value);
}

int main()
{
    char *names[] = {"Eric", "Stan", "Kyle", "Kenny", "Butters"};

    int name_count = sizeof(names) / sizeof(names[0]);

    htab_t *tab = htab_init(5);
    if (tab == NULL) return 0;

    for (int i=0;i<name_count;i++)
    {
        htab_lookup_add(tab, names[i]);

    }

    htab_lookup_add(tab, "Asdf");
    htab_lookup_add(tab, "fdsA");

    htab_for_each(tab, print_table);
    printf("============================\n");
    htab_free(tab);

    return 0;
}*/