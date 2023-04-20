#ifndef __HTAB_STRUCTS_H__
#define __HTAB_STRUCTS_H__



#include "htab.h"


struct htab_item {

    htab_pair_t *data;
    struct htab_item *next;
};


struct htab {

    size_t size;
    size_t arr_size;
    struct htab_item *items[];
};

#endif // __HTAB_STRUCTS_H__