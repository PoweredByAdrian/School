#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINESIZE 10240

void load_delim(int argc, char **argv, char *delimiter)
{
    if (argc >= 2)
    {

        if (strncmp(argv[1], "-d", 3))
        {
            fprintf(stderr, "Invalid argument");
        }

        if (argc >= 3)
        {
            strcpy(delimiter, argv[2]);
        }

        else
        {
            fprintf(stderr, "No delimiter supplied");
        }
    }
}

//delim0
void change_delims(char **argv, char *buffer, char *delimiter)
{
    for (unsigned int i = 1; i <= strlen(argv[2]); i++)
    {
        for (unsigned int j = 0; j < strlen(buffer); j++)
        {
            if (buffer[j] == delimiter[i])
            {
                buffer[j] = delimiter[0];
            }
        }
    }
}

//delims
int no_delims(char *buffer, char *delimiter)
{
    int no_delim = 0;
    for (unsigned int j = 0; j < strlen(buffer); j++)
    {
        if (buffer[j] == delimiter[0])
        {
            no_delim++;
        }
    }
    return no_delim;
}

//delims
int no_delims_row(char *buffer, char *delimiter)
{
    int no_delim_row = 0;
    for (unsigned int j = 0; j < strlen(buffer); j++)
    {
        if (buffer[j] == delimiter[0])
        {
            no_delim_row++;
        }
    }
    return no_delim_row;
}


char *append_char(char *cArr, char c)
{
    int len = strlen(cArr);
    
    cArr[len + 1] = cArr[len];
    cArr[len] = c;
    //printf ("|>%s<|", cArr);
    return cArr;
}

int round_str(char *str_cislo)
{
    char *end;
    double num_to_round = strtod(str_cislo, &end);
    //printf("|%f|", num_to_round);
    //printf("*%d*",(int)(num_to_round + 0.5));
    return (int)(num_to_round + 0.5);
    

}

int int_str(char *str_cislo)
{
    char *end;
    double num_to_int= strtod(str_cislo, &end);
    //printf("|%f|", num_to_round);
    //printf("*%d*",(int)(num_to_round + 0.5));
    return (int)(num_to_int);
    

}


//irow
//OK
void irow(int no_row, int row, char *delimiter, int no_delim)
{

    if (no_row == row - 1)
    {
        for (int i = 0; i < no_delim; i++)
        {
            printf("%c", delimiter[0]);
        }
        printf("\n");
    }
}

//arow
// OK
void arow(char *delimiter, int no_delim)
{
    printf("\n");
    for (int i = 0; i < no_delim; i++)
    {
        printf("%c", delimiter[0]);
    }
}

//drow
//OK
bool drow(int no_row, int row, char *buffer)
{
    if (no_row == row)
    {
        return false;
    }
    return true;
}

//acol ##přidá prázdný sloupec za poslední sloupec.###
//OK
void acol(char *buffer, char *delimiter)
{
    char *token;
    token = strtok(buffer, "\n");
    char *token2 = strcat(token, delimiter);
    buffer = strcat(token2, "\n");
}

//drows N M - odstraní řádky N až M (N <= M). V případě N=M se příkaz chová stejně jako drow N.
//OK
bool drows(int no_row, int N, int M)
{
    if (no_row >= N && no_row <= M)
    {
        return false;
    }
    return true;
}

//icol C - vloží prázdný sloupec před sloupec daný číslem C.
//OK
void icol(int cs, char *buffer, char *delimiter)
{
    int count = 1;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (cs == count)
        {
            printf("%c", delimiter[0]);
            count++;
        }

        if (buffer[i] == delimiter[0])
        {
            count++;
        }
        printf("%c", buffer[i]);
    }
}

//dcol C - odstraní sloupec číslo C.
//OK
void dcol(int cs, char *buffer, char *delimiter)
{
    int counts = 1;
    bool count;
    if (cs == 1)
    {
        count = false;
    }

    else
    {
        count = true;
    }

    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == '\n')
        {
            count = true;
        }
        {
            if (buffer[i] == delimiter[0])
            {
                if (cs != 1)
                {
                    count = true;
                }
                if (cs - 1 == counts)
                {
                    count = false;
                }

                else if (count)
                {
                    printf("%c", buffer[i]);
                }
                else if (cs == 1)
                {
                    count = true;
                }
                counts++;
            }
            else if (count)
            {
                printf("%c", buffer[i]);
            }
        }
    }
}

