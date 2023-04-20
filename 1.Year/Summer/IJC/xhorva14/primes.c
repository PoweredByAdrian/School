// primes.c
// Riešenie IJC-DU1, príklad a), 24.3.2021
// Autor: Adrián Horváth, FIT
// Preložené: gcc 7.5
// Implementácia výpočtu prvočísel

#include <stdio.h>
#include <time.h>
#include "bitset.h"
#include "eratosthenes.h"

#define N 200000000

int main()
{
  clock_t start = clock();
  bitset_create(pole, N);
  Eratosthenes(pole);

  int j = 10;

  unsigned long last_primes[10];

  for (unsigned long i = N - 1; i > 1; i--)
  {
    if (bitset_getbit(pole, i) == 0)
    {
      last_primes[j] = i;
      if (j == 0)
      {
        break;
      }
      j--;
    }
  }
  for (int i = 0; i < 10; i++)
  {
    printf("%ld\n", last_primes[i]);
  }
  
  fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);

  return 0;
}