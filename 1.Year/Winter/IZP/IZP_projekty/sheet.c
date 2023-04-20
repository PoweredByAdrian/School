/*******
********  Projekt 1 - Prace s textem
********  Autor: Adrian Horvath, xhorva14@stud.fit.vutbr.cz
********  Popis projektu: Cílem projektu je vytvořit program, který bude implementovat základní operace tabulkových procesorů. 
********  Vstupem programu budou textová data, zadání operací bude prostřednictvím argumentů příkazové řádky 
********  a svůj výsledek bude program vypisovat na výstup.
*******/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINESIZE 10240
#define MAX_COLSIZE 100


char *append_char(char *retazec, char c)
{
    int len = strlen(retazec);
    
    retazec[len + 1] = retazec[len];
    retazec[len] = c;
    return retazec;
}

//nacitanie delimiterov
void load_delim(int argc, char **argv, char *delimiter)
{   
    if (argc >= 2)
    {
        if ((argc >= 3) && (strncmp(argv[1], "-d", 3) == 0))
        {
            strcpy(delimiter, argv[2]);
        }
    }
}

//zmenenie vsetkych delimiterov na delimiter[0]
void change_delims(int argc, char **argv, char *riadok, char *delimiter)
{
    if (argc > 2)
    {
        if ((strncmp(argv[1], "-d", 3) == 0))
        {
            for (unsigned int i = 1; i <= strlen(argv[2]); i++)
            {
                for (unsigned int j = 0; j < strlen(riadok); j++)
                {
                    if (riadok[j] == delimiter[i])
                        riadok[j] = delimiter[0];
                }
            }
        }
    }
}

int no_delims(char *riadok, char *delimiter)   //vracia pocet delimitrov
{
    int no_delim = 0;
    for (unsigned int j = 0; j < strlen(riadok); j++)
    {
        if (riadok[j] == delimiter[0])
        {
            no_delim++;
        }
    }
    return no_delim;
}

int round_str(char *str_cislo)      //vracia intiger z desatinneho cisla po zaokruhleni
{
    char *end;
    double desatinne_cislo = strtod(str_cislo, &end);
    return (int)(desatinne_cislo + 0.5);
}

int double_to_int(char *str_cislo)   //vracia intiget z desatinneho cisla
{
    char *end;
    double desatinne_cislo = strtod(str_cislo, &end);

    return (int)(desatinne_cislo);
}

bool cislo(char *cislo)   //vracia true ak je cislo
{
    bool je_cislo = false;
    for (unsigned int i = 0; i < strlen(cislo); i++)
    {
        if (isdigit(cislo[i]))
        {
            je_cislo = true;
        }
        else
        {
            je_cislo = false;
        }
    }
    return je_cislo;
}


//*******************Příkazy pro úpravu tabulky*******************

//irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
void irow(int aktualny_riadok, int R, char *delimiter, int pocet_delimitrov, char *upraveny_riadok)
{   
    char novy_riadok[MAX_LINESIZE + pocet_delimitrov];
    novy_riadok[0] = '\0';
    if(aktualny_riadok == R)
    {
        for (int i = 0; i < pocet_delimitrov; i++)
            append_char(novy_riadok, delimiter[0]);
        append_char(novy_riadok, '\n');

        for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
            append_char(novy_riadok, upraveny_riadok[i]); 
        strcpy(upraveny_riadok, novy_riadok);
    }
    else
        strcpy(upraveny_riadok, upraveny_riadok);
}

//arow - přidá nový řádek tabulky na konec tabulky (append-row).
void arow(char *delimiter, int pocet_delimitrov, char* upraveny_riadok)
{
    append_char(upraveny_riadok, '\n');
    for (int i = 0; i < pocet_delimitrov; i++)
        append_char(upraveny_riadok, delimiter[0]);
}

//drow R - odstraní řádek číslo R > 0 (delete-row).
void drow(int aktualny_riadok, int R, char* upraveny_riadok)
{
    char novy_riadok[MAX_LINESIZE];
    novy_riadok[0] = '\0';
    if(aktualny_riadok == R)
        strcpy(upraveny_riadok, novy_riadok) ;
    else
        strcpy(upraveny_riadok, upraveny_riadok);    
}

//drows N M - odstraní řádky N až M (N <= M). V případě N=M se příkaz chová stejně jako drow N.
void drows(int aktualny_riadok, int N, int M, char* upraveny_riadok)
{
    for (int i = N; i <= M; i++)
    {
        int row = i;
        drow(aktualny_riadok, row, upraveny_riadok);
    }
}

