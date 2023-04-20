#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 1025






//Funkcia na pisanie poslednych N riadkov
int printLastN(FILE *f, int No_line, unsigned int Line_size)
{
	//allokacia pamate pre dany pocet riadkov 
    char *Lines_array = calloc(No_line * Line_size, 1);
    if(Lines_array == NULL)
    {
        fprintf(stderr, "ERROR: Allocation failed");
        return -1;
    }

    bool Line_size_overflow = true;
    int c;
    int l;      //dlzka ulozeneho riadku
    int index = 0;


	// nacitanie radku
    while(fgets(Lines_array + (Line_size * index), Line_size, f) != NULL)
    {
        //kontrola dlzky ulozeneho riadku
        if(strlen(Lines_array + (Line_size * index)) == (Line_size - 1))
        {	
            if(Lines_array[(Line_size * index) + Line_size - 2] != '\n')
            {	
                while((c = fgetc(f)) != '\n')
                {
                    if(c == EOF)
                        break;
                }
                //vlozenie \n na koniec skrateneho riadku
                Lines_array[(Line_size * index) + Line_size - 2] = '\n';
				
                if(Line_size_overflow)
                {
                    fprintf(stderr, "WARNING: Max length of line reached.\n");
                    Line_size_overflow = false;
                }
            }
        }
        index++;

		//nastavenie indexu na zaciatok pola
        if(index == No_line)
            index = 0;
    }

    for(int i = index; i < (index + No_line); i++)
    {
        printf("%s", Lines_array + ((i % No_line) * Line_size));
        l = strlen(Lines_array + ((i % No_line) * Line_size));
		
		//Doplnenie \n na koniec riadku kde neni
        if(l != 0 && Lines_array[((i % No_line) * Line_size) + l - 1] != '\n')
            printf("\n");
    }
    free(Lines_array);
    return 0;
}


//Funkcia na pisanie riadkov od urciteho cisla riadku
void printFromN(FILE *file, int No_line)
{
    int c;
    No_line--;
	//nevypisuje pokial neni na zadanom riadku
    while(No_line != 0 && (c = fgetc(file)) != EOF)
    {
        if(c == '\n')
            No_line--;
    }
	//pisanie riadkov
    while((c = fgetc(file)) != EOF)
        putchar(c);
}

int main(int argc, char *argv[])
{
 
    bool Print_mode = false; //false = from end | true = from begin
    int No_line = -1; // hodnota -n zo vstupu
    char *fileName = NULL;
    char *endPtr = NULL; // pomocny ukazatel pre strtol

    if(argc > 4)
    {
        fprintf(stderr, "ERROR: Too many arguments\n");
        return -1;
    }

    for(int i = 1; i < argc; i++)
    {

        if(strcmp(argv[i], "-n") == 0 && i < (argc - 1))
        {
            //nastavenie modu tlacenia
            if(argv[i + 1][0] == '+')
                Print_mode = true;
			//pretipovanie na cislo
            No_line = strtol(argv[i + 1], &endPtr, 10);
			//ABS vstupu
            if(No_line < 0)
                No_line = -No_line;
			// typova kontrola argumentu
            if(*endPtr != '\0')
            {
                fprintf(stderr, "ERROR: Incorrect argument entered.\n");
                return -1;
            }
            i++;
        }
        else
                fileName = argv[i];	
    }
	//Ak nebol zadany pocet riadkov nastavi sa implicitne na 10
    if(No_line == -1)
    {
        No_line = 10;
    }
	FILE *fPtr;

    if(fileName != NULL)
    {
		fPtr = fopen(fileName, "r");
        if(fPtr == NULL)
        {
            fprintf(stderr, "ERROR: File could not be open.\n");
            return -1;
        }
    }
    if(Print_mode == true)
    {
        if(fileName != NULL)
            printFromN(fPtr, No_line);
        else
            printFromN(stdin, No_line);
    }
    else
    {
        if(fileName != NULL)
        {
            if(printLastN(fPtr, No_line, MAX_LINE_SIZE))
            {
                fclose(fPtr);
                return -1;
            }
        }
        else
        {
            if(printLastN(stdin, No_line, MAX_LINE_SIZE))
                return -1;
        }
    }
    if(fileName != NULL)
        fclose(fPtr);

    return 0;
}