//dcols N M - odstraní sloupce N až M (N <= M). V případě N=M se příkaz chová stejně jako dcol N.
//OK
void dcols(int N, int M, char *buffer, char *delimiter)
{
    int counts = 1;
    bool count;
    if (N == 1)
    {
        count = false;
    }
    else
    {
        count = true;
    }

    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == '\n')
        {
            count = true;
        }
        if (buffer[i] == delimiter[0])
        {
            if (N != 1)
            {
                count = true;
            }
            if (N - 1 <= counts && M - 1 >= counts)
            {
                count = false;
            }

            else if (count)
            {
                printf("%c", buffer[i]);
            }
            else
            {
                count = true;
            }
            counts++;
        }
        else if (count)
        {
            printf("%c", buffer[i]);
        }
    }
}

//cset C STR - do buňky ve sloupci C bude nastaven řetězec STR.
//cset nepise posledny riadok 
void cset(int C, char *str, char *buffer, char *delimiter)
{
    int count = 1;
    bool print = true;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {

        if (C == count && C == 1)
        {
            printf("%s", str);
            count++;
            print = false;
        }

        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }

        if (C == count && C != 1)
        {
            printf("%c%s", delimiter[0], str);
            count++;
            print = false;
        } 

        
        if (buffer[i] == '\n')
        {
            printf("\n");
        }
        else if (print)
        {
            printf("%c", buffer[i]);
        }
    }
}

//tolower C - řetězec ve sloupci C bude převeden na malá písmena.
//tolower OK
void tlower(int C, char *buffer, char *delimiter)
{
    int count = 1;
    bool print = true;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (C == count)
        {
            printf("%c", tolower(buffer[i]));
            print = false;
        }
        else if (print)
        {
            printf("%c", buffer[i]);
        }
        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }
    }
}

//toupper C - řetězec ve sloupce C bude převeden na velká písmena.
//tolower OK
void tupper(int C, char *buffer, char *delimiter)
{
    int count = 1;
    bool print = true;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (C == count)
        {
            printf("%c", toupper(buffer[i]));
            print = false;
        }
        else if (print)
        {
            printf("%c", buffer[i]);
        }
        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }
    }
}

//round C - ve sloupci C zaokrouhlí číslo na celé číslo.
//round FIXME nepise nedesatinne cislo 
void roundc(int C, char *buffer, char *delimiter)
{
    int count = 1;
    bool print = true;
    char str_na_cislo[100];
    str_na_cislo[0] = '\0';
    bool x = true;
    bool p = false;
    int n = 0 ;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (C == count && buffer[i] != delimiter[0])
        {
            if ((isdigit(buffer[i])) || (buffer[i] == '.'))
            {
                if ((isdigit(buffer[i-1])) && (buffer[i] == '.') && (isdigit(buffer[i+1])))
                {
                append_char(str_na_cislo, buffer[i]);
                append_char(str_na_cislo, buffer[i+1]);
                //append_char(str_na_cislo, buffer[i+1]);
                n = round_str(str_na_cislo);
                x = false;
                
                }
                else if(x)
                { 
                append_char(str_na_cislo, buffer[i]);
                }
                if (x == false)
                {
                printf("%d",n);
                }
                /*else if (p)
                {
                    if (((buffer[i+1] != '.') || (isdigit(buffer[i+1]) != 1 ) ))
                    {
                        printf("%c", buffer[i]);
                    } 
                }*/
                
            }
            else if (buffer[i] == '\n')
            {
            printf("\n");
             //printf("+2");
            }
            else if (x == false)
            {
                printf("%d",n);
                 //printf("+3");
            }
            else
            {
                printf("%c", buffer[i]);
            }
            

            print = false;
            x = true;
            //p = false;
            //printf("**%d**", n);
            //printf("**%s**", str_na_cislo);
            
  
        }

        else if (print)
        {
            printf("%c", buffer[i]);
        }
        if (buffer[i] == delimiter[0])
        {
            if (print == false)
            {
                printf("%c",delimiter[0]);
            }
            
            count++;
            print = true;
        }
        
        //printf("%s", str_cislo);
    }
    




    /*int count = 1;
    bool print = true;
    double x;
    char *str;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (C == count)
        {
            x = strtod(buffer, &str);
            x = (int)(x + 0.5);
            if (buffer == str && x == 0)
            {
                printf("%d, %s", x, str);
            }
        }
        else if (print)
        {
            printf("%c", buffer[i]);
        }
        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }
    }*/
    
}

