/**
 * wordcount.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul se zdrojovym kodem programu wordcount, ktery pocita vyskyt jednotlivych slov v souboru.
 */

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "io.h"

#define WORD_LENGTH 128

/*
 Velikost tabulky bula zvolena na zaklade pasivni slovni zasoby a testu na nejruznejsich knihach.
 Vychazi ze zjisteni, ze v kazde delsi knize se slovni zasoba pohybuje okolo 30 000 - 40 000 slovy, pricemz pasivni slovni zasoba cloveka muze dosahovat az 50 000.
 Pri spojeni vice knih dohromady mnozstvi ruznych slov dale narustalo, proto definovana velikost zavisi vice na slovni zasobe.
 Definovany pocet vychazi ze dvou pravidel:
 1) Velikost royptylove tabulky by mela byt prvocislem
 2) velikost = 1.3 * pocet_ukladanych_zaznamu
 Tabulka ma tedy byt schopna pojmout efekticvne pasivni slovni zasobu
 */
#define HTAB_SIZE 65003

#define TEST_VALUE 102149

int main()
{
	char word[WORD_LENGTH];	// pomocne pole pro nacitani slov

	// inicializace rozptylove tabulky
	htab_t *table = htab_init(HTAB_SIZE);

	if(table == NULL)
	{
		fprintf(stderr, "CHYBA: Alolace pameti tabulky selhala\n");
		return 1;
	}

	htab_iterator_t iterator;

	// nacitani slov ze standardniho vstupu
	while(get_word(word, WORD_LENGTH, stdin) != EOF)
	{
		// ziskani iteratoru na zaznam dany klicem
		iterator = htab_lookup_add(table, word);
		
		// test validity iteratoru
		if(!htab_iterator_valid(iterator))
		{
			fprintf(stderr, "CHYBA: Alokace pameti pro dalsi zaznam se nezdarila.\n");
		}
		
		// inkrementace vyskytu daneho slova
		htab_iterator_set_value(iterator, htab_iterator_get_value(iterator) + 1);
	}

// Pokud je definovano makro TEST puvodni tabulka je prekopirovana do jine, pricemz se inicializuje na prazdnou.
#ifdef TEST

	htab_t *new_table = htab_move(TEST_VALUE, table);

	htab_free(table);

	table = new_table;

	if(table == NULL)
	{
		fprintf(stderr, "CHYBA: Realokace tabulky selhala.\n");
	}

#endif

	// ziskani iteratoru na prvni zaznam v rozptylove tabulce
	iterator = htab_begin(table);
	
	// pruchod pres zaznamy v tabulce od prvniho k poslednimu
	while(htab_iterator_valid(iterator)) // == while(!htab_iterator_equal(iterator, htab_end(table)))
	{
		printf("%s\t%d\n", htab_iterator_get_key(iterator), htab_iterator_get_value(iterator));
		
		// Tato funkce vraci vzdy validni iterator, pokud jiz neprosla pres posledni zaznam.
		iterator = htab_iterator_next(iterator);
	}
	
	// uvolneni tabulky
	htab_free(table);

    return 0;
}
