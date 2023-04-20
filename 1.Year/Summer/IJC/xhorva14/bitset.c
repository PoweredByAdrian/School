// bitset.h
// Riešenie IJC-DU1, príklad a), 24.3.2021
// Autor: Adrián Horváth, FIT
// Preložene: gcc 10.2.1
// Hlavičkový súbor pre makra

#include "bitset.h"

extern bitset_index_t bitset_size(bitset_t jmeno_pole);
extern void bitset_free(bitset_t jmeno_pole);
extern void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, int vyraz);
extern int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);