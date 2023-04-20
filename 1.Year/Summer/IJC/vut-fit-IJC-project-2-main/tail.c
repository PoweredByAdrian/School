/**
 * tail.c
 * Reseni IJC-DU2, priklad 1), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul se zdrojovym kodem programu tail.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SUCCESS 0

#define PARAM_ERR 1
#define INTERNAL_ERR 99

#define LENGTH 1025     // maximalni delka radku

void printFromN (FILE *file, int lineNumber);
int printLastN(FILE *file, int lineNumber, unsigned int bufferSize);

int main(int argc, char *argv[])
{
    int lineNumber = -1;		// Promenna oznacujici pocet radku k vytisteni, nebo cislo radku, od ktereho se ma vypisovat.
    bool printFromLine = false;	// Promenna oznacujici, zdali se bude tisknout od daneho radku, nebo se vypise dany pocet radku.
    char *endPtr = NULL;		// pomocny ukazatel
    char *fileName = NULL;		// ukazatel na jmeno souboru

	// test poctu argumentu
    if(argc > 4)
    {
        fprintf(stderr, "CHYBA: Zadan chybny pocet argumentu.\n");
        return PARAM_ERR;
    }

	// prochazeni argumentu a jejich testovani
    for(int i = 1; i < argc; i++)
    {
		// test na prepinac
        if(strcmp(argv[i], "-n") == 0 && i < (argc - 1))
        {
            if(argv[i + 1][0] == '+')
            {
                printFromLine = true;
            }
			
            lineNumber = strtol(argv[i + 1], &endPtr, 10);
			
			// zmena znamenka
            if(lineNumber < 0)
            {
                lineNumber = -lineNumber;
            }

			// typova kontrola argumentu
            if(*endPtr != '\0')
            {
                fprintf(stderr, "CHYBA: Na %d. pozici zadan neplatny argument \"%s\".\n", i + 1, argv[i + 1]);
                return PARAM_ERR;
            }

            i++;
        }
        else
        {
            int lineNumberHelp = strtol(argv[i], &endPtr, 10);  // pomocna promenna
			
            if(*endPtr != '\0')
            {
                fileName = argv[i];
            }
            else
            {
				// Plati, pokud jiz nebyl nacten pocet radku.
                if(lineNumber == -1)
                {
					// Bez prepinace muze byt cislo zadano pouze za pomlckou.
                    if(argv[i][0] != '-')
                    {
                        fprintf(stderr, "CHYBA: Na %d. pozici zadan neplatny argument \"%s\".\n", i, argv[i]);
                        return PARAM_ERR;
                    }

                    lineNumber = -lineNumberHelp;
                }
                else
                {
                    fprintf(stderr, "CHYBA: Na %d. pozici zadan neplatny argument \"%s\".\n", i, argv[i]);
                    return PARAM_ERR;
                }
            }
        }
    }
	
	// Pocet radku nebyl zadan.
    if(lineNumber == -1)
    {
        lineNumber = 10;    // implicitni hodnota
    }

	FILE *filePtr;

    if(fileName != NULL)
    {
		// Pokud byl zadan vstupni soubor, dojde k jeho otevreni.
		filePtr = fopen(fileName, "r");

        if(filePtr == NULL)
        {
            fprintf(stderr, "CHYBA: Soubor \"%s\" se nepodarilo otevrit.\n", fileName);
            return INTERNAL_ERR;;
        }
    }
	
	// tisk od urciteho radku, nebo tisk daneho poctu radku
    if(printFromLine == true)
    {
        if(fileName != NULL)
        {
            // nacitani dat ze zadaneho souboru
            printFromN(filePtr, lineNumber);
        }
        else
        {
			// nacitani vstupnich dat ze standardniho vstupu
            printFromN(stdin, lineNumber);
        }
    }
    else
    {
        if(fileName != NULL)
        {
            if(printLastN(filePtr, lineNumber, LENGTH))
            {
                fclose(filePtr);
                return INTERNAL_ERR;
            }
        }
        else
        {
            if(printLastN(stdin, lineNumber, LENGTH))
            {
                return INTERNAL_ERR;
            }
        }
    }
	
	// uzavreni souboru
    if(fileName != NULL)
    {
        fclose(filePtr);
    }

    return SUCCESS;
}

/**
 * Funkce tiskne radky ze souboru na standardni vystup od konkretniho radku zadaneho parametrem lineNumber.
 */
void printFromN(FILE *file, int lineNumber)
{
    int character;

    lineNumber--;
	
	// preskoceni daneho poctu radku
    while(lineNumber != 0 && (character = fgetc(file)) != EOF)
    {
        if(character == '\n')
        {
            lineNumber--;
        }
    }
	
	// vypis zbyvajicich radku
    while((character = fgetc(file)) != EOF)
    {
        putchar(character);
    }
}

/**
 * Funkce tiskne zadany pocet poslednich radku souboru na standardni vystup.
 */
int printLastN(FILE *file, int lineNumber, unsigned int bufferSize)
{
	// alokace pameti pro dany pocet radku
    char *savedLines = calloc(lineNumber * bufferSize, 1);

    if(savedLines == NULL)
    {
        fprintf(stderr, "Alokace pameti pro nacitane radky selhala.");
        return INTERNAL_ERR;
    }

    int counter = 0;	                // index pro ulozeni aktualne nacteneho radku
    int character;		                // pomocna promenna pro znak
    int length;			                // pomocna promenna pro delku ulozeneho radku
    bool firstBufferOverflow = false;	// promenna nesouci informaci o prekoroceni maximalni delky radku
	
	// nacitani radku
    while(fgets(savedLines + (bufferSize * counter), bufferSize, file) != NULL)
    {
        if(strlen(savedLines + (bufferSize * counter)) == (bufferSize - 1))
        {	
			// test na prekroceni maximalni delky radku
            if(savedLines[(bufferSize * counter) + bufferSize - 2] != '\n')
            {	
				// zahozeni zbytku radku
                while((character = fgetc(file)) != '\n')
                {
                    if(character == EOF)
                    {
                        break;
                    }
                }

                savedLines[(bufferSize * counter) + bufferSize - 2] = '\n';
				
				// tisk chyboveho hlaseni (jen pri prvnim prekroceni)
                if(firstBufferOverflow == false)
                {
                    fprintf(stderr, "VAROVANI: Prekroceni maximalni mozne delky radky.\n");
                    firstBufferOverflow = true;
                }
            }
        }

        counter++;
		
		// reset indexu po pruchodu celym polem
        if(counter == lineNumber)
        {
            counter = 0;
        }
    }

	// vypis radku na stdandardni vystup
    for(int i = counter; i < (counter + lineNumber); i++)
    {
        printf("%s", savedLines + ((i % lineNumber) * bufferSize));

        length = strlen(savedLines + ((i % lineNumber) * bufferSize));
		
		// Odradkuje, pokud radky nekonci znakem '\n'.
        if(length != 0 && savedLines[((i % lineNumber) * bufferSize) + length - 1] != '\n')
        {
            printf("\n");
        }

    }
	
	// uvolneni alokovaneho pole
    free(savedLines);

    return SUCCESS;
}
