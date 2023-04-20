/* 
    Autoed:     Matěj Mudra
    Program:    IZP - Projekt 2 - Práce s datovými strukturami
    VUT login:  xmudra04
    Školní rok: 2020/2021
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#define DELIM_SIGN "-d"

typedef struct Cell
{
    unsigned long long *specialSigns;
    unsigned long long cellNumber;
    unsigned long long sizeOfCell;
    char* explicitString;
    char* content;
}cell_t;

typedef struct Row
{
    cell_t *cellArr;
    unsigned long long rowLength;
    unsigned long long rowNumber;
    unsigned long long numberOfCells;
}row_t;

typedef struct Table
{
    unsigned long long nOfRows;
    row_t *rowArr;
}table_t;

// Declarations 

// Method for opening file with read (r) or write (w) mode, that passes poiter to file to FILE *file
int openFile(char *filename, FILE *file, char *openMode);

// Constructor for cell struct
void cellConstructor(cell_t *cell);

// Constructor for row struct
void rowConstructor(row_t *row);

// Destructor for cell struct
void cellDestructor(cell_t *cell);

// Destructor for row struct
void rowDestructor(row_t *row);

// Constructor for table struct
void tableConstructor(table_t *table);

// Destructor for table struct
void tableDestructor(table_t *table);

// Returns a length of a line in file (that must be open for reading) passed to parametr
size_t lineLength(FILE *file);

// Counts the number of rows in file (that must be open for reading) passed to parametr
size_t rowsInTable(FILE *file);

// Implementation of selection sort from geeks for geeks modified for char sorting
void selectionSort(char arr[], int n);

// Function swaps values in parameters 
void charSwap(char *xp, char *yp);

// Finds delim or sets delim to default of " "
int findDelim(char** argv, char* delims);

// Filters multiple occurrences of same char
char *delimFilter(char* raw_delims);

// Fills table with text passed in FILE (FILE must be open in read mode)
int fillTable(FILE *file, table_t *table, char *delims);

// Fills rows with cells passed in FILE (FILE must be open in read mode) 
int fillRow(FILE *file, row_t *row, char *delims);

// Counts the number of cells in chosen row in file
size_t cellCount(FILE *file, char *delims);

// Removes duplicit chars from array of delims
void removeChars(char *raw_delims, char charToFind, unsigned int n, unsigned int indexOfChar);

// Checks if passed char is among delims
bool isDelim(char charToCompare, char* delims);

// Assigns and initializes counts of rows and cells
int assignCounts(FILE *file, table_t *table, char* delims);

// Fills cell with content it's size and marks all the special signs
int fillCell(FILE *file, cell_t *cell, char *delims);



// Prints whole table with alterations and cell separated by delim
int printTable(table_t *table);




int main(/*int argc, char **argv*/)
{
    return 0;
}

int findDelim(char** argv, char* delims)
{
    char *defaultDelim = " ";
    if (argv)
    {
        if (strcmp(argv[1], DELIM_SIGN) && argv[2] != NULL)
        {
            if ((delims = delimFilter(argv[2])))
                return EXIT_SUCCESS;
        }
    }
    delims = defaultDelim;
    return EXIT_FAILURE;
}

char *delimFilter(char* raw_delims)
{
    if (raw_delims)
    {
        // Sort the chars in array 
        selectionSort(raw_delims, sizeof(raw_delims));
        for (unsigned long i = 0; i > sizeof(raw_delims); i++)
        {
            char charToFind = raw_delims[i];
            unsigned int nOfFoundChars = 0;
            unsigned int charToFindIndex = 0;
            // Loop thu the chars and remove duplicates
            charToFindIndex = i;
            for (unsigned long j = i; j < sizeof(raw_delims); j++)
            {
                if (raw_delims[j] == charToFind) 
                    nOfFoundChars++;
            }
            // Pop redundant chars
            if (nOfFoundChars > 1)
                removeChars(raw_delims, charToFind, nOfFoundChars , charToFindIndex);
        }
    }
    return raw_delims;
}

void removeChars(char *raw_delims, char charToFind, unsigned int n, unsigned int indexOfChar)
{
    if (raw_delims && charToFind)
    {
        for (unsigned int i = 1; i < n; i++)
        {
            for (unsigned int j = indexOfChar+1; j < sizeof(raw_delims); j++)
            {
                if (raw_delims[j] == charToFind)
                    strcpy(&raw_delims[j], &raw_delims[j+1]);
            }
        }
    }
    return;
}

