/**
 * tail2.cc
 * Reseni IJC-DU2, priklad 2), 20.4. 2019
 * Autor: Dominik Nejedly, FIT
 * Prelozeno: gcc 7.4.0
 * Modul s funkci se zdrojovym kodem programu tail2
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

void printFromN(istream &file, int lineNumber);
void printLastN(istream &file, int lineNumber);

int main(int argc, char **argv)
{
	std::ios::sync_with_stdio(false);	// Zpusobuje neuvolneni pameti (memory leaks).
	
    int lineNumber = -1;	        // Promenna oznacujici pocet radku k vytisteni, nebo cislo radku, od ktereho se ma vypisovat.
    bool printFromLine = false;	    // Promenna oznacujici, zdali se bude tisknout od daneho radku, nebo se vypise dany pocet radku.
    char *endPtr = NULL;	        // pomocny ukazatel
    string selector;	            // ulozeni prepinace
    char *fileName = NULL;	        // ukazatel na jmeno souboru

	// test poctu argumentu
    if(argc > 4)
    {
        cerr << "CHYBA: Zadan chybny pocet argumentu." << endl;
        return 1;
    }
	
	// prochazeni argumentu a jejich testovani
    for(int i = 1; i < argc; i++)
    {
        selector = argv[i];
		
		// test na prepinac
        if(selector == "-n" && i < (argc - 1))
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
                cerr << "CHYBA: Na " << i + 1 << " pozici zadan neplatny argument \"" << argv[i + 1] << "\"." << endl;
                return 2;
            }

            i++;
        }
        else
        {
            int lineNumberHelp = strtol(argv[i], &endPtr, 10);	// pomocna promenna

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
                        cerr << "CHYBA: Na " << i << " pozici zadan neplatny argument \"" << argv[i] << "\"." << endl;
                        return 2;
                    }

                    lineNumber = -lineNumberHelp;
                }
                else
                {
                    cerr << "CHYBA: Na " << i << " pozici zadan neplatny argument \"" << argv[i] << "\"." << endl;
                    return 2;
                }
            }
        }
    }

	// Pocet radku nebyl zadan.
    if(lineNumber == -1)
    {
        lineNumber = 10;    // implicitni hodnota
    }

    ifstream file;

	// Pokud byl zadan vstupni soubor, dojde k jeho otevreni.
    if(fileName != NULL)
    {
		file.open (fileName);

        if(!file.is_open())
        {
            cerr << "CHYBA: Soubor \"" << fileName << " se nepodarilo otevrit." << endl;
            return 3;
        }
    }

	// tisk od urciteho radku, nebo tisk daneho poctu radku
    if(printFromLine == true)
    {
        if(fileName != NULL)
        {
            // nacitani dat ze zadaneho souboru
            printFromN(file, lineNumber);
        }
        else
        {
			// nacitani vstupnich dat ze standardniho vstupu
            printFromN(cin, lineNumber);
        }
    }
    else
    {
        if(fileName != NULL)
        {
            printLastN(file, lineNumber);
        }
        else
        {
            printLastN(cin, lineNumber);
        }
    }

	// uzavreni souboru
    if(fileName != NULL)
    {
        file.close();
    }

    return 0;
}

/**
 * Funkce tiskne radky ze souboru na standardni vystup od konkretniho radku zadaneho parametrem lineNumber.
 */
void printFromN(istream &file, int lineNumber)
{
    string line;

    lineNumber--;

	// nacitani radku ze souboru
    while(getline(file, line))
    {
		// preskoceni daneho poctu prvnich radku a nasledny tisk
        if( lineNumber == 0)
        {
            cout << line << endl;
        }
        else
        {
            lineNumber--;
        }
    }
}

 /**
 * Funkce tiskne zadany pocet poslednich radku souboru na standardni vystup.
 */
void printLastN(istream &file, int lineNumber)
{
	// zavedeni kontejneru
    std::queue<string> buffer;

    string line;

	// nacitani pozadovaneho poctu radku ze souboru a jejich vkladani do kontejneru
    for(int i = 0; i < lineNumber && getline(file, line); i++)
    {
        buffer.push (line);
    }

	// po nacteni daneho poctu se vdy pri nacteni noveho radku ten nejstarsi uvolni
    while(getline(file, line))
    {
        buffer.push (line);
        buffer.pop();
    }

	// vypis radku a uvolneni kontejneru
    while(!buffer.empty())
    {
        cout << buffer.front() << endl;
        buffer.pop();
    }
}