//icol C - vloží prázdný sloupec před sloupec daný číslem C.
void icol(int C, char *upraveny_riadok, char *delimiter)
{
    int count = 0;
    char novy_riadok[MAX_LINESIZE + 1];
    novy_riadok[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (C - 1 == count)
        {
            append_char(novy_riadok, delimiter[0]);
            count++;
        }
        if (upraveny_riadok[i] == delimiter[0])
            count++;
        if (upraveny_riadok[i] == '\n')
            count = 0;
        append_char(novy_riadok, upraveny_riadok[i]);  
    }
    strcpy(upraveny_riadok, novy_riadok) ;
}

//acol - přidá prázdný sloupec za poslední sloupec.
void acol(char *upraveny_riadok, char *delimiter)
{
    char novy_riadok[MAX_LINESIZE + 1];
    novy_riadok[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (upraveny_riadok[i] == '\n')
        {
            append_char(novy_riadok, delimiter[0]);
            append_char(novy_riadok, '\n');
        }
        else
            append_char(novy_riadok, upraveny_riadok[i]);        
    }
    strcpy(upraveny_riadok, novy_riadok) ;
}

//dcol C - odstraní sloupec číslo C
void dcol(int C, char *upraveny_riadok, char *delimiter)
{
    char novy_riadok[MAX_LINESIZE];
    novy_riadok[0] = '\0';
    int counts = 1;
    bool print;
    if (C == 1)
        print = false;
    else
        print = true;
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (upraveny_riadok[i] == '\n')
            print = true;
        
        if (upraveny_riadok[i] == delimiter[0])
        {
            if (C != 1)
                print = true;
            if (C - 1 == counts)
                print = false;
            if (print)
                append_char(novy_riadok, upraveny_riadok[i]);
            if (C == 1)
                print = true;
            counts++;    
        }
        else if ((print) || ((upraveny_riadok[i] == '\n') && (upraveny_riadok[i-1] == '\0')))
            append_char(novy_riadok, upraveny_riadok[i]);        
    }
    strcpy(upraveny_riadok, novy_riadok);
}

//dcols N M - odstraní sloupce N až M (N <= M). V případě N=M se příkaz chová stejně jako dcol N.
void dcols(int N, int M, char *upraveny_riadok, char *delimiter)
{
    for (int i = M ; i >= N; i--)
    {
        int C = i;
        dcol(C, upraveny_riadok, delimiter);
    }
}


//*******************Příkazy pro zpracování dat*******************

