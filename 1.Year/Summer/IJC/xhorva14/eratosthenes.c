// eratosthenes.c
// Riešenie IJC-DU1, príklad a), 24.3.2021
// Autor: Adrián Horváth, FIT
// Preložene: gcc 10.2.1
// Zdrojový soubor funkcie na vypočet prvočísel


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "eratosthenes.h"


//bit = 0 => je prvocislo
// bit = 1 => neni prvocislo


void Eratosthenes(bitset_t pole)  
{                                
  unsigned long cislo = bitset_size(pole);
  bitset_setbit(pole, 1, 1);
  bitset_setbit(pole, 0, 1);
  for(unsigned long i = 1; i < sqrt(cislo); i++)
   {
    if(bitset_getbit(pole, i) == 1)
    {
      continue;
    }
    else if(bitset_getbit(pole, i) == 0)
    {
      for(unsigned long n = 2; (n * i) < cislo; n++)
      {
        bitset_setbit(pole, (n * i), 1);
      }
    }
  }
}


























/*Jako testovací příklad implementujte funkci, která použije algoritmus známý
   jako Eratostenovo síto (void Eratosthenes(bitset_t pole);) a použijte ji
   pro výpočet posledních 10 prvočísel ze všech prvočísel od 2 do
   N=200000000 (200 milionů). (Doporučuji program nejdříve odladit pro N=100.)
   Funkci Eratosthenes napište do samostatného modulu "eratosthenes.c".

   Každé prvočíslo tiskněte na zvláštní řádek v pořadí
   vzestupném.  Netiskněte  nic  jiného  než  prvočísla (bude se
   automaticky  kontrolovat!).  Pro kontrolu správnosti prvočísel
   můžete použít program "factor" (./primes|factor).
*/


/*Poznámky:  Eratosthenovo síto (přibližná specifikace):
   1) Nulujeme bitové pole  p  o rozměru N,
      p[0]=1; p[1]=1; // 0 a 1 nejsou prvočísla
      index i nastavit na 2
   2) Vybereme nejmenší index i, takový, že p[i]==0.
      Potom je i prvočíslo
   3) Pro všechny násobky i nastavíme bit p[n*i] na 1
      ('vyškrtneme' násobky - nejsou to prvočísla)
   4) i++; dokud nejsme za sqrt(N), opakujeme bod 2 až 4
      (POZOR: sestavit s matematickou knihovnou parametrem -lm)
   5) Výsledek: v poli p jsou na prvočíselných indexech hodnoty 0
*/