//
//int FIXME nepise nedesatinne cislo 
void int_C(int C, char *buffer, char *delimiter)
{
    int count = 1;
    bool print = true;
    char str_na_cislo[100];
    str_na_cislo[0] = '\0';
    bool x = true;
    int n = 0 ;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (C == count && buffer[i] != delimiter[0])
        {
            if ((isdigit(buffer[i])) || (buffer[i] == '.'))
            {
                if ((isdigit(buffer[i-1])) && (buffer[i] == '.') && (isdigit(buffer[i+1])))
                {
                append_char(str_na_cislo, buffer[i]);
                n = round_str(str_na_cislo);
                x = false;
                }
                else if(x)
                { 
                append_char(str_na_cislo, buffer[i]);
                
                }
                if (x == false)
                {
                printf("%d",n);
                }
                /*else if ((x) && ((buffer[i+1] != '.') || (isdigit(buffer[i+1]) == 0 ) ))
                {
                    printf("|%d|",n);
                }*/
                
            }
            else if (buffer[i] == '\n')
            {
            printf("\n");
             //printf("+2");
            }
            else if (x == false)
            {
                printf("%d",n);
                 //printf("+3");
            }
            else
            {
                printf("%c", buffer[i]);
            }
            

            print = false;
            x = true;
            //printf("**%d**", n);
            //printf("**%s**", str_na_cislo);
            
  
        }

        else if (print)
        {
            printf("%c", buffer[i]);
        }
        if (buffer[i] == delimiter[0])
        {
            if (print == false)
            {
                printf("%c",delimiter[0]);
            }
            
            count++;
            print = true;
        }
        
        //printf("%s", str_cislo);
    }
    




    /*int count = 1;
    bool print = true;
    double x;
    char *str;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (C == count)
        {
            x = strtod(buffer, &str);
            x = (int)(x + 0.5);
            if (buffer == str && x == 0)
            {
                printf("%d, %s", x, str);
            }
        }
        else if (print)
        {
            printf("%c", buffer[i]);
        }
        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }
    }*/
    
}

//copy N M - přepíše obsah buněk ve sloupci M hodnotami ze sloupce N.
//copy nepise posledny riadok
void copy(int N, int M, char *buffer, char *delimiter)
{
    int count = 1;
    char string[100] ;
    string[0] = '\0';
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (N == count)
        {
            if (buffer[i] != delimiter[0])
            {
                append_char(string, buffer[i]);
            }    
        }
        if (buffer[i] == delimiter[0])
        {
            count++;
        }
        
    } 
    cset(M, string, buffer, delimiter);
}

//swap N M - zamění hodnoty buněk ve sloupcích N a M.
//swap FIXME
void swap(int N, int M, char *buffer, char *delimiter)
{
    int count1 = 1;
    char stringN[100] ;
    stringN[0] = '\0';
    char stringM[100];
    stringM[0] = '\0';
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (N == count1)
        {
            if ((buffer[i] != delimiter[0]) && (buffer[i] != '\n'))
            {
                append_char(stringN, buffer[i]);
            }    
        }

        if (M == count1)
        {
            if ((buffer[i] != delimiter[0]) && (buffer[i] != '\n'))
            {
                append_char(stringM, buffer[i]);
            }    
        }

        if (buffer[i] == delimiter[0])
        {
            count1++;
        }
    }


    
    int count = 1;
    bool print = true;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }
        if (M == count && M == 1)
        {
            printf("%s*N", stringN);
            count++;
            print = false;
        }
        if (N == count && N == 1)
        {
            printf("%s*M", stringM);
            count++;
            print = false;
        }
        if (M == count && M != 1)
        {
            printf("%c%s*N", delimiter[0], stringN);
            count++;
            print = false;
        }
        if (N == count && N != 1)
        {
            printf("%c%s*M", delimiter[0], stringM);
            count++;
            print = false;
        } 
        if (buffer[i] == '\n')
        {
            printf("\n");
        }
        else if ((N != count) && (M != count))
        {
            printf("%c", buffer[i]);
        }
    }
    
    printf("M%sM", stringM);
    printf("N%sN", stringN);
    printf("\n");
}