//cset C STR - do buňky ve sloupci C bude nastaven řetězec STR.
void cset(int C, char *str, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    bool print = true;
    int StrSize = strlen(str);
    char novy_riadok[MAX_LINESIZE];
    novy_riadok[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {

        if (C == count && C == 1)
        {
            for (int j = 0; j < StrSize; j++)
                append_char(novy_riadok, str[j]);
            count++;
            print = false;
        }
        if (upraveny_riadok[i] == delimiter[0])
        {
            count++;
            print = true;
        }
        if (C == count && C != 1)
        {
            append_char(novy_riadok, delimiter[0]);
            for (int j = 0; j < StrSize; j++)
                append_char(novy_riadok, str[j]);
            count++;
            print = false;
        } 
        if (upraveny_riadok[i] == '\n')
            append_char(novy_riadok, '\n');
        else if (print)
            append_char(novy_riadok, upraveny_riadok[i]);
    }
    strcpy(upraveny_riadok, novy_riadok);
}

//tolower C - řetězec ve sloupci C bude převeden na malá písmena.
void tlower(int C, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    bool print = true;
    char novy_riadok[MAX_LINESIZE];
    novy_riadok[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (C == count)
        {   
            append_char(novy_riadok, tolower(upraveny_riadok[i]));
            print = false;
        }
        else if (print)
            append_char(novy_riadok, upraveny_riadok[i]);
        if (upraveny_riadok[i] == delimiter[0])
        {
            count++;
            print = true;
        }
    }
    strcpy(upraveny_riadok, novy_riadok);
}

//toupper C - řetězec ve sloupce C bude převeden na velká písmena.
void tupper(int C, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    bool print = true;
    char novy_riadok[MAX_LINESIZE];
    novy_riadok[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (C == count)
        {    
            append_char(novy_riadok, toupper(upraveny_riadok[i]));
            print = false;
        }
        else if (print)
            append_char(novy_riadok, upraveny_riadok[i]);
        if (upraveny_riadok[i] == delimiter[0])
        {
            count++;
            print = true;
        }
    }
    strcpy(upraveny_riadok, novy_riadok);
}

//round C - ve sloupci C zaokrouhlí číslo na celé číslo.
void roundc(int C, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    bool print = true;
    char str_na_cislo[MAX_COLSIZE];
    str_na_cislo[0] = '\0';
    char novy_riadok[MAX_COLSIZE];
    novy_riadok[0] = '\0';
    int zaokruhlene_cislo = 0 ;
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (C == count && upraveny_riadok[i] != delimiter[0])
        {
            if ((isdigit(upraveny_riadok[i])) || (upraveny_riadok[i] == '.'))
            {
                if ((isdigit(upraveny_riadok[i-1])) && (upraveny_riadok[i] == '.') && (isdigit(upraveny_riadok[i+1])))
                    append_char(str_na_cislo, upraveny_riadok[i]);
                else
                    append_char(str_na_cislo, upraveny_riadok[i]);
                if ((isdigit(upraveny_riadok[i+1]) ==0) && (upraveny_riadok[i+1] != '.'))
                {
                    zaokruhlene_cislo = round_str(str_na_cislo);
                    sprintf(str_na_cislo, "%d", zaokruhlene_cislo);
                    for (unsigned int j = 0; j < strlen(str_na_cislo); j++)
                        append_char(novy_riadok, str_na_cislo[j]);
                    str_na_cislo[0] = '\0';
                }    
            }
            else if (upraveny_riadok[i] == '\n')
                append_char(novy_riadok, '\n');
            else
                append_char(novy_riadok, upraveny_riadok[i]);
            print = false;
        }

        else if (print)
            append_char(novy_riadok, upraveny_riadok[i]);
        if (upraveny_riadok[i] == delimiter[0])
        {
            if (print == false)
                append_char(novy_riadok, delimiter[0]);
            count++;
            print = true;
        }
    }
    strcpy(upraveny_riadok, novy_riadok);  
}

//int C - odstraní desetinnou část čísla ve sloupci C.
void int_C(int C, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    bool print = true;
    char str_na_cislo[MAX_COLSIZE];
    str_na_cislo[0] = '\0';
    char novy_riadok[MAX_COLSIZE];
    novy_riadok[0] = '\0';
    int cele_cislo = 0 ;
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (C == count && upraveny_riadok[i] != delimiter[0])
        {
            if ((isdigit(upraveny_riadok[i])) || (upraveny_riadok[i] == '.'))
            {
                if ((isdigit(upraveny_riadok[i-1])) && (upraveny_riadok[i] == '.') && (isdigit(upraveny_riadok[i+1])))
                    append_char(str_na_cislo, upraveny_riadok[i]);
                else 
                    append_char(str_na_cislo, upraveny_riadok[i]);
                if ((isdigit(upraveny_riadok[i+1]) ==0) && (upraveny_riadok[i+1] != '.'))
                {
                    cele_cislo = double_to_int(str_na_cislo);
                    sprintf(str_na_cislo, "%d", cele_cislo);
                    for (unsigned int j = 0; j < strlen(str_na_cislo); j++)
                        append_char(novy_riadok, str_na_cislo[j]);
                    str_na_cislo[0] = '\0';
                }    
            }
            else if (upraveny_riadok[i] == '\n')
                append_char(novy_riadok, '\n');
            else
                append_char(novy_riadok, upraveny_riadok[i]);
            print = false;
        }
        else if (print)
            append_char(novy_riadok, upraveny_riadok[i]);
        if (upraveny_riadok[i] == delimiter[0])
        {
            if (print == false)
                append_char(novy_riadok, delimiter[0]);
            count++;
            print = true;
        }
    }
    strcpy(upraveny_riadok, novy_riadok);
}

//copy N M - přepíše obsah buněk ve sloupci M hodnotami ze sloupce N.
void copy(int N, int M, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    char stringN[MAX_COLSIZE + 1] ;
    stringN[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (N == count)
        {
            if ((upraveny_riadok[i] != delimiter[0]) && (upraveny_riadok[i] != '\n'))
                append_char(stringN, upraveny_riadok[i]);   
        }
        if (upraveny_riadok[i] == delimiter[0])
            count++;
    } 
    cset(M, stringN, upraveny_riadok, delimiter);
}