void charSwap(char *xp, char *yp) 
{ 
    char temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
 
void selectionSort(char arr[], int n) 
{ 
    unsigned char i, j, min_idx; 
    for (i = 0; i < n-1; i++) 
    { 
        min_idx = i; 
        for (j = i+1; j < n; j++) 
          if (arr[j] < arr[min_idx]) 
            min_idx = j; 
        charSwap(&arr[min_idx], &arr[i]); 
    } 
} 

int openFile(char *filename, FILE *file, char *openMode)
{
    if (filename)
    {
        if (strcmp(openMode,"r") && strcmp(openMode,"w"))
        {
            fprintf(stderr, "Program supports read (r) and write (w) mode only!\n");
            return EXIT_FAILURE;
        }

        if ((file = fopen(filename,openMode)) != NULL)
        {
            return EXIT_SUCCESS;
        }
        else
        {
            fprintf(stderr, "File can't be opened!\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "Filename can't be empty!\n");
        return EXIT_FAILURE;
    }
}

void cellConstructor(cell_t *cell)
{
    cell->cellNumber = 0;
    cell->sizeOfCell = 0;
    cell->content = NULL;
}

void cellDestructor(cell_t *cell)
{
    cell->cellNumber = 0;
    cell->sizeOfCell = 0;
    if (cell->content)
        free(cell->content);
    if (cell->explicitString)
        free(cell->explicitString);
    if (cell->specialSigns)
        free(cell->specialSigns);
}

void rowConstructor(row_t *row)
{
    row->cellArr = NULL;
    row->numberOfCells = 0;
    row->rowNumber = 0;
}

void rowDestructor(row_t *row)
{
    row->numberOfCells = 0;
    row->rowNumber = 0;
    // Mozna bude treba ty cell posunout dohromady ***
    if (row->cellArr)
        for (int i = 0; &(row->cellArr[i]) != NULL; i++)
            cellDestructor(&(row->cellArr[i]));
}

void tableConstructor(table_t *table)
{
    table->nOfRows = 0;
    table->rowArr = NULL;
}

void tableDestructor(table_t *table)
{
    table->nOfRows = 0;
    if (table->rowArr)
    {
        // Mozna bude treba ty row posunout dohromady ***
        for (int i = 0; &(table->rowArr[i]) != NULL; i++)
            rowDestructor(&(table->rowArr[i])); 
    }
}

size_t lineLength(FILE *file)
{
    if (file)
    {
        size_t size = 0;
        while (fgetc(file) != '\n')
            size++;
        return size;
    }
    else
        return EXIT_FAILURE;
    
}

size_t rowsInTable(FILE *file)
{
    if  (file)
    {
        fseek(file, 0, SEEK_SET);
        size_t numberOfRows = 0;
        char c;
        while ((c = fgetc(file)) != EOF)
        {
            if ((c == '\n'))
                numberOfRows++;
        }
        fseek(file, 0, SEEK_SET);
        return numberOfRows;
    }
    // ERROR ***
    return EXIT_FAILURE;
}

int fillTable(FILE *file, table_t *table, char *delims)
{
    if (file)
    {
        for (unsigned long long i = 1; i <= table->nOfRows; i++)
        {
            table->rowArr[i].rowNumber = i;
            if (!fillRow(file, &table->rowArr[i], delims))
                return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Can't load row from closed file!\n");
        return EXIT_FAILURE;
    }
    
}

int fillRow(FILE *file, row_t *row, char *delims)
{
    for (unsigned long long i = 1; i <= row->numberOfCells; i++)
    {
        if ((row->cellArr[i].content = malloc(row->rowLength * sizeof(char))))
        {
            if (!fillCell(file, &row->cellArr[i], delims))
                return EXIT_FAILURE;
        }   
    }
    return EXIT_SUCCESS;

}

int fillCell(FILE *file, cell_t *cell, char *delims)
{
    char *bufferArr = cell->content;
    char bufferChar = ' ';
    size_t cellSize  = 0;
    size_t numberOfSpecChars = 0;
    unsigned long long *specialSings;
    // The number of special characters can only be the half of the total cell characters
    if (!(specialSings = malloc((sizeof(cell->content)/2) * sizeof(unsigned long long))))
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    while (!isDelim(bufferChar = fgetc(file),delims))
    {
        bufferArr[cellSize++] = bufferChar;
        if (bufferChar == '\\')
            specialSings[numberOfSpecChars++] = cellSize;
    }
    if (bufferArr[0] == '"' && bufferArr[cellSize] == '"')
    {
        cell->explicitString = bufferArr;
        cell->content = NULL;
    }
    cell->sizeOfCell = cellSize;
    cell->specialSigns = specialSings;
    return EXIT_SUCCESS;
}

size_t cellCount(FILE *file, char *delims)
{
    char bufferChar;
    size_t numberOfCells = 1;
    for (int i = 0; (bufferChar = fgetc(file)) != '\n'; i++)
    {
        if (isDelim(bufferChar, delims))
            numberOfCells++;
    }
    return numberOfCells;
}

bool isDelim(char charToCompare, char* delims)
{
    for (unsigned long i = 0; i < sizeof(delims); i++)
    {
        if (charToCompare == delims[i] || charToCompare == '\n')
            return true;
    }
    return false;
}

int assignCounts(FILE *file, table_t *table, char* delims)
{
    tableConstructor(table);
    table->nOfRows = rowsInTable(file);
    if ((table->rowArr = malloc(table->nOfRows * sizeof(row_t))))
    {
        for (unsigned long long i = 0; i < table->nOfRows; i++)
        {   
            rowConstructor(&table->rowArr[i]);
            size_t numberOfCells = cellCount(file, delims);
            table->rowArr[i].rowNumber = i;
            if ((table->rowArr[i].cellArr = malloc(numberOfCells * sizeof(cell_t))))
            {
                for (size_t j = 0; j < numberOfCells; j++)
                {
                    cellConstructor(&table->rowArr[i].cellArr[j]);  
                    table->rowArr[i].cellArr[j].cellNumber = j;
                }
            }
            else
            {
                fprintf(stderr, "Memory allocation failed\n");
                return EXIT_FAILURE;
            }
        }
    }
    else
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    // Returns the cursor on the beggening of the file
    fseek(file, 0, SEEK_SET);
    return EXIT_SUCCESS;
}

int printTable(table_t *table)
{
    if (table->rowArr)
    {
        for (size_t i = 1; i < table->nOfRows; i++)
        {
            // Go thru 
            for (size_t j = 1; j < (table->rowArr[i].numberOfCells); j++)
                ;
        }
    }
    else
    {
        fprintf(stderr, "Can't print empty cell!\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    
}