//move N M - přesune sloupec N před sloupec M.
//move
void move(int N, int M, char *buffer, char *delimiter)
{
    int count1 = 1;
    char stringN[100] ;
    stringN[0] = '\0';
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (N == count1)
        {
            if ((buffer[i] != delimiter[0]) && (buffer[i] != '\n'))
            {
                append_char(stringN, buffer[i]);
            }    
        }

        if (buffer[i] == delimiter[0])
        {
            count1++;
        }
    }


    
    int count = 1;
    bool print = true;
    for (unsigned int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == delimiter[0])
        {
            count++;
            print = true;
        }
        if (M == count && M == 1)
        {
            printf("%s*N", stringN);
            count++;
            print = false;
        }
        if (N == count && N == 1)
        {
            //printf("%s*M", stringM);
            count++;
            print = false;
        }
        if (M == count && M != 1)
        {
            printf("%c%s*N", delimiter[0], stringN);
            count++;
            print = false;
        }
        if (N == count && N != 1)
        {
            //printf("%c%s*M", delimiter[0], stringM);
            count++;
            print = false;
        } 
        if (buffer[i] == '\n')
        {
            printf("\n");
        }
        else if ((N != count) && (M != count))
        {
            printf("%c+", buffer[i]);
        }
    }
    
    //printf("M%sM", stringM);
    printf("N%sN", stringN);
    printf("\n");
}














int main(int argc, char **argv)
{
    char delimiter[100];
    char buffer[MAX_LINESIZE];

    load_delim(argc, argv, delimiter);
    int no_delim;
    int no_row = 1;

    while (fgets(buffer, MAX_LINESIZE, stdin))
    {
        bool print = true;
        //int ndr = 0;
        //int row_count = no_rows(buffer);
        change_delims(argv, buffer, delimiter);
        if (no_row == 1)
        {
            no_delim = no_delims(buffer, delimiter);
            for (int i = 2; i < argc; i++)
            {
                if (strcmp(argv[i], "icol") == 0)
                {
                    no_delim++;
                }
                if (strcmp(argv[i], "dcol") == 0)
                {
                    no_delim--;
                }
            }
        }
        //ndr = no_delims_row(buffer, delimiter);
        //printf("%d  ", no_delim);
        //printf("   %d", no_delim);
        //Příkazy pro úpravu tabulky:

        for (int i = 2; i < argc; i++)
        {
            //Úprava tabulky

            if (strcmp(argv[i], "irow") == 0)
            {
                int row = strtol(argv[i + 1], NULL, 10);
                irow(no_row, row, delimiter, no_delim);
            }

            if (strcmp(argv[i], "drow") == 0)
            {
                int row = strtol(argv[i + 1], NULL, 10);
                print = drow(no_row, row, buffer);
            }

            if (strcmp(argv[i], "drows") == 0)
            {
                int N = strtol(argv[i + 1], NULL, 10);
                int M = strtol(argv[i + 2], NULL, 10);
                print = drows(no_row, N, M);
            }

            if (strcmp(argv[i], "acol") == 0)
            {
                acol(buffer, delimiter);
            }

            if (strcmp(argv[i], "icol") == 0)
            {
                int cs = strtol(argv[i + 1], NULL, 10);
                icol(cs, buffer, delimiter);
                print = false;
            }

            if (strcmp(argv[i], "dcol") == 0)
            {
                int cs = strtol(argv[i + 1], NULL, 10);
                dcol(cs, buffer, delimiter);
                print = false;
            }

            if (strcmp(argv[i], "dcols") == 0)
            {
                int N = strtol(argv[i + 1], NULL, 10);
                int M = strtol(argv[i + 2], NULL, 10);
                dcols(N, M, buffer, delimiter);
                print = false;
            }

            //Zpracování dat
            if (strcmp(argv[i], "cset") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                char *str = argv[i + 2];
                cset(C, str, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "tolower") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                tlower(C, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "toupper") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                tupper(C, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "round") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                roundc(C, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "copy") == 0)
            {
                int N = strtol(argv[i + 1], NULL, 10);
                int M = strtol(argv[i + 2], NULL, 10);
                copy(N, M, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "swap") == 0)
            {
                int N = strtol(argv[i + 1], NULL, 10);
                int M = strtol(argv[i + 2], NULL, 10);
                swap(N, M, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "int") == 0)
            {
                int C = strtol(argv[i + 1], NULL, 10);
                int_C(C, buffer, delimiter);
                print = false;
            }
            if (strcmp(argv[i], "move") == 0)
            {
                int N = strtol(argv[i + 1], NULL, 10);
                int M = strtol(argv[i + 2], NULL, 10);
                move(N, M, buffer, delimiter);
                print = false;
            }
        }

        if (print)
        {
            printf("%s", buffer);
        }
        no_row++;

        //printf ("    %d" , no_delim);
    }

    //Úprava tabulky
    //arow
    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "arow") == 0)
        {
            arow(delimiter, no_delim);
        }
    }

    //int delim_count = no_delims(buffer, delimiter);//, delimiter);

    //printf("\n delimiter %s", delimiter);
    //printf("%d", no_row);
    //printf("delim_count %d", delim_count);
    //printf("strlen delims %d", strlen(argv[2]));

    return 0;
}