//swap N M - zamění hodnoty buněk ve sloupcích N a M.
void swap(int N, int M, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    char stringN[MAX_COLSIZE + 1] ;
    stringN[0] = '\0';
    char stringM[MAX_COLSIZE + 1];
    stringM[0] = '\0';
    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (N == count)
        {
            if ((upraveny_riadok[i] != delimiter[0]) && (upraveny_riadok[i] != '\n'))
                append_char(stringN, upraveny_riadok[i]);   
        }
        else if (M == count)
        {
            if ((upraveny_riadok[i] != delimiter[0]) && (upraveny_riadok[i] != '\n'))
                append_char(stringM, upraveny_riadok[i]);    
        }
        if (upraveny_riadok[i] == delimiter[0])
            count++;
    }
    cset(N, stringM, upraveny_riadok, delimiter);
    cset(M, stringN, upraveny_riadok, delimiter);
}

//move N M - přesune sloupec N před sloupec M.
void move(int N, int M, char *upraveny_riadok, char *delimiter)
{
    int count = 1;
    char stringN[MAX_COLSIZE + 1] ;
    stringN[0] = '\0';
    char stringM[MAX_COLSIZE + 1] ;
    stringM[0] = '\0';
    bool print = false;

    for (unsigned int i = 0; i < strlen(upraveny_riadok); i++)
    {
        if (M > 1)
        {
            if (M - 1 == count)
            {
                if ((upraveny_riadok[i] != delimiter[0]) && (upraveny_riadok[i] != '\n'))
                    append_char(stringM, upraveny_riadok[i]);
            }
        }
        else
        {
            if (M == count)
            {
                if ((upraveny_riadok[i] != delimiter[0]) && (upraveny_riadok[i] != '\n'))
                    append_char(stringM, upraveny_riadok[i]);
            }
        }
            
        if (N == count)
        {
            
            if ((upraveny_riadok[i] != delimiter[0]) && (upraveny_riadok[i] != '\n'))
                append_char(stringN, upraveny_riadok[i]);
            print =true;   
        }

        if (upraveny_riadok[i] == delimiter[0])
            count++;
    }
    if ((print) && (M > 1)) 
        append_char(stringM, delimiter[0]);
    else if (print)
        append_char(stringN, delimiter[0]);
    if (M > 1)
    {
        for (unsigned int j = 0; j < strlen(stringN); j++)
            append_char(stringM, stringN[j]);
    }
    else
    {
        for (unsigned int j = 0; j < strlen(stringM); j++)
            append_char(stringN, stringM[j]);
    }
    
    
    if (M > 1)
    {
        cset(M - 1, stringM, upraveny_riadok, delimiter);
        if (N < M)
            dcol(N, upraveny_riadok, delimiter);
        else
            dcol(N + 1, upraveny_riadok, delimiter);
    }
    if (M == 1)
    {
        cset(M, stringN, upraveny_riadok, delimiter);
        dcol(N + 1, upraveny_riadok, delimiter);  
    }
    
}


//*******************Příkazy pro selekce řádků********************

//rows N M - procesor bude zpracovávat pouze řádky N až M včetně (N <= M).
bool rows(char *N, char *M, int aktualny_riadok, int last_row)
{
    bool n_number = cislo(N);
    bool m_number = cislo(M);

    if ((n_number) && (m_number))
    {
        int NN =atoi(N);
        int MM = atoi(M);
        if ((NN <= aktualny_riadok) && (aktualny_riadok <= MM))
            return true;
        else
            return false;  
    }
    else if (n_number) 
    {
       int NN =atoi(N);
        if ((NN <= aktualny_riadok) && (aktualny_riadok <= aktualny_riadok + 1))
            return true;
        else
            return false;
    }
    else if (N[0] == '-')
    {
        if ((last_row <= aktualny_riadok) && (aktualny_riadok <= last_row))
            return true;
        else
            return false;  
    }
    else
        return false; 
}

//beginswith C STR - procesor bude zpracovávat pouze ty řádky, jejichž obsah buňky ve sloupci C začíná řetězcem STR.
bool beginswitch(char *str, char* upraveny_riadok)
{
    if (strncmp(str, upraveny_riadok, strlen(str)) == 0)
        return true;
    else
        return false;
}

