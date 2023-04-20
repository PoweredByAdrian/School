/**
 * io.c
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci pro nacitani slov.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Funkce nacte slovo ze souboru do vstupniho retezce a vrati pocet nactenych znaku.
 * Oddelovacem je jakykoli bily znak.
 * V pripade prekroceni maximalni povolene delky slova, retezec orizne a zbytek zahodi,
 * pricemz tiskne chybove hlaseni (pouze pri prvnim vyskytu prekroceni maximalni delky retezce).
 */
int get_word(char *s, int max, FILE *f)
{
    int character;
    int index = 0;

    static bool overflow = false;
	
	// Nacitani znaku dokud je aktualni index mensi nez maximalni pocet znaku.
    while(index < max)
    {
        character = fgetc(f);
		
		// Nacitani konci, pokud aktualni index neni nulovy a je nacten bily znak.
        if(isspace(character))
        {	
			// preskoceni vice bilych znaku za sebou
			if(index == 0)
			{
				continue;
			}

            s[index] = '\0';
            return index;
        }
		
		// Nacitani konci take pri dosazeni konce souboru.
        if(character == EOF)
        {
            s[index] = '\0';
            return character;
        }

        s[index] = character;

        index++;
    }

    s[index - 1] = '\0';
	
	// chybove hlaseni
    if(overflow == false)
    {
        overflow = true;
        fprintf(stderr, "CHYBA: Prekrocni maximalniho poctu znaku slova.\n");
    }
	
	// zahozeni zbytku slova
    while(!isspace(character = fgetc(f)))
    {
        if(character == EOF)
        {
            return character;
        }
    }

    return max - 1;
}
