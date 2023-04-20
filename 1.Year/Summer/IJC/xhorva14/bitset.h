// bitset.h
// Riešenie IJC-DU1, príklad a), 24.3.2021
// Autor: Adrián Horváth, FIT
// Preložene: gcc 10.2.1
// Hlavičkový súbor pre makra

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "error.h"


typedef unsigned long bitset_t[];

typedef unsigned long bitset_index_t;


#define bitset_create(jmeno_pole, velikost) unsigned long jmeno_pole[velikost / (sizeof(unsigned long) * CHAR_BIT) + (velikost % (sizeof(unsigned long) * CHAR_BIT) != 0 ? 2 : 1)] = {velikost, 0};\
assert(velikost < 200000002 && velikost > 64)

#define bitset_alloc(jmeno_pole, velikost) {jmeno_pole = calloc((velikost / (sizeof(unsigned long) * CHAR_BIT) + (size % (sizeof(unsigned long) * CHAR_BIT) > 0 ? 2 : 1)), sizeof(bitset_index_t));\
  if (jmeno_pole == NULL)\
  {\
    error_exit("bitset_alloc: Chyba alokacie pamäte\n");\
  }\
  else\
  {\
    jemno_pole[0] = velikost;\
  }\
}\

#ifndef USE_INLINE

#define bitset_free(jmeno_pole) free(jmeno_pole)

#define bitset_size(jmeno_pole) jmeno_pole[0]

#define bitset_setbit(jmeno_pole, index, vyraz){\
  if (index > bitset_size(jmeno_pole))\
  {\
    error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (bitset_index_t) index, (bitset_index_t) bitset_size(jmeno_pole)); \
  }\
  else\
  {\
    (vyraz) ? (jmeno_pole[index / (sizeof(unsigned long) * CHAR_BIT) + 1] |= ((1UL << (index % (sizeof(unsigned long) * CHAR_BIT))))) : (jmeno_pole[index / (sizeof(unsigned long) * CHAR_BIT) + 1] &= ((1UL << ~(index % (sizeof(unsigned long) * CHAR_BIT)))));\
  }\
}
 #define bitset_getbit(jmeno_pole, index)\
  ((bitset_index_t)index >= bitset_size(jmeno_pole)) ? error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)index, bitset_size(jmeno_pole)), 0 : (bitset_index_t)((jmeno_pole[(index/(sizeof(bitset_index_t)*__CHAR_BIT__)+1)] & ((bitset_index_t)1 << (index%(sizeof(bitset_index_t)*__CHAR_BIT__)))) > 0) \


#else

inline void bitset_free(bitset_t jmeno_pole)
{
  free(jmeno_pole);
}

inline bitset_t bitset_size(bitset_t jmeno_pole)
{
  return jmeno_pole[0];
}

inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, int vyraz)
{
  if (index < 0 || index > bitset_size(jmeno_pole) - 1) {
        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (bitset_index_t) index, (bitset_index_t) bitset_size(jmeno_pole));
  }
  bitset_setbit(jmeno_pole, index, vyraz);
}

inline int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
{
  if (index < 0 || index > bitset_size(jmeno_pole) - 1) {
        error_exit("bitget_getbit: Index %lu mimo rozsah 0..%lu", (bitset_index_t) index, (bitset_index_t) bitset_size(jmeno_pole));
  }
  return (bitset_getbit(jmeno_pole, index));
}

#endif