//contains C STR - procesor bude zpracovávat pouze ty řádky, jejichž buňky ve sloupci C obsahují řetězec STR.
bool contains(char *str, char* upraveny_riadok)
{
    if (strstr(upraveny_riadok, str))
        return true;
    else
        return false;  
}


//************************Volanie funkcii*************************

int uprava_stlpcov(int argc, char **argv, char *upraveny_riadok, int no_row, int no_delim, char *delimiter, bool end)
{
     for (int i = 1; i < argc; i++)
    {
            if (strcmp(argv[i], "icol") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                icol(C, upraveny_riadok, delimiter);
                if (no_row == 1)
                    no_delim ++;
            }
            if (strcmp(argv[i], "acol") == 0)
            {
                acol(upraveny_riadok, delimiter);
                if (no_row == 1)
                    no_delim ++;
                if (end)
                    append_char(upraveny_riadok, delimiter[0]);
                
            }
            if (strcmp(argv[i], "dcol") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                dcol(C, upraveny_riadok, delimiter);
                if (no_row == 1)
                    no_delim --;
            }
            if (strcmp(argv[i], "dcols") == 0)
            {
                int N = strtol(argv[i + 1], NULL, 10);
                int M = strtol(argv[i + 2], NULL, 10);
                dcols(N, M, upraveny_riadok, delimiter);
            }
    }
    return(no_delim);
}

int uprava_riadkov(int argc, char **argv, int no_row, char *delimiter,int no_delim, char* upraveny_riadok, bool end, int last_row)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "irow") == 0)
        {
            int row = strtol(argv[i + 1], NULL, 10);
            irow(no_row, row, delimiter, no_delim, upraveny_riadok);
        }
        if ((strcmp(argv[i], "arow") == 0) && (end))
        {
            if (end)
            {
              arow(delimiter, no_delim, upraveny_riadok);
              last_row++;        
            }          
        } 
        if (strcmp(argv[i], "drow") == 0)
        {
            int row = strtol(argv[i + 1], NULL, 10);
            drow(no_row, row, upraveny_riadok);
        }
        if (strcmp(argv[i], "drows") == 0)
        {
            int N = strtol(argv[i + 1], NULL, 10);
            int M = strtol(argv[i + 2], NULL, 10);
            drows(no_row, N, M, upraveny_riadok);
        }
    } 
    return last_row;  
}

void spracovanie_dat(int argc, char **argv, char *upraveny_riadok, char *delimiter)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "cset") == 0)
        {
            int C = strtol(argv[i + 1], NULL, 10);
            char *str = argv[i + 2];
            cset(C, str, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "tolower") == 0)
        {
            int C = strtol(argv[i + 1], NULL, 10);
            tlower(C, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "toupper") == 0)
        {
            int C = strtol(argv[i + 1], NULL, 10);
            tupper(C, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "copy") == 0)
        {
            int N = strtol(argv[i + 1], NULL, 10);
            int M = strtol(argv[i + 2], NULL, 10);
            copy(N, M, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "swap") == 0)
        {
            int N = strtol(argv[i + 1], NULL, 10);
            int M = strtol(argv[i + 2], NULL, 10);
            swap(N, M, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "move") == 0)
        {
            int N = strtol(argv[i + 1], NULL, 10);
            int M = strtol(argv[i + 2], NULL, 10);
            move(N, M, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "round") == 0)
        {
            int C = strtol(argv[i + 1], NULL, 10);
            roundc(C, upraveny_riadok, delimiter);
        }
        if (strcmp(argv[i], "int") == 0)
        {
            int C = strtol(argv[i + 1], NULL, 10);
            int_C(C, upraveny_riadok, delimiter);
        }
    }
}

bool selekcia_riadkov(int argc, char **argv,  char *upraveny_riadok, int no_row, int last_row)
{
    bool selekcia_riadku = true;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "rows") == 0)
        {
            char *N = (argv[i + 1]);
            char *M = (argv[i + 2]);
            selekcia_riadku = rows(N, M, no_row, last_row);
        }
        if (strcmp(argv[i], "beginswitch") == 0)
        {
            char *str = argv[i + 1];
            selekcia_riadku = beginswitch(str, upraveny_riadok);
        }
        if (strcmp(argv[i], "contains") == 0)
        {
            char *str = argv[i + 1];
            selekcia_riadku = contains(str, upraveny_riadok);
        }
    }
    return selekcia_riadku;
}


