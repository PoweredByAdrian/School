#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define DEF_DELIM ' '
#define MAX_ARGUMENT_LENGTH 1000
#define MAX_DELIM_SIZE 1000



char *load_delim()
{
    if (strcmp(argv[1], "-d") == 0)
    {
        if (strlen(argv[2]) >1 )
        {
            //viac delimitrov
        }
        else
        {
            //jeden delimiter 
        }
        
    }
    else
    {
        //delim je " "
    }
    
        
}

char *lead_arguments()
{
    if (strcmp(argv[1], "-d") == 0)
    {
        //argumenty su v argv[3]
    }
    else
    {
        //argumenty su v argv[1]
    }
    
    
}

typedef struct cell {
    int *obsah;
    int velkost_bunky;
} Bunka;

typedef struct row {
    int pocet_buniek;
    Bunka *bunky;
} Riadok;

typedef struct table {
    int pocet_riadkov;
    Riadok *riadky;
} Tabulka;




Tabulka *load_table()
{
    int character;

    Tabulka *new_table;
    new_table = malloc(sizeof(Tabulka));
    if (new_tabulka == NULL)
        return NULL;
    
    int velkost_bunky = 1;
    new_table->pocet_riadkov = 1;

    Riadok *new_row;
    new_row = malloc(sizeof(Riadok))
    if (new_row == NULL)
        return NULL;
    new_table->riadky = new_row;

    Bunka *new_cell;
    new_cell = malloc(sizeof(Bunka));
    if (new_cell == NULL)
        return NULL;
    new_table->riadky->bunky = new_cell;
    
    new_table->riadky->pocet_buniek = 1;
    new_table->riadky->bunky->velkost_bunky = velkost_bunky;

    int *novy_obsah;
    novy_obsah = malloc(sizeof(int));
    if (novy_obsah == NULL)
        return NULL;
    new_table->riadky->bunky->obsah = novy_obsah;

    int akt_pocet_riadkov = 1;

    int akt_pocet_buniek = 1;

    character = fgetc(stdin);

    while (character != EOF)
    {
        
    }
    
}


