//kontola chyb vo volaniach
bool chyby_uprava_stlpcov(int argc, char **argv)
{
    bool error = true;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "icol") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *C = argv[i + 1];
            if (cislo(C) == false)
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(C) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            } 
        }
        if (strcmp(argv[i], "dcol") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            } 
            char *C = argv[i + 1];
            if (cislo(C) == false )
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(C) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
             
        }
        if (strcmp(argv[i], "dcols") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            } 
            char *N = argv[i + 1];
            char *M = argv[i + 2];
            if (cislo(N) == false ) 
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (cislo(M) == false ) 
            {
                fprintf(stderr, " %s neni relevantny argument ", argv[i+2]);
                error = false;
            }
            else if (atoi(M) < atoi(N))
            {
                fprintf(stderr, "Cislo stlpca M menise ako N ");
                error = false;
            }
            else if (atoi(N) < 1)
            {
                fprintf(stderr, "Cislo stlpca mensie ako 1 ");
                error = false;
            }
            
        }
    }
    return(error);
}

bool chyby_uprava_riadokv(int argc, char **argv)
{
    bool error = true;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "irow") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *R = argv[i + 1];
            if (cislo(R) == false)
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(R) < 1)
            {
                fprintf(stderr, "Cislo riadku R mensie ako 1 ");
                error = false;
            }
              
        }
        if (strcmp(argv[i], "drow") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *R = argv[i + 1];
            if (cislo(R) == false )
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(R) < 1)
            {
                fprintf(stderr, "Cislo riadku R mensie ako 1 ");
                error = false;
            }
              
        }
        if (strcmp(argv[i], "drows") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *N = argv[i + 1];
            char *M = argv[i + 2];
            if (cislo(N) == false ) 
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (cislo(M) == false ) 
            {
                fprintf(stderr, " %s relevantny argument ", argv[i+2]);
                error = false;
            }
            else if (atoi(M) < atoi(N))
            {
                fprintf(stderr, "Cislo riadku M menise ako N ");
                error = false;
            }
            else if (atoi(N) < 1)
            {
                fprintf(stderr, "Cislo riadka N mensie ako 1 ");
                error = false;
            }
             
        }
    }
    return(error);
}

bool chyby_spracovanie_dat(int argc, char **argv)
{
    bool error = true;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "cset") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *C = argv[i + 1];
            char *str = argv[i + 2];
            if (cislo(C) == false )
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(C) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            else if (strlen(str) > MAX_COLSIZE)
            {
                fprintf(stderr, "velkost str vacsi ako 100 ");
                error = false;
            } 
        }
        if (strcmp(argv[i], "tolower") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *C = argv[i + 1];
            if (cislo(C) == false )
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(C) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            
        }
        if (strcmp(argv[i], "toupper") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *C = argv[i + 1];
            if (cislo(C) == false )
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(C) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            
        }
        if (strcmp(argv[i], "copy") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *N = argv[i + 1];
            char *M = argv[i + 2];
            if (cislo(N) == false )
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(N) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            else if (cislo(M) == false )
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(M) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            
        }
        if (strcmp(argv[i], "swap") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *N = argv[i + 1];
            char *M = argv[i + 2];
            if (cislo(N) == false )
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(N) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            else if (cislo(M) == false )
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(M) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            
        }
        if (strcmp(argv[i], "move") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *N = argv[i + 1];
            char *M = argv[i + 2];
            if (cislo(N) == false )
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(N) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            }
            else if (cislo(M) ==0 )
            {
                fprintf(stderr, "%s relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(M) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            } 
        }
        if (strcmp(argv[i], "round") == 0)
        {
            if (argc < i + 2)
                {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
                }
            char *C = argv[i + 1];
            if (cislo(C) == false )
                {
                    fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                    error = false;
                }
                else if (atoi(C) < 1)
                {
                    fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                    error = false;
                }
                
        }
        if (strcmp(argv[i], "int") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
                break;
            }
            char *C = argv[i + 1];
            if (cislo(C) == false )
            {
                fprintf(stderr, "%s neni relevantny argument ", argv[i+1]);
                error = false;
            }
            else if (atoi(C) < 1)
            {
                fprintf(stderr, "Cislo stlpca C mensie ako 1 ");
                error = false;
            } 
        }
    }
    return error;
}

bool chyby_selekcia_riadkov(int argc, char **argv)
{
    bool error = true;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "rows") == 0)
        {
            if (argc < i + 3)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
            }
            char *N = (argv[i + 1]);
            char *M = (argv[i + 2]);
            if (cislo(N) == false)
            {
                if (strlen(N) != 1)
                {
                    fprintf(stderr, "N neni kladne cislo ani - ");
                    error = false;
                }
                else if (N[0] != '-')
                {
                    fprintf(stderr, "N neni klade cislo ani - ");
                    error = false;
                }
            }
            else if (atoi(N) < 1)
            {
                fprintf(stderr, "Cislo riadka N mensie ako 1 ");
                error = false;
            }
            if (cislo(M) == false)
            {
                if (strlen(M) != 1 )
                {
                    fprintf(stderr, "N neni kladne cislo ani - ");
                    error = false;
                }
                else if (M[0] != '-')
                {
                    fprintf(stderr, "N neni kladne cislo ani - ");
                    error = false;
                }  
            }
            else if (atoi(M) < 1)
            {
                fprintf(stderr, "Cislo riadka M mensie ako 1 ");
                error = false;   
            }
            else if (atoi(M) < atoi(N))
            {
            fprintf(stderr, "Cislo riadku M menise ako N ");
            error = false;
            }
        }
        if (strcmp(argv[i], "beginswitch") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
            }
            char *str = argv[i + 1];
            if (strlen(str) > MAX_COLSIZE)
            {
                fprintf(stderr, "Argument vacsi ako 100 ");
                error = false;
            }
        }
        if (strcmp(argv[i], "contains") == 0)
        {
            if (argc < i + 2)
            {
                fprintf(stderr, "Malo argumentov");
                error = false;
            }
            char *str = argv[i + 1];
            if (strlen(str) > MAX_COLSIZE)
            {
                fprintf(stderr, "Argument vacsi ako 100 ");
                error = false;
            }
        }
    }
    return error;
}

bool ERROR(int argc, char **argv)
{
    bool Serror = chyby_uprava_stlpcov(argc, argv);
    bool Rerror = chyby_uprava_riadokv(argc, argv);
    bool SRerror = chyby_selekcia_riadkov(argc, argv);
    bool Derror = chyby_spracovanie_dat(argc, argv);
    if (argc == 2)
    {
        if (strncmp(argv[1], "-d", 3) == 0)
        {
            fprintf(stderr, "Chyba delimiter");
            return true;
        }
        else
            return false; 
    }
    else if (Serror && Rerror && Derror && SRerror)
        return true;
    else
        return false;
}


int main(int argc, char **argv)
{
    char delimiter[MAX_COLSIZE];
    delimiter[0] = ' ';
    char riadok_tabulky[MAX_LINESIZE];
    int no_delim;
    int no_row = 1;
    char upraveny_riadok[MAX_LINESIZE];
    bool selekcia_riadku = true;
    bool end = false;
    int last_row = 0;
    if(ERROR(argc, argv))
    {
        load_delim(argc, argv, delimiter);
        while ((fgets(riadok_tabulky, MAX_LINESIZE, stdin) != 0) || (fgets(riadok_tabulky, MAX_LINESIZE + 1, stdin) == 0)) //nacitanie tabulky po riadkoch
        { 
            if (strlen(riadok_tabulky) == 0)           
                fprintf(stderr,  "Prazdna tabulka");
            change_delims(argc, argv, riadok_tabulky, delimiter);
            if (no_row == 1)
                no_delim = no_delims(riadok_tabulky, delimiter); //pocet delimitrov je urceny podla prveho riadku 
            if (feof(stdin))
            {
                end = true;
                last_row = no_row;
            }
            upraveny_riadok[0] = '\0';
            for (unsigned int i = 0; i < strlen(riadok_tabulky); i++)
                append_char(upraveny_riadok, riadok_tabulky[i]); 
            no_delim = uprava_stlpcov(argc, argv, upraveny_riadok, no_row, no_delim, delimiter, end);
            last_row = uprava_riadkov(argc, argv, no_row, delimiter, no_delim, upraveny_riadok, end, last_row);
            selekcia_riadku = (selekcia_riadkov(argc, argv, upraveny_riadok, no_row, last_row));
            if (selekcia_riadku)
                spracovanie_dat(argc, argv, upraveny_riadok, delimiter);
            no_row++;
            printf("%s", upraveny_riadok);
            if (end)
                break;
        }
    }    
    return 0;
}